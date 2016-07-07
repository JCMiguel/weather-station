/************************************************************************\
|*	       Estación meteorológica con registro en microSD		*|
|************************************************************************|
|* (IN ENGLISH)		      Weather station with SD card registration	*|
|* (AUF DEUTSCH)	     Wetterstation  mit SD-Karten-Registrierung	*|
|*   	    Miguel, Juan Cristian Daniel    Copyright © 2016		*|
\************************************************************************/
/*
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/************************************************************************\
|*	            FUNCIÓN PRINCIPAL DE PROGRAMA			*|
|************************************************************************|
|* Descripcion:                                                         *|
|*  Este archivo sólo contiene la función principal (main) y las ruti-  *|
|*  nas para cada vector de interrupción utilizado.			*|
\************************************************************************/

#include "config.h"
#include "LCD.h"
#include "pff.h"
#include "diskio.h"
#include "error_checking.h"

volatile unsigned char var=0;
volatile unsigned char key=0;
volatile unsigned char time2sd=0;
volatile unsigned char ovf_tim1=0;	//Cantidad de OVF del Timer1
volatile unsigned int ovf_nec;	//OVF necesarios.
volatile unsigned char ovf_tim0=0;	//Cantidad de OVF del Timer0
volatile unsigned char time=0;
volatile unsigned char uno=0;
volatile unsigned char tiempo[43];
volatile unsigned char ovfs[43]; //


int main (void)
{
	//Para usos diversos.
	unsigned char data[5];
	unsigned short int counter, i=0, j=0, len1;
	char laststate=0, state=0, nm=0;
	int hume;
	int tempe;
	int wind;

	//Para representación en LCD.
	static char asciihume[5];
	static char asciitempe[5];
	static char asciiwind[2];
	static char cadena1[] = {"H:"};
	static char cadena2[] = {"% T:"};
	static char cadena3[] = {"'C "};
	static char cadena4[] = {"V:"};
	static char cadena5[] = {"Km/h  "};
	static char jumplin[] = {"\n"};

	static char asciires[5];
	static char auxcad[256];
	static char cadinit[] = {"INICIANDO DISCO"};
	static char cadmnt[] =  {"MONTANDO UNIDAD"};
	static char cadopen[] = {"ABRIENDO ARCHIVO"};
	static char cadread[] = {"LEYENDO DATOS..."};
	static char cadwrte[] = {"GRABANDO SD..."};

	static char cadprog[] ={"|"};
	static char cadX[] = {"X"};
	static char cadOK[] ={"OK"};
	static char cadFAIL[] = {"ERROR"};

	//Para la tarjeta SD
	FATFS fs;
	UINT br;
	char *buff;
	static DWORD datawrite = 0;
	int res = 7;


	//Usaré el timer en modo normal, y tomaré sus OVF como base de tiempo.
	config_timer1;

	//Usaré el timer en modo normal, y tomaré sus OVF como base de tiempo.
    	config_timer0;

	//Valores iniciales del vector. Deberían ser cero, salvo para pruebas.
	data[0]=2;
	data[1]=0x84;
	data[2]=0;
	data[3]=0xAE;
	init_ports();
	//init_lcd();

	//Inicialización de disco.
	res = disk_initialize();
	 init_lcd();
	 len1=strlen(cadinit);
	 for (i=0; i< len1;i++)
	  	lcd_send(cadinit[i]);
	next_line();
	check_operation_fat(res);
	_delay_ms(1000);

	//Montaje del sistema de archivos FAT.
	res = pf_mount(&fs);
  	 init_lcd();
	 len1=strlen(cadmnt);
	 for (i=0; i< len1;i++)
	  	lcd_send(cadmnt[i]);
	next_line();
	check_operation_fat(res);
	_delay_ms(1000);


	while(1)
	{
		//Empieza comunicación con el sensor.
	 	if(key==0)
		{	key++;

			ovf_nec = DELAY_1s; //OVF a transcurrir antes de mostrar el resultado por pantalla.

			set_bit(SENS,DDRD); //Configuro el pin como salida.
	 		clr_bit(SENS,PB);   //Pongo en cero el Bit SENS.
			_delay_ms(2);	    //_delay_us(2000);	//Acá falta el delay. (2ms)
			set_bit(SENS,PB);   //Pongo en uno el Bit SENS.
			_delay_us(40);	    //Acá otro delay (40us)
			clr_bit(SENS,PB);
			clr_bit(SENS,DDRD); //Configuro como entrada

			config_int;
			listen_int;
			sei();

			start_timer1;
		}

		if(key==1)
		{
			state = PIND & 0x08;
			if(state != laststate)
			{
				nm++;
				laststate=state;
			}
			start_timer1;
		}


		//Ya pasó un segundo...
		if(key==2 && time2sd<10)
		{
			stop_int;
			stop_timer0;
			stop_timer1;
			ovf_tim1=0;
			ovf_tim0=0;
			key = 0;

			//Proceso data
			for(j=1;j<17;j++)
			{
				//if((tiempo[j]*256+minitemp[j]) > UMBRAL)
				if(tiempo[j] > UMBRAL)
					data[(j-1)/8] |= 0x01;
				data[(j-1)/8] <<= 1;
			}


			hume = data[0]; //& 0x7F;
			hume *= 256;
        		hume += data[1];
        		hume /= 10; //divido por 80 porque se desplazó tres bits.

			for(j=20;j<36;j++)
			{
				//if((tiempo[j]*256+minitemp[j]) > UMBRAL)
				if(tiempo[j] > UMBRAL)
					data[(j-2)/8] |= 0x01;
				data[(j-2)/8] <<= 1;
			}

			tempe = data[2] & 0x7F;
			tempe *= 256;
        		tempe += data[3];
        		tempe /= 10;	//Corrección del resultado.

			wind=Kte*nm/(N);

			
			init_lcd();

			len1=strlen(cadena1);
			for (i=0; i< len1;i++)
	  			lcd_send(cadena1[i]);

			itoa(hume,asciihume,10);
			len1=strlen(asciihume);
			for (i=0; i< len1;i++)
	  			lcd_send(asciihume[i]);

			len1=strlen(cadena2);
			for (i=0; i< len1;i++)
	  			lcd_send(cadena2[i]);

			itoa(tempe,asciitempe,10);
			len1=strlen(asciitempe);
			for (i=0; i< len1;i++)
	  			lcd_send(asciitempe[i]);

			len1=strlen(cadena3);
			for (i=0; i< len1;i++)
	  			lcd_send(cadena3[i]);

			next_line();

			len1=strlen(cadena4);
			for (i=0; i< len1;i++)
  				lcd_send(cadena4[i]);

			itoa(wind,asciiwind,10);
			len1=strlen(asciiwind);
			for (i=0; i< len1;i++)
  				lcd_send(asciiwind[i]);

			len1=strlen(cadena5);
			for (i=0; i< len1;i++)
  				lcd_send(cadena5[i]);

			nm = 0;
			var = 0;
		}


		//Time to SD Card!
		if(time2sd == 10)
		{
			stop_int;
			stop_timer0;
			stop_timer1;
			ovf_tim1=0;
			ovf_tim0=0;
			key = 0;
			time2sd = 0;

			//Proceso data
			for(j=1;j<17;j++)
			{
				if(tiempo[j] > UMBRAL)
					data[(j-1)/8] |= 0x01;
				data[(j-1)/8] <<= 1;
			}
			hume = data[0]; //& 0x7F;
			hume *= 256;
        		hume += data[1];
        		hume /= 10;
			itoa(hume,asciihume,10);

			for(j=20;j<36;j++)
			{
				if(tiempo[j] > UMBRAL)
					data[(j-2)/8] |= 0x01;
				data[(j-2)/8] <<= 1;
			}
			tempe = data[2] & 0x7F;
			tempe *= 256;
        		tempe += data[3];
        		tempe /= 10;	//Corrección del resultado.
			itoa(tempe,asciitempe,10);

			wind=Kte*nm/(N);
			itoa(wind,asciiwind,10);

//ACá está lo de la SD.
			init_lcd();
			res = pf_open("DATA.TXT");
			 len1=strlen(cadopen);
			 for (i=0; i< len1;i++)
			  	lcd_send(cadopen[i]);
			next_line();
			check_operation_fat(res);
			_delay_ms(1000);

//Me posiciono dentro del archivo.
//Leo el contenido del archivo.
			init_lcd();
			res = pf_read(auxcad,datawrite,&br);
			 len1=strlen(cadread);
			 for (i=0; i< len1;i++)
			  	lcd_send(cadread[i]);
			next_line();
			check_operation_fat(res);
			 _delay_ms(1000);
//Regrabo lo que ya tiene.
			init_lcd();
			res = pf_write(auxcad,(UINT)datawrite,&br);
			 len1=strlen(cadwrte);
			 for (i=0; i< len1;i++)
			  	lcd_send(cadwrte[i]);
			next_line();
			check_progress_bar(res);
			 _delay_ms(1000);

//Escribiendo la SD
			len1=strlen(cadena1);
			 datawrite+=len1;
			res = pf_write(cadena1,(UINT)len1,&br);
			check_progress_bar(res);
			 _delay_ms(1000);

			len1=strlen(asciihume);
			 datawrite+=len1;
			res = pf_write(asciihume,(UINT)len1,&br);
			check_progress_bar(res);
			 _delay_ms(1000);

//Escribiendo la SD
			len1=strlen(cadena2);
			 datawrite+=len1;
			res = pf_write(cadena2,(UINT)len1,&br);
			check_progress_bar(res);
			 _delay_ms(1000);

			len1=strlen(asciitempe);
			 datawrite+=len1;
			res = pf_write(asciitempe,(UINT)len1,&br);
			check_progress_bar(res);
			 _delay_ms(1000);

			len1=strlen(cadena3);
			 datawrite+=len1;
			res = pf_write(cadena3,(UINT)len1,&br);
			check_progress_bar(res);
			 _delay_ms(1000);

//Escribiendo el viento
			len1=strlen(cadena4);
			 datawrite+=len1;
			res = pf_write(cadena4,(UINT)len1,&br);
			check_progress_bar(res);
			 _delay_ms(1000);

			len1=strlen(asciiwind);
			 datawrite+=len1;
			res = pf_write(asciiwind,(UINT)len1,&br);
			check_progress_bar(res);
			 _delay_ms(1000);

			len1=strlen(cadena5);
			 datawrite+=len1;
			res = pf_write(cadena5,(UINT)len1,&br);
			check_progress_bar(res);
			 _delay_ms(1000);

			len1=strlen(jumplin);
			 datawrite+=len1;
			res = pf_write(jumplin,(UINT)len1,&br);
			check_progress_bar(res);
			 _delay_ms(1000);

//Cerrando el archivo.
			res = pf_write(0, 0, &br);
			check_progress_bar(res);
			 _delay_ms(1000);

			var = 0;
		}
	}
}

ISR(INT0_vect)    // External Interrupt 0 ISR  Interrupt Service Routine
{
	stop_timer0;

	if(var>1)
	{
		//timer[var-1]=TCNT0;   //
		tiempo[var-1]=ovf_tim0; //
	}

	var++;	//cuenta un pulso.

	ovf_tim0=0;
	TCNT0=0;
	start_timer0; //Arranco el timer de nuevo
}

ISR(TIMER1_OVF_vect)
{
	if(ovf_tim1==ovf_nec)
	{
		key++;	time2sd++;
	}
	ovf_tim1++;
}

ISR(TIMER0_OVF_vect)
{
	ovf_tim0++; //
}

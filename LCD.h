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
|*            ARCHIVO DE CABECERA PARA EMPLEO DE DISPLAY LCD		*|
|************************************************************************|
|* Descripcion:                                                         *|
|*  En éste, se encuentran las definiciones, macros de configuración y 	*|
|*  prototipos de funciones para el uso del display LCD. Este header	*|
|*  está basado en otro subido a un foro, pero contiene modificaciones	*|
|*  acordes al proyecto donde se implementa.				*|
|*  Los datos al display no se transmiten a través de un único puerto	*|
|*  del microcontrolador. Esto se debe a un problema de diseño del	*|
|*  circuito impreso.							*|
|*  Para solventar este inconveniente, se usan macros que dividen los	*|
|*  caracteres a transmitir en bits que se derivan al pin que corres-	*|
|*  ponda.								*|
\************************************************************************/

#ifndef LCD_HEADER
#define LCD_HEADER

#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Definiciones para utilización de displayLCD (puertos de datos y control)
#define DATA_DDR1		DDRB
#define DATA_PORT1		PORTB
#define DATA_PORT2		PORTC
#define DATA_DDR2		DDRC
#define CONTROL_DDR		DDRD
#define CONTROL_PORT 		PORTD
#define ENABLE_BIT		6
#define REGSELECT_BIT		4     
#define READ_WRITE		5
#define CONTROL_MASK		0X70
#define SET_STATUS		1
#define CLEAR_STATUS		0


#ifndef REGBIT
// Definición de campo de bits.
typedef struct{
	unsigned char B0:1;
	unsigned char B1:1;
	unsigned char B2:1;
	unsigned char B3:1;
	unsigned char B4:1;
	unsigned char B5:1;
	unsigned char B6:1;
	unsigned char B7:1;
}bit_t;
#define REGBIT(reg,bit)		((volatile bit_t*)&reg)->B##bit	//Macro para manejar registros de a bit
#endif


#define ENABLE_LCD  				CONTROL_PORT|=(1<<ENABLE_BIT)
#define DISABLE_LCD 				CONTROL_PORT&=~(1<<ENABLE_BIT)
#define WRITE_LCD			    	CONTROL_PORT&=~(1<<READ_WRITE)
#define READ_LCD			  	CONTROL_PORT|=(1<<READ_WRITE)
#define INSTRUCTION_SELECT 			CONTROL_PORT&=~(1<<REGSELECT_BIT)
#define DATA_REGISTER_SELECT	   	 	CONTROL_PORT|=(1<<REGSELECT_BIT)
#define DATA_LCD(a)				REGBIT(DATA_PORT1,7) = REGBIT(a,7); \
						REGBIT(DATA_PORT1,6) = REGBIT(a,6); \
						REGBIT(DATA_PORT2,3) = REGBIT(a,5); \
						REGBIT(DATA_PORT2,2) = REGBIT(a,4); \
						REGBIT(DATA_PORT2,1) = REGBIT(a,3); \
						REGBIT(DATA_PORT2,0) = REGBIT(a,2); \
						REGBIT(DATA_PORT1,1) = REGBIT(a,1); \
						REGBIT(DATA_PORT1,0) = REGBIT(a,0)

void init_ports(void);
void init_lcd(void);
void rewrite_lcd(void);
void clear_lcd(void);
void lcd_send(unsigned char);
void next_line(void);

#endif

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
|*            	    FUNCIONES PARA EMPLEO DE DISPLAY LCD		*|
|************************************************************************|
|* Descripcion:                                                         *|
|*  Este archivo contiene todas las funciones necesarias para la utili-	*|
|*  zación de display LCD en el proyecto. El mismo se utiliza tanto	*|
|*  para mostrar la información de los sensores como para chequear	*|
|*  errores tras operaciones. Ver archivos "error_checking".		*|
\************************************************************************/

#include "LCD.h"


void lcd_send(unsigned char a)
{
	DATA_REGISTER_SELECT;
	WRITE_LCD;
	DATA_LCD(a);
	ENABLE_LCD;
	_delay_ms(1);
	DISABLE_LCD;
	_delay_ms(1);
}

 	
void init_ports(void)
{
	DATA_DDR1|=0xC3;
	DATA_DDR2|=0x0F;
	CONTROL_DDR=CONTROL_MASK;
	CONTROL_PORT&=~((1<<ENABLE_BIT)|(1<<REGSELECT_BIT )|(1<<READ_WRITE));
}


void init_lcd(void)
{
	char init[5];
	int i;
	init[0] = 0x0C;
	init[1] = 0x38;
	init[2] = 0x06;
	init[3] = 0x80;
	init[4] = 0x01;
	INSTRUCTION_SELECT;
	WRITE_LCD;
	for(i=0;i<5;i++)
	{
		DATA_LCD(init[i]);
		ENABLE_LCD;
		_delay_ms(1);
		DISABLE_LCD;
		_delay_ms(1);
	}

}


void rewrite_lcd(void)
{
	char init[5];

	int i;
	init[0] = 0x0C;
	init[1] = 0x38;
	init[2] = 0x06;
	init[3] = 0x80;
	//init[4] = 0x01;
	INSTRUCTION_SELECT;
	WRITE_LCD;
	for(i=0;i<4;i++)
	{
		DATA_LCD(init[i]);
		ENABLE_LCD;
		_delay_ms(1);
		DISABLE_LCD;
		_delay_ms(1);
	}
}


void clear_lcd(void)
{
	char cmd=0x01;
	INSTRUCTION_SELECT;
	WRITE_LCD;
	DATA_LCD(cmd);
	ENABLE_LCD;
	_delay_ms(1);
	DISABLE_LCD;
	_delay_ms(1);
}


void next_line(void)
{
	char caracter = 0xC0;	
	INSTRUCTION_SELECT;
	WRITE_LCD;
	DATA_LCD(caracter);
	ENABLE_LCD;
	_delay_ms(1);
	DISABLE_LCD;
}

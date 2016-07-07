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
|*	       FUNCIONES DE RUTINAS DE CHEQUEO DE ERROR			*|
|************************************************************************|
|* Descripcion:                                                         *|
|*  Archivo que contiene las funciones de informe de errores en opera-	*|
|*  ciones FAT.								*|
\************************************************************************/

#include "error_checking.h"

void check_operation_fat(int res)
{
	static char cadOK[] ={"OK"};
	static char cadFAIL[] = {"ERROR"};
	unsigned char len;
	unsigned char i;

	if(!res)
	{
		len=strlen(cadOK);
		for (i=0; i< len;i++)
		  	lcd_send(cadOK[i]);
	}
	else
	{
		len=strlen(cadFAIL);
		for (i=0; i< len;i++)
		  	lcd_send(cadFAIL[i]);
	}
}


void check_progress_bar(int res)
{
	static char cadprog[] ={"|"};
	static char cadX[] = {"X"};
	unsigned char len;
	char i;

	if(!res)
	{
		len=strlen(cadprog);
		for (i=0; i< len;i++)
		  	lcd_send(cadprog[i]);
	}
	else
	{
		len=strlen(cadX);
		for (i=0; i< len;i++)
		  	lcd_send(cadX[i]);
	}
}

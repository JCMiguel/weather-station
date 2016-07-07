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
|*         ARCHIVO DE CABECERA PARA RUTINAS DE CHEQUEO DE ERROR		*|
|************************************************************************|
|* Descripcion:                                                         *|
|*  Este header contiene los prototipos de funciones para informe de	*|
|*  errores en operaciones relacionadas al sistema de archivos FAT.	*|
\************************************************************************/

#ifndef ERROR_CHK
#define ERROR_CHK

#include <string.h>

void check_operation_fat(int);
void check_progress_bar(int);

#endif

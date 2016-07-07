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
|*           ARCHIVO DE CABECERA PARA COMUNICACIÓN SERIE (SPI)		*|
|************************************************************************|
|* Descripcion:                                                         *|
|*  SPI es el protocolo de comunicación empleado para comunicarse con	*|
|*  la memoria microSD. Aquí se encuentran definiciones y prototipos 	*|
|*  de funciones afines. Los prototipos de funciones de	comunicación 	*|
|*  con la tarjeta SD están descriptas en la biblioteca Petit FATFs 	*|
|*  de Elm Chan, utilizadas en este proyecto.				*|
\************************************************************************/

#ifndef SPICOM
#define SPICOM

#include "config.h"

#define MOSI	3
#define MISO	4
#define SS	2
#define	SCK	5

#define SELECT()	clr_bit(SS,PORTB)	/* CS = L */
#define	DESELECT()	set_bit(SS,PORTB)	/* CS = H */
#define	MMC_SEL		!(PORTB & (1<<SS))	/* CS status (true:CS == L) */

void init_spi (void);		/* Initialize SPI port */
void xmit_spi (BYTE d);		/* Send a byte to the SDCard */
BYTE rcv_spi (void);		/* Send a 0xFF to the SDCard */

#endif

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
|*	       FUNCIONES DE COMUNICACIÓN SERIE (SPI)			*|
|************************************************************************|
|* Descripcion:                                                         *|
|*  SPI es el protocolo de comunicación empleado para comunicarse con	*|
|*  la memoria microSD. Aquí se encuentran las funciones elementales 	*|
|*  de transmisión y recepción de bytes por SPI. Las funciones de	*|
|*  comunicación con la tarjeta SD están descriptas en la biblioteca	*|
|*  Petit FATFs de Elm Chan, utilizadas en este proyecto.		*|
\************************************************************************/

#include "spi.h"

void init_spi (void)
{	
	set_bit(SPE,SPCR);	//Enable SPI.
	set_bit(MSTR,SPCR);	//Atmega328P como MASTER.
	clr_bit(DORD,SPCR);	//Empiezo transmitiendo el LSB.
	SPCR |= (0<<SPR1) | (1<<SPR0);	//fosc/64 (a 8MHz, da 125KHz)

	PORTB &= 0xC3;
	set_bit(MOSI,DDRB);
	clr_bit(MISO,DDRB);
	set_bit(SCK,DDRB);
	set_bit(SS,DDRB);	
	return;
}


void xmit_spi (BYTE d)
{	
	/* Start transmission */
	SPDR = d;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	return;
}


BYTE rcv_spi (void)
{	
	/* Start transmission */
	SPDR = 0xFF;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));

	
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)));
	/* Return Data Register */
	return SPDR;
}

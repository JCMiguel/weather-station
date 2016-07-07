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
|*		     ARCHIVO CABECERA DE CONFIGURACIÓN			*|
|************************************************************************|
|* Descripcion:                                                         *|
|*  Este header tiene todas las configuraciones del funcionamiento del  *|
|*  programa. Es decir, aquí se definen los puertos, macros generales,	*|
|*  etc.								*|
\************************************************************************/

#ifndef	CONFIG_H
#define	CONFIG_H

//Bibliotecas 
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "integer.h"
#include "spi.h"

//------------ MACROS ------------//
// Manejo de bits.
#define set_bit(bit,reg) reg|=(1<<bit)
#define clr_bit(bit,reg) reg&=~(1<<bit)
// Macros para Timers.
#define start_timer0	set_bit(TOIE0,TIMSK0)
#define stop_timer0	clr_bit(TOIE0,TIMSK0)
#define config_timer0	TCCR0A=0b00000000; TCCR0B=0b00000001; TCNT0=0
	//1-Las salidas OCR no están activadas. 2-Sin prescaler y modo Normal. 3-Valor inicial del timer.
#define start_timer1	set_bit(TOIE1,TIMSK1)
#define stop_timer1	clr_bit(TOIE1,TIMSK1)
#define config_timer1	TCCR1A=0b00000000; TCCR1B=0b00000001; TCNT1=0
	//1-Las salidas OCR no están activadas. 2-Sin prescaler y modo Normal. 3-Valor inicial del timer.
// Macros para interrupción externa.
#define listen_int	set_bit(INT0,EIMSK)
#define stop_int	clr_bit(INT0,EIMSK)
#define config_int	EICRA = (1<<ISC01) | (1<<ISC00)


//Constantes relacionadas a los puertos.
#define PB	PORTD
#define SENS 	2
//Constantes de configuración
#define DELAY_1s	(F_CPU/65536)
#define DELAY_500ms	((0.5*F_CPU)/65536)
#define DELAY_2ms	65536-16000	//valor que hay que cargar en TCNT.
#define DELAY_30us	65536-240	//valor que hay que cargar en TCNT.
#define MAXTIMINGS 	85	//¿Estoy usando esto...?
#define UMBRAL 		2//((80*F_CPU/1000000)/(256)) //Empírico.
#define N 	8	 //Cantidad de marcas del encoder
#define Kte	2	 //d*2*pi*3600 [d = 10cm]
#endif	//CONFIG_H


/**
  ******************************************************************************
  * @file    eon_string.h 
  * @author  Pablo Fuentes
	* @version V1.0.1
  * @date    2019
  * @brief   Header de STRING Library
  ******************************************************************************
*/

#ifndef __EON_STRING_H
#define __EON_STRING_H

#include "stdint.h"
#include <string.h>

/** 
 ===============================================================================
              ##### DEFINICIONES #####
 ===============================================================================
 */

/* Common Terminators */
#define CNULL '\0'
#define ENTER '\n'

/* Degrees Symbol Number for FONT5X7 */
#define DEGREES_5X7 254
/* Degrees Symbol Number for FONT3X5 */
#define DEGREES_3X5 127

/* Easy conditions */
#define _IF_C_IS_NOT_CR(__c__) ((c != 0) && (c != '\r') && (c != '\n'))
#define _IF_C_IS_CR(__c__) ((c != 0) && (c == '\r'))

/*- CHAR2NUM and CHARISNUM Definitions -*/
#define _CHAR2NUM(__x__) ((__x__) - '0')
#define _CHARISNUM(__x__) ((__x__) >= '0' && (__x__) <= '9')

/** 
 ===============================================================================
             ##### FUNCIONES #####
 ===============================================================================
 */

uint8_t str_compare(char varstr[], char *conststr);
int str_length(char str[], char terminator);
double str2float(char buff[]);
int str_indexOf(char str[], const char *target);
int str_indexOfFrom(char str[], const char *target, int index);
void str_clear(char str[]);
void str_zeros(char str[], uint16_t len);

#endif

/***************************FIN DEL ARCHIVO**************************/

/**
  ******************************************************************************
  * @file    eon_string.c 
  * @author  Pablo Fuentes
	* @version V1.0.0
  * @date    2019
  * @brief   STRING Functions
  ******************************************************************************
*/

#include "eon_string.h"

/** 
 ===============================================================================
              ##### FUNCIONES #####
 ===============================================================================
 */

/** 
  * @brief  Compare a char array (array) with other (pointer)
	* @param  {varstr} Variable array
	* @param  {constr} Constant array
  */
uint8_t str_compare(char varstr[], char *conststr)
{
	uint8_t i = 0;
	uint8_t result = 1;
	while (conststr[i] != '\0')
	{
		if (varstr[i] != conststr[i])
		{
			result = 0;
			break;
		}
		i++;
	}
	return result;
}

/** 
  * @brief  Length of string until terminator
	* @param  {str} String to calculate length
	* @param  {terminator} Terminator
  */
int str_length(char str[], char terminator)
{
	int i = 0;
	while (str[i] != terminator)
	{
		i++;
	}
	return i;
}

/**
 * @brief Convert a string to a float number
 * 
 * @param {buff} Buffer to convert. Ex: "45.234"
 * @return {double} Float number
 */
double str2float(char buff[])
{
	double rVal = 0;
	uint8_t i = 0;
	uint8_t decimals = 0;
	uint32_t num_integer = 0;
	uint16_t num_decimal = 0;

	while (_CHARISNUM(buff[i]))
	{
		num_integer = num_integer * 10 + _CHAR2NUM(buff[i]);
		i++;
	}
	i++;
	while (_CHARISNUM(buff[i]))
	{
		num_decimal = num_decimal * 10 + _CHAR2NUM(buff[i]);
		i++;
		decimals++;
	}

	if (decimals == 0)
		rVal = (double)(num_decimal);
	else if (decimals == 1)
		rVal = (double)(num_decimal) / 10;
	else if (decimals == 2)
		rVal = (double)(num_decimal) / 100;
	else if (decimals == 3)
		rVal = (double)(num_decimal) / 1000;
	else if (decimals == 4)
		rVal = (double)(num_decimal) / 10000;
	rVal = rVal + num_integer;

	return rVal;
}

/**
 * @brief Returns the position of target inside str. -1 if it isn't contain it.
 * 
 * @param {str} String which must contain the target string
 * @param {target} Target string 
 * @return {int} Position of target into string 
 */
int str_indexOf(char str[], const char *target)
{
	char *p;
	p = strstr(str, target);
	if (p == 0)
		return -1;
	return (p - str);
}

/**
 * @brief Returns the position of target inside str from a start pos. -1 if it isn't contain it.
 * 
 * @param {str} String which must contain the target string
 * @param {target} Target string 
 * @param {index} Start position 
 * @return {int} Position of target into string 
 */
int str_indexOfFrom(char str[], const char *target, int index)
{
	char *p;
	p = strstr(str + index, target);
	if (p == 0)
		return -1;
	return (p - str);
}

/**
 * @brief Clear array buffer
 * 
 * @param {str} Array buffer to be cleared 
 */
void str_clear(char str[])
{
	uint8_t i = 0;
	while (str[i] != '\0')
	{
		str[i] = '\0';
		i++;
	}
}

/**
 * @brief Fill the array buffer with zeros
 * 
 * @param {str} Array buffer to be filled by zeros 
 * @param {len} Length of the array 
 */
void str_zeros(char str[], uint16_t len)
{
	uint8_t i = 0;
	for (i = 0; i < len; i++)
	{
		str[i] = 0;
	}
}

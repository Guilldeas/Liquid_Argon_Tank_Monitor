/******************************************************************
 ******************************************************************
  CIEMAT, Avolar Project, PCB rev2.5

  Internship: Guillermo, de Arana, Schoebel.

  03/02/2022
--------------------------------------------------------------------

  Header file with PINs and variables.
  
*******************************************************************
*******************************************************************/


/* ifndef evaluates whether the header has already been linked so 
   that the header is not included twice. All code must be contained between
   '#define PINS_AND_VARIABLES_H' and '#endif'
*/

#ifndef PINS_AND_VARIABLES_H  
#define PINS_AND_VARIABLES_H



/******************************************************************
  Variables
*******************************************************************/
// Capacitance reading variables
//extern int calAdress;  
//extern float cableCap;


/******************************************************************
  Front Panel Control 
*******************************************************************/
// Switches
#define SWT_LEVL 51
#define SWT_TEMP 3
#define SWT_CAL_MAX 5   // coincide en mult y esquemático 
#define SWT_CAL_MIN 2   // coincide en mult y esquemático 
#define SWT_LIM_PR_LV 3  // parece que está conectado a un condensador pero nada mas
#define SWT_LIM 2        // parece que está conectado al 5 en la placa
#define SWT_CTRL_STOP 52    
#define SWT_CAL 50    // Según lo que he visto con el multímetro

// LEDs
#define TEST_LED 13
#define LED_TEMP 48
#define LED_LEVEL 49
#define LED_MAX_DEP1 47
#define LED_ERROR 46
#define LED_LOW_DEP1 45
#define LED_MID_DEP1 44
#define LED_SPARE 43
#define LED_LOW_DEP3 42


// Temperature
#define TEMPERAT0 A13
#define TEMPERAT1 A12
#define TEMPERAT2 A11
#define TEMPERAT3 A10

// Level
#define LEVEL3 22
#define LEVEL2 23
#define LEVEL0 24
#define LEVEL1 25
#define LEVEL_GASBRD0 26
#define LEVEL_GASBRD1 27
#define LEVEL_GASBRD2 28
#define LEVEL_GASBRD3 29
#define LEVEL_ID2 30
#define LEVEL_ID1 31
#define LEVEL_ID0 32
#define LEVEL_SPARCFG 33
#define LEVEL_ID3 34
#define LEVEL_ID4 35
#define LEVEL_ID5 36
#define LEVEL_ID6 37

// Muxes
#define SEL_CFG_LEVL 53

// Display
#define DISP_DIN 41
#define DISP_ENA 40
#define DISP_CLK 39



#endif

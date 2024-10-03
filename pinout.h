/*
 * Arduino pinout definitions for ALL boards
 */

#ifndef _PINOUT_H_
#define _PINOUT_H_

//----------------------------------------------------------
//--- BACK-PLANE SIGNALS
//----------------------------------------------------------
#define BKPLN_ERROR0    9
#define BKPLN_ERROR1    10
#define BKPLN_ERROR2    11

#define BKPLN_LEVEL0    A5
#define BKPLN_LEVEL1    A3
#define BKPLN_LEVEL2    A1

#define BKPLN_TEMP0     A4
#define BKPLN_TEMP1     A2
#define BKPLN_TEMP2     A0

//----------------------------------------------------------
//--- COMMUNICATIONS
//----------------------------------------------------------
/* Auxiliary RX/TX bus (digital) */
#define AUX_RX0         0
#define AUX_TX0         1

/* I2C Bus */
#define I2C_SDA         20
#define I2C_SCL         21

//----------------------------------------------------------
//--- FRONT PANEL DISPLAYS
//----------------------------------------------------------
/* Display level out (analog) */
#define DSPBAR_LEVEL    8

/* Digital 7-seg display control */
#define DISPLAY_DIN     41
#define DISPLAY_ENA     40
#define DISPLAY_CLK     39

/* Leds */
#define LED_FRONT1      48
#define LED_FRONT2      49
#define LED_FRONT3      46
#define LED_FRONT4      47
#define LED_FRONT5      44
#define LED_FRONT6      45
#define LED_FRONT7      42
#define LED_FRONT8      43

//----------------------------------------------------------
//--- LEVEL
//----------------------------------------------------------
/* Tank-level digital inputs */
#define D_LEVEL0        24
#define D_LEVEL1        25
#define D_LEVEL2        23
#define D_LEVEL3        22
#define D_LEVEL4_GAS0   26
#define D_LEVEL5_GAS1   27
#define D_LEVEL6_BRD0   28
#define D_LEVEL7_BRD1   29

#define D_LEVEL8_SCFG   33
#define D_LEVEL9_ID0    32
#define D_LEVEL10_ID1   31
#define D_LEVEL11_ID2   30
#define D_LEVEL12_ID3   34
#define D_LEVEL13_ID4   35
#define D_LEVEL14_ID5   36
#define D_LEVEL15_ID6   37

#define LEVEL_ERROR     38

/* On-board and valves connector analog level out */
#define A_LEVEL_OUT     12

//----------------------------------------------------------
//--- PRESSURES
//----------------------------------------------------------
/* Temperature analog inputs */
#define A_PRESS0        A8
#define A_PRESS1        A9
#define A_PRESS2        A15
#define A_PRESS3        A14

//----------------------------------------------------------
//--- SWITCHES (Front-panel)
//----------------------------------------------------------
#define SWITCH1         51         //SW_DISP_TMPCAP1
#define SWITCH2         3          //SW_DISP_TMPCAP0
#define SWITCH5         50         //SW_CALIBRATION
#define SWITCH6         2          //SW_MAXMIN0
#define SWITCH7         5          //SW_MAXMIN1
#define SWITCH10        52         //SW_START_STOP
#define SWITCH11        4          //SWITCH6

//----------------------------------------------------------
//--- TEMPERATURES
//----------------------------------------------------------
/* Temperature analog inputs */
#define A_TEMP0         A13
#define A_TEMP1         A12
#define A_TEMP2         A11
#define A_TEMP3         A10

//----------------------------------------------------------
//--- VALVES
//----------------------------------------------------------
#define VALVE0          7
#define VALVE1          6
#define VALVE2          14
#define VALVE3          15
#define VALVE4          16
#define VALVE5          17
#define VALVE6          18
#define VALVE7          19

//----------------------------------------------------------
//--- OTHERS
//----------------------------------------------------------
/* Input analog auxiliary lines */
#define ANALOG_AUX0     A7
#define ANALOG_AUX1     A6

/* Selector for on-board configuration or level value */
#define CFG_SELECTOR    53

/* LED pin. Unuseful for other purposes */
#define LED_ARDUINO     13


#endif

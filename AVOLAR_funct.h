/******************************************************************
 ******************************************************************
  CIEMAT, Avolar Project, PCB rev2.5

  Internship: Guillermo, de Arana, Schoebel.

  03/02/2022
--------------------------------------------------------------------

  Header file with function prototypes.

*******************************************************************
*******************************************************************/



#ifndef AVOLAR_FUNCT_H  
#define AVOLAR_FUNCT_H
/******************************************************************
  readCal: reads the cable capacitance stored in EEPROM after calibration
  and returns said value to be substracted from measurement.
*******************************************************************/
float readCal(int calAdress, float cableCap);

/******************************************************************
  storeCal: stores the cable capacitance in EEPROM.
  This is performed with the function EEPROM.update() so that the EEPROM 
  memory is not cycled unless necessary (the amount of cycles are limited)
*******************************************************************/
float storeCal(int calAdress, float cableCap);

/******************************************************************
  display: displays a given float number X, the type of variable
  should be specified by passing the function it's flagDisp
    ·'C': Capacity in Farads from 0 to 99.5nF
    ·'L': Level percentage from 0 to 100
    ·'E': For Error
     'T': For Temperature from 0 to 999
*******************************************************************/
void display(float X, char flagDisp);

/******************************************************************
  cicleDisplay: Cicles through all possible numbers on display 
  starting ar start and waiting ms milliseconds to print the 
  next one, the step size is set to the minimum representable 
  step 0.01.
*******************************************************************/
void cicleDisplay(unsigned long ms, float start);

/******************************************************************
  readLevelCounts: Return the ADC counts corresponding to the 
  capacitance reading, used mainly for calibration purpouses
*******************************************************************/
unsigned int readLevelCounts();

/******************************************************************
  readLevel: Reads the capacitance value at the output and returns
  said value on Farads.
*******************************************************************/
float readLevel(float linearRegression[], bool dispRnC);

/******************************************************************
  readTempCounts: Return the ADC counts corresponding to the 
  resistance reading, used mainly for calibration purpouses
*******************************************************************/
unsigned int readTempCounts(bool rangeFlag);

/******************************************************************
  readTemp: Reads the resistance value on the PT100, it has 2 
  range of values for either cryogenic or ambient temperatures.
*******************************************************************/
float readTemp(float linearRegression[], bool dispRnC);

/******************************************************************
  calibrateADC: Calibrates the resistance and capacitance values read by 
  the ADC by connecting 2 external components of known value.

  To calibrate the resistance set SPR cfg switch from U37 on the PCB to LOW? HIGH? and
  reset the arduino, then set a 22 ohm resistor on the Temp input, set 
  the frontal switch to MIN and press CAL, now place a 100 Ohm resistor,
  set it to MAX and press CAL.
  
  To calibrate the capacity do the same on the level input but for ? and ? capacitors.
*******************************************************************/
void calibrateADC(bool calRnC_crio, bool calRnC_amb, bool calTnL_crio, bool calTnL_amb, int i);

/******************************************************************
  EEPROMWriteFloat: Writes a float into floatAdress, since EEPROM 
  memory is only 1 byte long per adress the next available adress 
  to write will be floatAdress + 4;  
*******************************************************************/
void EEPROMWriteFloat(int floatAdress, float Float);

/******************************************************************
  EEPROMReadFloat: Returns a float stored into the EEPROM adress
  floatAdres, since EEPROM memory is only 1 byte long per adress 
  the next available adress to read will be floatAdress + 4;  
*******************************************************************/
float EEPROMReadFloat(int floatAdress);


//float* linRegFun(float Y2, float Y1, float X2, float X1)

#endif

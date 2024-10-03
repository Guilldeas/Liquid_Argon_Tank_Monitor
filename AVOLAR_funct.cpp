/******************************************************************
*******************************************************************
  CIEMAT, Avolar Project, PCB rev2.5

  Internship: Guillermo, de Arana, Schoebel.

  03/02/2022
--------------------------------------------------------------------

  Implementation of functions called in main code.
  
*******************************************************************
*******************************************************************/
#include <SoftwareSerial.h>
#include <math.h>
#include <EEPROM.h>
#include<LedControl.h>
#include "pins_and_var.h"
#include "AVOLAR_funct.h"

/*
float readCal(int calAdress, float calValue)
{
  // Call Read Cap Function
  cableCap = 0.0;
  EEPROM.get(calAdress, calValue);
  return calValue;
}
*/
/*****************************************************************/
/*
void storeCal(int calAdress, float calValue)
{
  // Read capacity functions
  EEPROM.put(calAdress, calValue);
  return;
}
*/
/*****************************************************************/

LedControl lc1 = LedControl(DISP_DIN, DISP_CLK, DISP_ENA, 1);
void display(float X, char flagDisp)
{
  int decPlace = 0;
  int decPlaceArr[3];
  int x;
  int edgeCase = 0;
  int endCharFlag = 0;
  char endChar = 'o';
  float Xbuff = 0.0;

  // Capacitance is represented in pF
  if (flagDisp == 'C')
    X = X * (float)pow(10, 12);

  // Error messages are treated differently
  if (flagDisp == 'E')
    endCharFlag = 1;
    endChar = 'E';
    
  // Check if value makes sense
  if ((flagDisp == 'C' && (X < 0.01 || X > 99500))|| (flagDisp == 'T' && (X > 400 || X < 0)) || (flagDisp == 'R' && (X > 170 || X < 0)))
  {
    lc1.setChar(0, 0, 'o', false);
    lc1.setChar(0, 1, 'F', false);
    lc1.setChar(0, 2, 'L', false);
    return;
  }

  // Beyond range cases are displayed as either 0 or 100 when in Level mode.
  if (X > 100 && flagDisp == 'L')
  {
    X = 100;
  }
  if (X < 0 && flagDisp == 'L')
  {
    X = 0;
  }

  // The code is now divided in 4 cases for each possible display output.
  // Only X digits will be printed
  // Printing XXYY.
  if(X >= 999.50 && flagDisp == 'C') 
  {
    X /= 1000;  // pF to nF
    endCharFlag = 1;
    endChar = 'n';
  }
  
  // Printing XXX. digits
  if (X >= 99.95 && X <999.5)
  {
    // The input is rounded depending on which digit needs to be rounded
    // To do so we check whethter the most significant non displayed digit is above or equal to 0.5
    if (X - (float)((int)X) >= 0.5)
    {
      X += 1;
    }
    
    x = (int)X;
    decPlace = 0;
  }

  // Printing XX.X digits
  if (X >= 9.995 && X < 99.95)
  {
    // The most significant non displayed digit is different with each case
    Xbuff = 10 * X;

    // Taking care of edge cases printing A instead of 10;
    if (X >= 9.995 && X <= 10)
    {
      X = 10;
      edgeCase = 1;
    }
    // Rounding for every other case.
    if ((Xbuff - (float)((int)Xbuff) >= 0.5) && (edgeCase == 0))
    {
      X += 0.1;
    }
    
    x = (int)(X * 10);
    decPlace = 1;
  }

  // Printing X.XX digits
  if ((X >= 0.0) && (X < 9.995))
  {
    Xbuff = 100 * X;
    
    if(Xbuff - (float)((int)Xbuff) >= 0.5)
    {
      X +=0.01;
    }
    
    x = (int)(X * 100);
    decPlace = 2;
  }

  // We set the decimal point argument array depending on the amount of iterations
  if (decPlace == 0)
  {
    decPlaceArr[0] = false;
    decPlaceArr[1] = false;
    decPlaceArr[2] = false;
  }

  if (decPlace == 1)
  {
    decPlaceArr[0] = decPlaceArr[2] = 0;
    decPlaceArr[1] = 1;
  }

  if (decPlace == 2)
  {
    decPlaceArr[1] = decPlaceArr[0] = 0;
    decPlaceArr[2] = 1;
  }

  int ones;
  int tens;
  int hundreds;

  ones = x % 10;
  x = x / 10;
  tens = x % 10;
  x = x / 10;
  hundreds = x;

  //Now print the number digit by digit
  if (endCharFlag == 0)
  {
    lc1.setChar(0, 0, (byte)hundreds, decPlaceArr[2]);
    lc1.setChar(0, 1, (byte)tens, decPlaceArr[1]);
    lc1.setChar(0, 2, (byte)ones, decPlaceArr[0]);
  }
  if (endCharFlag == 1)
  {
    lc1.setChar(0, 0, (byte)hundreds, decPlaceArr[2]);
    lc1.setChar(0, 1, (byte)tens, decPlaceArr[1]);
    lc1.setChar(0, 2, endChar, decPlaceArr[0]);
  }
}

/*****************************************************************/

void cicleDisplay(unsigned long ms, float start)
{
  float f = start;
  while(1)
  {
    display(f*pow(10, -12), 'C');
    f += 0.01;
    delay(ms);
  }
}


/*****************************************************************/
unsigned int readLevelCounts() 
{
  bool levelBits [16];
  unsigned int level = 0;
  levelBits[0] = digitalRead(LEVEL0);    
  levelBits[1] = digitalRead(LEVEL1);    
  levelBits[2] = digitalRead(LEVEL2);    
  levelBits[3] = digitalRead(LEVEL3);    
  levelBits[4] = digitalRead(LEVEL_GASBRD0);
  levelBits[5] = digitalRead(LEVEL_GASBRD1);
  levelBits[6] = digitalRead(LEVEL_GASBRD2);
  levelBits[7] = digitalRead(LEVEL_GASBRD3);
  levelBits[8] = digitalRead(LEVEL_SPARCFG);  // ¿NO HAY PROBLEMA PORQUE ESTÉ ENCENDIDO?
  levelBits[9] = digitalRead(LEVEL_ID0);
  levelBits[10] = digitalRead(LEVEL_ID1);
  levelBits[11] = digitalRead(LEVEL_ID2);
  levelBits[12] = digitalRead(LEVEL_ID3);
  levelBits[13] = digitalRead(LEVEL_ID4);
  levelBits[14] = digitalRead(LEVEL_ID5);
  levelBits[15] = digitalRead(LEVEL_ID6);

  // Every bit should be writen into an unsigned int.
  unsigned int sum [16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
  for (int i=0; i<16; i++)
  {
    if (levelBits[i] == HIGH)
    {
      level = level | sum[i];
    }
  }
  //memcpy(&level, &levelBits, sizeof(unsigned int));
  //Serial.print("Level: ");
  //Serial.println(level);

  return level;
  
}
/***************************************************************/
float readLevel(float linearRegression[], bool dispRnC)
{
  bool levelBits [16];
  unsigned int level = 0;

  level = 0;
  levelBits[0] = digitalRead(LEVEL0);    
  levelBits[1] = digitalRead(LEVEL1);    
  levelBits[2] = digitalRead(LEVEL2);    
  levelBits[3] = digitalRead(LEVEL3);    
  levelBits[4] = digitalRead(LEVEL_GASBRD0);
  levelBits[5] = digitalRead(LEVEL_GASBRD1);
  levelBits[6] = digitalRead(LEVEL_GASBRD2);
  levelBits[7] = digitalRead(LEVEL_GASBRD3);
  levelBits[8] = digitalRead(LEVEL_SPARCFG);
  levelBits[9] = digitalRead(LEVEL_ID0);
  levelBits[10] = digitalRead(LEVEL_ID1);
  levelBits[11] = digitalRead(LEVEL_ID2);
  levelBits[12] = digitalRead(LEVEL_ID3);
  levelBits[13] = digitalRead(LEVEL_ID4);
  levelBits[14] = digitalRead(LEVEL_ID5);
  levelBits[15] = digitalRead(LEVEL_ID6);

  // Every bit should be writen into an int.
  unsigned int sum [16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
  for (int i=0; i<16; i++)
  {
    if (levelBits[i] == HIGH)
    {
      level = level | sum[i];
    }
  }
  /*Serial.print("ADC_counts level = ");
  Serial.println(level);*/
  // Linear regresion is performed with diferent values depending on whether we want to display T or R.
  // Those values are stored 6 positions later on the linear regression array.
  int i = 0;
  if (!dispRnC)
  {
    i = 6;
  }

  float C = ((float)level) * linearRegression[4+i] + linearRegression[5+i];
/*
  Serial.print("C = ");
  Serial.println(C);
  Serial.print("Level = ");
  Serial.println(level);*/
  //return (float)Cap;
  return C;
}

/*****************************************************************/
unsigned int readTempCounts(bool rangeFlag)
{
  int T = 0;

  // Depending on the range we want to calibrate we choose a different chanel.
  if (!rangeFlag)
  {
    T = analogRead(TEMPERAT0);    // Scaled through hardware for cryogenic temperatures.
  }
  if (rangeFlag)
  {
    T = analogRead(TEMPERAT1);    // Scaled through hardware for ambient temperatures.
  }
  
  return (unsigned int)T;
}
/*****************************************************************/

float readTemp(float linearRegression[], bool dispRnC)
{
  float T = 0.0;
  bool ambRange = false;
  bool crioRange = true;
  int chanel = TEMPERAT0;
  if (analogRead(TEMPERAT0)>=1023)  // Change chanel if we are out of range
  {
    chanel = TEMPERAT1;
    ambRange = true;
    crioRange = false;
  }

  T = (float)analogRead(chanel);    // Scaled through hardware for cryogenic temperatures.

  // Linear regresion is performed with diferent values depending on whether we want to display T or R.
  // Those values are stored 6 positions later on the linear regression array.
  int i = 0;
  if (!dispRnC)
  {
    i = 6;
  }
  if (ambRange) 
  {
    T = T * linearRegression[2+i] + linearRegression[3+i];  
  }
  if (crioRange)
  {
    T = T * linearRegression[0+i] + linearRegression[1+i];  
  }
  
  return T;
}

/*****************************************************************/
void calibrateADC(bool calRnC_crio, bool calRnC_amb, bool calTnL_crio, bool calTnL_amb, int i)
{  
  // We interpret the range of T or R we want to measure. rangeFlag is true for ambient range and false for cryogenic.
  bool rangeFlag = (!calRnC_crio || !calTnL_crio) && (calRnC_amb || calTnL_amb);

  // Switches
  int Swt_level = !digitalRead(SWT_LEVL);
  int Swt_temp = !digitalRead(SWT_TEMP);
  int Swt_cal = !digitalRead(SWT_CAL);
  int Swt_cal_max = !digitalRead(SWT_CAL_MAX);
  int Swt_cal_min = !digitalRead(SWT_CAL_MIN);


  /* CAMBIAR
   *    - STORING ADRESS                        check
   *    - FLAGS DE ENTRADA A CALIBRACION        check
   *    - DISPLAYS por pantalla, serial.prints() y comentarios
   *    - QUITAR CALADCVALUESBUFF                                   check
   */

  /* FIRST CALIBRATION
   * WRITE ON EEPROM ADC COUNTS FOR RESISTANCE AND CAPACITY 
   */
   
  // Capture ADC counts for resistance on the cryogenic and ambient temperature range and store them.
  if (Swt_temp && (calRnC_crio || calRnC_amb))
  {
    digitalWrite(LED_TEMP, HIGH);
    digitalWrite(LED_LEVEL, LOW);


    // Capture ADC_R_min for the cryogenic temperature range and store it in adress 0.
    if(Swt_cal_min && calRnC_crio)  
    {
      int dataRecorded = 0;

      // Display the value we aim to calibrate to the user
      display(22.0, 'R');
   
      while(Swt_cal)
      {
        Swt_cal = !digitalRead(SWT_CAL);
        dataRecorded = 1;
      }
      // Write once and only after releasing SWT_CAL (the EEPROM has a limited set of overwritting cycles).
      if(dataRecorded)
      {
        EEPROMWriteFloat(0, (float)readTempCounts(rangeFlag));
        Serial.print("ADC_R_min_crio = ");
        Serial.println((float)readTempCounts(rangeFlag));
      }
    }


    // Capture ADC_R_max for the cryogenic temperature range and store it in adress 1.
    if(Swt_cal_max && calRnC_crio)  
    {
      int dataRecorded = 0;
      if (calRnC_crio)
      {
        display(33.0, 'R');
      }
      if (calTnL_crio)
      {
        display(98.97, 'R');
      }
      
      while(Swt_cal)
      {
        Swt_cal = !digitalRead(SWT_CAL);
        dataRecorded = 1;
      }
      
      if(dataRecorded)
      {
        EEPROMWriteFloat(1, (float)readTempCounts(rangeFlag));
        Serial.print("ADC_R_max_crio");
        Serial.println((float)readTempCounts(rangeFlag));
      }
    }



    // Capture ADC_R_min for the ambient temperature range and store it in adress 2.
    if(Swt_cal_min && calRnC_amb)
    {
      int dataRecorded = 0;

      display(33.0, 'R');

      while(Swt_cal)
      {
        Swt_cal = !digitalRead(SWT_CAL);
        dataRecorded = 1;
      }
      
      if(dataRecorded)
      {
        EEPROMWriteFloat(2, (float)readTempCounts(rangeFlag));
        Serial.print("ADC_R_min_amb = ");
        Serial.println((float)readTempCounts(rangeFlag));
      }
    }



    // Capture ADC_R_max for the ambient temperature range and store it in adress 3.
    if(Swt_cal_max && calRnC_amb)
    {
      int dataRecorded = 0;

      display(100.0, 'T');

      while(Swt_cal)
      {
        Swt_cal = !digitalRead(SWT_CAL);
        dataRecorded = 1;
      }
      
      if(dataRecorded)
      {
        EEPROMWriteFloat(3, (float)readTempCounts(rangeFlag));
        Serial.print("ADC_R_max_amb");
        Serial.println((float)readTempCounts(rangeFlag));
      }
    }
  }


  
  // WRITE ON EEPROM ADC COUNTS FOR CAPACITY  
  if (Swt_level && (calRnC_crio || calRnC_amb))
  {
    digitalWrite(LED_TEMP, LOW);
    //digitalWrite(LED_LEVEL, HIGH);



    // Capture ADC_C_min for any temperature range and store it in adress 4.
    if(Swt_cal_min)
    {
      int dataRecorded = 0;
      display(97e-12, 'C');
      
      while(Swt_cal)
      {
        Swt_cal = !digitalRead(SWT_CAL);
        dataRecorded = 1;
      }
      
      if(dataRecorded)
      {
        EEPROMWriteFloat(4, (float)readLevelCounts());
        Serial.print("ADC_C_min");
        Serial.println((float)readLevelCounts());
      }
    }



    // Capture ADC_C_max for any temperature range and store it in adress 5.
    if(Swt_cal_max)
    {
      int dataRecorded = 0;
      display(1.5e-9, 'C');
      
      while(Swt_cal)
      {
        Swt_cal = !digitalRead(SWT_CAL);
        dataRecorded = 1;
      }
      
      if(dataRecorded)
      {
        EEPROMWriteFloat(5, (float)readLevelCounts());
        Serial.print("ADC_C_max");
        Serial.println((float)readLevelCounts());
      }
    }
  }


  /* SECOND CALIBRATION
   *  
   * WRITE ON EEPROM ADC COUNTS FOR CAPACITY AND RESISTANCE
   */
  if(Swt_temp && (calTnL_crio || calTnL_amb))
  {
    // Capture ADC_T_min for the cryogenic temperature range and store it in adress 6.     
      if(Swt_cal_min && calTnL_crio)  
      {
        int dataRecorded = 0;
  
        display(70.703, 'T');
          
        while(Swt_cal)
        {
          Swt_cal = !digitalRead(SWT_CAL);
          dataRecorded = 1;
        }
        // Write once and only after releasing SWT_CAL (the EEPROM has a limited set of overwritting cycles).
        if(dataRecorded)
        {
          EEPROMWriteFloat(6, (float)readTempCounts(rangeFlag));
          Serial.print("ADC_T_min_crio = ");
          Serial.println((float)readTempCounts(rangeFlag));
        }
      }
  
  
      // Capture ADC_T_max for the cryogenic temperature range and store it in adress 7.
      if(Swt_cal_max && calTnL_crio)  
      {
        int dataRecorded = 0;
        
        display(98.97, 'T');
        
        while(Swt_cal)
        {
          Swt_cal = !digitalRead(SWT_CAL);
          dataRecorded = 1;
        }
        
        if(dataRecorded)
        {
          EEPROMWriteFloat(7, (float)readTempCounts(rangeFlag));
          Serial.print("ADC_T_max_crio = ");
          Serial.println((float)readTempCounts(rangeFlag));
        }
      }



    // Capture ADC_T_min for the ambient temperature range and store it in adress 8.
    if(Swt_cal_min && calTnL_amb)
    {
      int dataRecorded = 0;

      display(98.97, 'T');
      
      while(Swt_cal)
      {
        Swt_cal = !digitalRead(SWT_CAL);
        dataRecorded = 1;
      }
      
      if(dataRecorded)
      {
        EEPROMWriteFloat(8, (float)readTempCounts(rangeFlag));
        Serial.print("ADC_T_min_amb = ");
        Serial.println((float)readTempCounts(rangeFlag));
      }
    }
  }



    // Capture ADC_T_max for the ambient temperature range and store it in adress 9.
    if(Swt_cal_max && calTnL_amb)
    {
      int dataRecorded = 0;

      display(273, 'T');
      
      while(Swt_cal)
      {
        Swt_cal = !digitalRead(SWT_CAL);
        dataRecorded = 1;
      }
      
      if(dataRecorded)
      {
        EEPROMWriteFloat(9, (float)readTempCounts(rangeFlag));
        Serial.print("ADC_T_max_amb = ");
        Serial.println((float)readTempCounts(rangeFlag));
      }
    }



  
  // WRITE ON EEPROM ADC COUNTS FOR LEVEL 
  if (Swt_level && (calTnL_crio || calTnL_amb))
  {
    digitalWrite(LED_TEMP, LOW);
    digitalWrite(LED_LEVEL, HIGH);



    // Capture ADC_L_min for any temperature range and store it in adress 10.
    if(Swt_cal_min)
    {
      int dataRecorded = 0;

      display(0, 'L');
      
      while(Swt_cal)
      {
        Swt_cal = !digitalRead(SWT_CAL);
        dataRecorded = 1;
      }
      
      if(dataRecorded)
      {
        EEPROMWriteFloat(10, (float)readLevelCounts());
        Serial.print("ADC_L_min = ");
        Serial.println((float)readLevelCounts());
      }
    }



    // Capture ADC_C/L_max for any temperature range and store it in adress 11.
    if(Swt_cal_max)
    {
      int dataRecorded = 0;
      
      display(100, 'L');

      while(Swt_cal)
      {
        Swt_cal = !digitalRead(SWT_CAL);
        dataRecorded = 1;
      }
      
      if(dataRecorded)
      {
        EEPROMWriteFloat(11, (float)readLevelCounts());
        Serial.print("ADC_L_max");
        Serial.println((float)readLevelCounts());
      }
    }
  }

}

/*****************************************************************/
void EEPROMWriteFloat(int floatAdress, float storedFloat)
{  
  byte Bytes[4];
  
  memcpy(Bytes, &storedFloat, sizeof(float));

  for (int d=0; d<4; d++)
  {
    EEPROM.write(4*floatAdress+d, Bytes[d]);
  }
  return 0;
}

/*****************************************************************/
float EEPROMReadFloat(int floatAdress)
{
  float valBuff = 0.0;
  byte Bytes[4];

  for (int d=0; d<4; d++)
  {
    Bytes[d] = EEPROM.read(4*floatAdress+d);
  }

  memcpy(&valBuff, Bytes, sizeof(float));
  
  return valBuff;
}

/*****************************************************************/
/*
float* linRegFun(float Y2, float Y1, float X2, float X1)
{
  float arr[4];
  return arr;
}
*/

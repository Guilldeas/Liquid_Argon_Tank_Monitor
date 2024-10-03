/******************************************************************
*******************************************************************
  CIEMAT, Avolar Project, PCB rev2.5

  Internship: Guillermo, de Arana, Schoebel.

  03/02/2022
  --------------------------------------------------------------------

  This code aims to reprodue certain functions of the control code
  for the original Avolar control PCB.

*******************************************************************
*******************************************************************/
// GUARDAR UN VALOR DE ADC PARA CALIBRACION DE NIVEL Y OTRO DISTINTO PARA CALIBRACION DE CAPACIDAD!!!
// Si no hay que rehacer las calibraciones cada vez que quieres cambiar de "unidades" en el display

// Que se cambien los datos de display si se presiona cal en loop() en lugar de resetando

/////////////////////////PROXIMO DIA:
/*
 * 20 ohmios a 77Kelvin (cuando esta en el Nitrogeno) no deberia dar, hay un problema de medida de resistencias, comprobar con polimetro las resistencias
 * 
 */


#include <SoftwareSerial.h>
#include <EEPROM.h>
#include<LedControl.h>
#include "pins_and_var.h"
#include "AVOLAR_funct.h"

//PROBLEMA, ALMACENAR TAMBIEN 12 VALORES PARA CALADCVALUES
float calADCValues[12];
float linearRegression[12];


// Code flags
bool  calRnC_crio = false;    // Flag is raised when user wants to calibrate R for criogenic temperatures or C. Switches: [000]
bool  calRnC_amb = false;     // Flag is raised when user wants to calibrate R for ambient temperatures or C. Switches: [001]
bool  calTnL_crio = false;    // Flag is raised when user wants to calibrate T for criogenic temperatures or L. Switches: [010]
bool  calTnL_amb = false;     // Flag is raised when user wants to calibrate T for ambient temperatures or L. Switches: [011]
bool  dispRnC = false;        // Flag is true when user wants to display R and C values: Switch CAL is HIGH. A false flag means display T and L: Switch CAL is LOW 
//bool  dispTnL = false;        // Flag is raised when user wants to display T and L values. Switches: [101]


void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("INICIO ");

  // Setup 7 segment display
  LedControl lc1 = LedControl(DISP_DIN, DISP_CLK, DISP_ENA, 1);
  lc1.shutdown(0, false);
  lc1.setIntensity(0, 8);
  void clearDisplay(int addr);

  // LED Pins
  pinMode(TEST_LED, OUTPUT);
  pinMode(LED_TEMP, OUTPUT);
  pinMode(LED_LEVEL, OUTPUT);
  pinMode(LED_MAX_DEP1, OUTPUT);
  pinMode(LED_ERROR, OUTPUT);
  pinMode(LED_LOW_DEP1, OUTPUT);
  pinMode(LED_MID_DEP1, OUTPUT);
  pinMode(LED_SPARE, OUTPUT);
  pinMode(LED_LOW_DEP3, OUTPUT);

  // Switch Pins
  pinMode(SWT_LEVL, INPUT);
  pinMode(SWT_TEMP, INPUT);
  pinMode(SWT_CAL_MAX, INPUT);
  pinMode(SWT_CAL_MIN, INPUT);
  pinMode(SWT_LIM_PR_LV, INPUT);
  pinMode(SWT_LIM, INPUT);
  pinMode(SWT_CTRL_STOP, INPUT);
  pinMode(TEST_LED, OUTPUT);
  pinMode(SEL_CFG_LEVL, OUTPUT);


  // Set SEL_CFG_LEVL LOW to check if the user wants to perform calibrations or change the data displayed.
  digitalWrite(SEL_CFG_LEVL, LOW);

  calRnC_crio = ((!digitalRead(LEVEL_ID3)) == LOW) && ((!digitalRead(LEVEL_SPARCFG)) == LOW) && ((!digitalRead(LEVEL_GASBRD0)) == LOW);
  calRnC_amb = ((!digitalRead(LEVEL_ID3)) == LOW) && ((!digitalRead(LEVEL_SPARCFG)) == LOW) && ((!digitalRead(LEVEL_GASBRD0)) == HIGH);
  calTnL_crio = ((!digitalRead(LEVEL_ID3)) == LOW) && ((!digitalRead(LEVEL_SPARCFG)) == HIGH) && ((!digitalRead(LEVEL_GASBRD0)) == LOW);
  calTnL_amb = ((!digitalRead(LEVEL_ID3)) == LOW) && ((!digitalRead(LEVEL_SPARCFG)) == HIGH) && ((!digitalRead(LEVEL_GASBRD0)) == HIGH);
  //dispRnC = ((!digitalRead(LEVEL_ID3)) == HIGH) && ((!digitalRead(LEVEL_SPARCFG)) == LOW) && ((!digitalRead(LEVEL_GASBRD0)) == LOW);
  //dispTnL = ((!digitalRead(LEVEL_ID3)) == HIGH) && ((!digitalRead(LEVEL_SPARCFG)) == LOW) && ((!digitalRead(LEVEL_GASBRD0)) == HIGH);


  // Set HIGH to read Input_0 values (i.e. level bits).
  digitalWrite(SEL_CFG_LEVL, HIGH);


  // CALIBRATE
  //calADCValues --> {ADC_R/T_min_crio, ADC_R/T_max_crio, ADC_R/T_min_amb, ADC_R/T_max_amb, ADC_Cmin, ADC_Cmax}
  if (calRnC_crio || calRnC_amb || calTnL_crio || calTnL_amb)
  {
    // Execute calibration until the switches are set to a display mode and RESET is pressed.
    Serial.println("CALIBRATION ");
    while (1)
    {
      for (int i = 0; i < 4; i++)
      {
        calibrateADC(calRnC_crio, calRnC_amb, calTnL_crio, calTnL_amb, i);
      }
    }
  }


  // Always read calibration values at setup
  digitalWrite(SEL_CFG_LEVL, HIGH);
  Serial.println("NO CALIBRATION ");

  for (int d = 0; d < 12; d++)
  {
    calADCValues[d] = EEPROMReadFloat(d);
  }

  Serial.print("ADC_R_min for the cryogenic temperature range  = ");
  Serial.println(calADCValues[0]);
  Serial.print("ADC_R_max for the cryogenic temperature range = ");
  Serial.println(calADCValues[1]);
  Serial.print("ADC_R_min for the ambient temperature range  = ");
  Serial.println(calADCValues[2]);
  Serial.print("ADC_R_max for the ambient temperature range = ");
  Serial.println(calADCValues[3]);
  Serial.print("ADC_C_min for any temperature range = ");
  Serial.println(calADCValues[4]);
  Serial.print("ADC_C_max for any temperature range = ");
  Serial.println(calADCValues[5]);
  Serial.print("ADC_T_min for the cryogenic temperature range  = ");
  Serial.println(calADCValues[6]);
  Serial.print("ADC_T_max for the cryogenic temperature range = ");
  Serial.println(calADCValues[7]);
  Serial.print("ADC_T_min for the ambient temperature range  = ");
  Serial.println(calADCValues[8]);
  Serial.print("ADC_T_max for the ambient temperature range = ");
  Serial.println(calADCValues[9]);
  Serial.print("ADC_L_min for any temperature range = ");
  Serial.println(calADCValues[10]);
  Serial.print("ADC_L_max for any temperature range = ");
  Serial.println(calADCValues[11]);
  Serial.println(" ");


  // Find a linear regresion for R and C or T and L (refered to as Y1 and Y2 respectively) according to the calibration data
  // {Y1_slope, Y1_intercept, Y2_slope, Y2_intercept}
  float C_max = 1.5e-9;
  float C_min = 97e-12;
  float L_max = 100;
  float L_min = 0;

  float R_max_amb = 100;
  float R_min_amb = 33;
  float R_max_crio = 33;
  float R_min_crio = 22.4;
  float T_max_amb = 273;
  float T_min_amb = 98.97;
  float T_max_crio = 98.97;
  float T_min_crio = 77;

  float Y1_max = 0;
  float Y1_min = 0;
  float Y2_max = 0;
  float Y2_min = 0;

  ///////////////////////////////// REVISAR A PARTIR DE AQUI: REGRESION LINEAL /////////////////////////////////////////////

  //calADCValues --> {ADC_R/T_min_crio, ADC_R/T_max_crio, ADC_R/T_min_amb, ADC_R/T_max_amb, ADC_C/L_min, ADC_C/L_max}

  // Calculate a linear regression for R_crio
  float Y1_slope = (R_max_crio - R_min_crio) / (calADCValues[1] - calADCValues[0]);
  float Y1_intercept = R_max_crio - calADCValues[1] * Y1_slope;

  // Calculate a linear regression for R_amb
  float Y2_slope = (R_max_amb - R_min_amb) / (calADCValues[3] - calADCValues[2]);
  float Y2_intercept = R_max_amb - calADCValues[3] * Y2_slope;

  // Calculate a linear regression for C
  float Y3_slope = (C_max - C_min) / (calADCValues[5] - calADCValues[4]);
  float Y3_intercept = C_max - calADCValues[5] * Y3_slope;

  linearRegression[0] = Y1_slope;
  linearRegression[1] = Y1_intercept;
  linearRegression[2] = Y2_slope;
  linearRegression[3] = Y2_intercept;
  linearRegression[4] = Y3_slope;
  linearRegression[5] = Y3_intercept;

  Serial.print("Y1_slope = ");
  Serial.println(Y1_slope);
  Serial.print("Y1_intercept = ");
  Serial.println(Y1_intercept);
  Serial.print("Y2_slope = ");
  Serial.println(Y2_slope);
  Serial.print("Y2_intercept = ");
  Serial.println(Y2_intercept);
  Serial.print("Y3_slope = ");
  Serial.println(Y3_slope, 12);
  Serial.print("Y3_intercept = ");
  Serial.println(Y3_intercept);
  Serial.println(" ");




  // Calculate a linear regression for T_crio
  Y1_slope = (T_max_crio - T_min_crio) / (calADCValues[7] - calADCValues[6]);
  Y1_intercept = T_max_crio - calADCValues[7] * Y1_slope;

  // Calculate a linear regression for T_amb
  Y2_slope = (T_max_amb - T_min_amb) / (calADCValues[9] - calADCValues[8]);
  Y2_intercept = T_max_amb - calADCValues[9] * Y2_slope;

  // Calculate a linear regression for L
  Y3_slope = (L_max - L_min) / (calADCValues[11] - calADCValues[10]);
  Y3_intercept = L_max - calADCValues[11] * Y3_slope;


  linearRegression[6] = Y1_slope;
  linearRegression[7] = Y1_intercept;
  linearRegression[8] = Y2_slope;
  linearRegression[9] = Y2_intercept;
  linearRegression[10] = Y3_slope;
  linearRegression[11] = Y3_intercept;

  Serial.print("Y1_slope = ");
  Serial.println(Y1_slope);
  Serial.print("Y1_intercept = ");
  Serial.println(Y1_intercept);
  Serial.print("Y2_slope = ");
  Serial.println(Y2_slope);
  Serial.print("Y2_intercept = ");
  Serial.println(Y2_intercept);
  Serial.print("Y3_slope = ");
  Serial.println(Y3_slope, 12);
  Serial.print("Y3_intercept = ");
  Serial.println(Y3_intercept);
  Serial.println(" ");

  // Written here so that it doesnt flash for each loop
  digitalWrite(LED_LEVEL, LOW);
}



void loop()
{
  // Switches
  int Swt_level = !digitalRead(SWT_LEVL);
  int Swt_temp = !digitalRead(SWT_TEMP);
  int Swt_cal = !digitalRead(SWT_CAL);
  int Swt_cal_max = !digitalRead(SWT_CAL_MAX);
  int Swt_cal_min = !digitalRead(SWT_CAL_MIN);
  int Swt_lim_pr_lv = !digitalRead(SWT_LIM_PR_LV);
  int Swt_lim = !digitalRead(SWT_LIM);
  int Swt_ctrl_stop = !digitalRead(SWT_CTRL_STOP);


  // Reset LEDS each cycle.
  digitalWrite(TEST_LED, LOW);
  digitalWrite(LED_TEMP, LOW);
  digitalWrite(LED_MAX_DEP1, LOW);
  digitalWrite(LED_ERROR, LOW);
  digitalWrite(LED_LOW_DEP1, LOW);
  digitalWrite(LED_MID_DEP1, LOW);
  digitalWrite(LED_SPARE, LOW);
  digitalWrite(LED_LOW_DEP3, LOW);


  ///////////////////////////////////////////////POR HACER////////////////////////////////////////////////////////////
  //
  // PROBLEMA: SE HACE LA REGRESION LINEAL EN SETUP PARA UN CASO O EL OTRO
  //
  // LEER CORRECTAMENTE CADA REGRESION DEPENDIENDO DEL MODO DE DISPLAY DE DATOS
  // PROBLEMA EN CALCULO DE YSLOPE YINTERCEPT


  // Data display mode 
  dispRnC = false;
  if (Swt_cal == HIGH)
  {
    dispRnC = true;
  }
   
  // Temperature and Resistance
  if (Swt_temp == HIGH)
  {
    digitalWrite(LED_TEMP, HIGH);
    ////
    float Temp_acc = 0;
    float T = 0;
    int average = 1000;
    for (int i = 0; i < average; i++)
    {
      T += readTemp(linearRegression, dispRnC);
    }
    T /= average;
    
    if (dispRnC)
    {
      display(T, 'R');
    }
    if (!dispRnC)
    {
      display(T, 'T');
    }
  }

  // Level and Capacitance
  if (Swt_level == HIGH)
  {
    float Level_acc = 0;
    float Level = 0;
    int average = 1000;
    for (int i = 0; i < average; i++)
    {
      Level += readLevel(linearRegression, dispRnC);
    }
    Level /= average;
    
    if (dispRnC)
    {
      display(Level, 'C');
    }
    if (!dispRnC)
    {
      display(Level, 'L');
    }
  }

// Tell user if the canister is at liquification temperature for Argon.
if (readTemp(linearRegression, false) <= 87)
{
  digitalWrite(LED_LEVEL, HIGH);
}
if (readTemp(linearRegression, false) > 87)
{
  digitalWrite(LED_LEVEL, LOW);
}

}

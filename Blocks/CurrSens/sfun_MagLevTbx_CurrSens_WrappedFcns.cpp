/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 * 
 * February 16, 2026
 *
 * Dept. of Information Engineering, University of Padova 
 *
 */

#ifndef MATLAB_MEX_FILE

#include <Arduino.h>
#include <rtwtypes.h>

// Current sensor pins
// CURR_SOL3 (+Y) - PIN 20
// CURR_SOL4 (-X) - PIN 21
// CURR_SOL2 (+X) - PIN 22
// CURR_SOL1 (-Y) - PIN 23

#define CURRENT_Y_POS 20
#define CURRENT_X_NEG 21
#define CURRENT_X_POS 22
#define CURRENT_Y_NEG 23

const int NUM_SENSORS = 4;
const uint16_t sensorPin[NUM_SENSORS] = {
    CURRENT_X_POS,      //  (+X)
    CURRENT_X_NEG,      //  (-X)
    CURRENT_Y_POS,      //  (+Y)
    CURRENT_Y_NEG };    //  (-Y)

#endif

#ifdef __cplusplus
extern "C" {
#endif

void sfun_MagLevTbx_CurrSens_WrappedStart(uint8_T sensorId)
{
#ifndef MATLAB_MEX_FILE

    //  Initialize current sensor pin
    pinMode(sensorPin[sensorId], INPUT);

    // Set ADC resolution to 10 bits (0-1023)
    analogReadResolution(10);

#endif
}


void sfun_MagLevTbx_CurrSens_WrappedOutput(uint8_T sensorId, float *y0)
{
#ifndef MATLAB_MEX_FILE

  uint16_t data = analogRead(sensorPin[sensorId]);      //  Read ADC
  float voltage = (data*3.3)/1023.0;                    //  ADC to voltage
  float voltage_diff = voltage - 1.65;                  //  Centered around 1.65V (no current)
  *y0 = voltage_diff/(100.0*0.015);                     //  Gain = 100 (INA214), Rshunt = 0.015Ω

#endif   
}


void sfun_MagLevTbx_CurrSens_WrappedTerminate(void)
{
#ifndef MATLAB_MEX_FILE
      
#endif      
}


#ifdef __cplusplus
}
#endif
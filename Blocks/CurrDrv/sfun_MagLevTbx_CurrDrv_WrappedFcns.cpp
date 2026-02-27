/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 * 
 * February 16, 2026
 *
 * Dept. of Information Engineering, University of Padova 
 *
 */

# ifndef MATLAB_MEX_FILE
# include <Arduino.h>
//#include <math.h>
#include <rtwtypes.h>

// Motor driver pins
// HB_0A, HB_0B (-Y) - PIN 4, 5
// HB_1A, HB_1B (+X) - PIN 2, 3
// HB_2A, HB_2B (+Y) - PIN 8, 9
// HB_3A, HB_3B (-X) - PIN 6, 7

// Motor driver pins
#define HB_0A 4
#define HB_0B 5
#define HB_1A 2
#define HB_1B 3
#define HB_2A 8
#define HB_2B 9
#define HB_3A 6
#define HB_3B 7

const int NUM_DRIVERS = 4;
const uint16_t driverPinA[NUM_DRIVERS] = {
    HB_1A,      //  (+X) 
    HB_3A,      //  (-X)
    HB_2A,      //  (+Y)
    HB_0A };    //  (-Y)
const uint16_t driverPinB[NUM_DRIVERS] = {
    HB_1B,      //  (+X) 
    HB_3B,      //  (-X)
    HB_2B,      //  (+Y)
    HB_0B };    //  (-Y)

# endif


#ifdef __cplusplus
extern "C" {
#endif


void sfun_MagLevTbx_CurrDrv_WrappedStart(uint8_T driverId)
{
# ifndef MATLAB_MEX_FILE
    
    //  Initialize current driver pins
    pinMode(driverPinA[driverId], OUTPUT);
    pinMode(driverPinB[driverId], OUTPUT);

    //  Set PWM resolution to 8 bits (0-255)
    analogWriteResolution(8);

    //  Set PWM frequency
    analogWriteFrequency(driverPinA[driverId], 31250);
    analogWriteFrequency(driverPinB[driverId], 31250);

    //  Set initial state to 0
    analogWrite(driverPinA[driverId], 0);
    analogWrite(driverPinB[driverId], 0);

# endif
}


void sfun_MagLevTbx_CurrDrv_WrappedOutput(uint8_T driverId, int16_T *u0)
{
# ifndef MATLAB_MEX_FILE
    
    //  Set current driver PWM
    if (*u0 > 0) {
        analogWrite(driverPinA[driverId], 255 - abs(*u0));
        analogWrite(driverPinB[driverId], 255);
    } else if (*u0 < 0) {
        analogWrite(driverPinA[driverId], 255);
        analogWrite(driverPinB[driverId], 255 - abs(*u0));
    } else {
        analogWrite(driverPinA[driverId], 255);
        analogWrite(driverPinB[driverId], 255);
    }

# endif   
}


void sfun_MagLevTbx_CurrDrv_WrappedTerminate(uint8_T driverId)
{
# ifndef MATLAB_MEX_FILE
    
    //  Turn current driver off
    analogWrite(driverPinA[driverId], 0);
    analogWrite(driverPinB[driverId], 0);
    
# endif      
}


#ifdef __cplusplus
}
#endif
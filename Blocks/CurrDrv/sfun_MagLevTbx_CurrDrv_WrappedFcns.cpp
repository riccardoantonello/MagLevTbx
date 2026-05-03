/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 * 
 * April 28, 2026
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


//  Aux functions
static inline bool isValidDriverId(uint8_T driverId)
{
    return driverId < NUM_DRIVERS;
}

static inline void setDriverZeroCurrent(uint8_T driverId)
{
    analogWrite(driverPinA[driverId], 255);
    analogWrite(driverPinB[driverId], 255);
}

static inline void setDriverOff(uint8_T driverId)
{
    analogWrite(driverPinA[driverId], 0);
    analogWrite(driverPinB[driverId], 0);
}

# endif


#ifdef __cplusplus
extern "C" {
#endif


void sfun_MagLevTbx_CurrDrv_WrappedStart(uint8_T driverId)
{
# ifndef MATLAB_MEX_FILE
    
    //  Return if invalid driverId
    if (!isValidDriverId(driverId)) {
        return;
    }

    //  Initialize current driver pins
    pinMode(driverPinA[driverId], OUTPUT);
    pinMode(driverPinB[driverId], OUTPUT);

    //  Set PWM resolution to 8 bits (0-255)
    analogWriteResolution(8);

    //  Set PWM frequency
    analogWriteFrequency(driverPinA[driverId], 31250);
    analogWriteFrequency(driverPinB[driverId], 31250);

    //  Set initial state to zero current
    setDriverZeroCurrent(driverId);

# endif
}


void sfun_MagLevTbx_CurrDrv_WrappedEnable(uint8_T driverId)
{
    # ifndef MATLAB_MEX_FILE

    //  Return if invalid driverId
    if (!isValidDriverId(driverId)) {
        return;
    }

    //  Re-enable current driver in a safe zero-current state.
    //  The next Outputs() call will apply the requested PWM command.
    setDriverZeroCurrent(driverId);

    # endif
}


void sfun_MagLevTbx_CurrDrv_WrappedOutput(uint8_T driverId, int16_T *u0)
{
# ifndef MATLAB_MEX_FILE
    
    int16_T pwmCmd = *u0;

    //  Return if invalid driverId
    if (!isValidDriverId(driverId)) {
        return;
    }

    //  Saturate command to the 8-bit PWM range expected by analogWriteResolution(8).
    if (pwmCmd > 255) {
        pwmCmd = 255;
    } else if (pwmCmd < -255) {
        pwmCmd = -255;
    }

    //  Set current driver PWM
    if (pwmCmd > 0) {
        analogWrite(driverPinA[driverId], 255 - pwmCmd);
        analogWrite(driverPinB[driverId], 255);
    } else if (pwmCmd < 0) {
        analogWrite(driverPinA[driverId], 255);
        analogWrite(driverPinB[driverId], 255 + pwmCmd);
    } else {
        setDriverZeroCurrent(driverId);
        //setDriverOff(driverId);
    }

# endif   
}


void sfun_MagLevTbx_CurrDrv_WrappedDisable(uint8_T driverId)
{
# ifndef MATLAB_MEX_FILE

    if (!isValidDriverId(driverId)) {
        return;
    }

    //  Disable current driver outputs
    setDriverOff(driverId);

# endif
}


void sfun_MagLevTbx_CurrDrv_WrappedTerminate(uint8_T driverId)
{
# ifndef MATLAB_MEX_FILE

    //  Return if invalid driverId
    if (!isValidDriverId(driverId)) {
        return;
    }
    
    //  Turn current driver off
    setDriverOff(driverId);
    
# endif      
}


#ifdef __cplusplus
}
#endif
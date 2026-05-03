/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 * 
 * May 02, 2026
 *
 * Dept. of Information Engineering, University of Padova 
 *
 */

#ifndef MATLAB_MEX_FILE

/*  Includes ------------------------------------------------------------*/
#include <Arduino.h>
#include "Wire.h"
#include <math.h>
#include <rtwtypes.h>

#include <TLV493D.h>


/*  Defines -------------------------------------------------------------*/

/*  I2C clock frequency [Hz]    */
const uint32_t I2C_CLOCK_HZ = 400000; 


/*  Private variables ---------------------------------------------------*/
static bool magInitialized = false;
static double lastGood[3] = {0.0};


/*  Private functions ---------------------------------------------------*/

/*  Non-fatal recovery used only after a failed transaction */
void i2cRecover(void)
{
    Wire.end();
    delayMicroseconds(200);
    Wire.begin();
    Wire.setClock(I2C_CLOCK_HZ);
#ifdef WIRE_HAS_TIMEOUT
    Wire.setWireTimeout(3000, true);
#endif
}


#endif

/* Callback functions definitions ---------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


void sfun_MagLevTbx_MagSens_WrappedStart(void)
{
#ifndef MATLAB_MEX_FILE

    //  Initialize I2C communication
    Wire.begin();
    Wire.setClock(I2C_CLOCK_HZ);

    //  Set timeout to prevent permanent lock
    #ifdef WIRE_HAS_TIMEOUT
    Wire.setWireTimeout(3000, true);
    #endif
    delay(50);

    //  Init state variables
    magInitialized = false;
    lastGood[0] = 0.0;
    lastGood[1] = 0.0;
    lastGood[2] = 0.0;

    //  Init the mag sensor
    magInitialized = TLV493D_InitSensor();

#endif
}


void sfun_MagLevTbx_MagSens_WrappedOutput(double *y0)
{
#ifndef MATLAB_MEX_FILE

    //  If initialization failed or the sensor was reset, try to configure it again
    if (!magInitialized) {
        magInitialized = TLV493D_InitSensor();
        if (!magInitialized) {
            y0[0] = lastGood[0];
            y0[1] = lastGood[1];
            y0[2] = lastGood[2];
            return;
        }
    }

    //  Read data
    double bx, by, bz;
    if (!TLV493D_GetMagField_mT(&bx, &by, &bz)) {
        magInitialized = false;
        y0[0] = lastGood[0];
        y0[1] = lastGood[1];
        y0[2] = lastGood[2];
        i2cRecover();
        return;
    }

    lastGood[0] = bx;
    lastGood[1] = by;
    lastGood[2] = bz;

    y0[0] = bx;
    y0[1] = by;
    y0[2] = bz;

#endif   
}


void sfun_MagLevTbx_MagSens_WrappedTerminate(void)
{
#ifndef MATLAB_MEX_FILE

#endif      
}

#ifdef __cplusplus
}
#endif
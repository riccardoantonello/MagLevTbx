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

#include <TCA9548.h>
#include <TLV493D.h>

#include <rtwtypes.h>

/*  Defines -------------------------------------------------------------*/

/*  I2C clock frequency [Hz]    */
const uint32_t I2C_CLOCK_HZ = 400000; 

/*  Number of sensors   */
const uint8_t  NUM_SENSORS = 8;

/*  TCA9548 default 7-bit I2C address  */
const uint8_t TCA9548_ADDR = 0x70;

/*  MCU reset pin for TCA9548 I2C mux   */
const uint8_t TCA9548_RESET_PIN = 26;


/*  Variables -----------------------------------------------------------*/
TCA9548 i2cMux(TCA9548_ADDR);


/*  Private variables ---------------------------------------------------*/
static int8_t activeChannel = -1;
static bool magInitialized[NUM_SENSORS] = {false};
static double lastGood[NUM_SENSORS][3] = {{0.0}};


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
    activeChannel = -1;
}


#endif

/* Callback functions definitions ---------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


void sfun_MagLevTbx_i2cMux_WrappedStart(void)
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

    //  Initialize the I2C multiplexer
    if (!i2cMux.begin()) {
        return;
    }

    //  Set reset pin
    i2cMux.setResetPin(TCA9548_RESET_PIN);

    //  Reset mux and wait briefly
    i2cMux.reset();
    delay(10);

    // Do not call i2cMux.reset() unless a real reset pin was configured.
    if (!i2cMux.isConnected()) {
        return;
    }

    //  Init state variables
    activeChannel = -1;
    for (uint8_t i = 0; i < NUM_SENSORS; ++i) {
        magInitialized[i] = false;
        lastGood[i][0] = 0.0;
        lastGood[i][1] = 0.0;
        lastGood[i][2] = 0.0;
    }

#endif
}


void sfun_MagLevTbx_MagSens_WrappedStart(uint8_T sensorId)
{
#ifndef MATLAB_MEX_FILE

    //  Return in case of invalid sensor
    if (sensorId >= NUM_SENSORS) {
        return;
    }
        
    //  Select mux channel
    if (!i2cMux.selectChannel(sensorId)) {
        activeChannel = -1;
        return false;
    }
    activeChannel = (int8_t)sensorId;
    delayMicroseconds(200);

    //  Init the sensor connected to the currently selected mux channel
    magInitialized[sensorId] = TLV493D_InitSensor();

#endif
}


void sfun_MagLevTbx_MagSens_WrappedOutput(uint8_T sensorId, double *y0)
{
#ifndef MATLAB_MEX_FILE

    //  Return in case of invalid sensor
    if (sensorId >= NUM_SENSORS) {
        y0[0] = y0[1] = y0[2] = NAN;
        return;
    }

    //  Select mux channel for this sensor only when needed
    if (activeChannel != sensorId) {
        if (!i2cMux.selectChannel(sensorId)) {
            y0[0] = lastGood[sensorId][0];
            y0[1] = lastGood[sensorId][1];
            y0[2] = lastGood[sensorId][2];
            activeChannel = -1;
            return;
        }
        activeChannel = sensorId;
        delayMicroseconds(50);
    }

    //  If initialization failed or the sensor was reset, try to configure it again
    if (!magInitialized[sensorId]) {
        magInitialized[sensorId] = TLV493D_InitSensor();
        if (!magInitialized[sensorId]) {
            y0[0] = lastGood[sensorId][0];
            y0[1] = lastGood[sensorId][1];
            y0[2] = lastGood[sensorId][2];
            return;
        }
    }

    //  Read data
    double bx, by, bz;
    if (!TLV493D_GetMagField_mT(&bx, &by, &bz)) {
        magInitialized[sensorId] = false;
        y0[0] = lastGood[sensorId][0];
        y0[1] = lastGood[sensorId][1];
        y0[2] = lastGood[sensorId][2];
        i2cRecover();
        return;
    }

    lastGood[sensorId][0] = bx;
    lastGood[sensorId][1] = by;
    lastGood[sensorId][2] = bz;

    y0[0] = bx;
    y0[1] = by;
    y0[2] = bz;

#endif   
}


void sfun_MagLevTbx_MagSens_WrappedTerminate(void)
{
#ifndef MATLAB_MEX_FILE

    // Leave the I2C bus in a safe state: deselect all TCA9548 channels
    i2cMux.disableAllChannels();
    activeChannel = -1;

#endif      
}

#ifdef __cplusplus
}
#endif
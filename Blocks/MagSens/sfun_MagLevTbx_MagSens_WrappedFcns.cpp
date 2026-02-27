/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 * 
 * February 15, 2026
 *
 * Dept. of Information Engineering, University of Padova 
 *
 */

#ifndef MATLAB_MEX_FILE

#include <Arduino.h>
#include "Wire.h"
#include <TCA9548.h>
#include <TLx493D_inc.hpp>
#include <rtwtypes.h>

// Create an instance of the TCA9548 I2C multiplexer
TCA9548 mux_sensors(0x70);

// Create an array of TLx493D_A1B6 sensor objects with same I2C address 
// since they are on different channels of the multiplexer
using namespace ifx::tlx493d;

const int NUM_SENSORS = 8;
TLx493D_A1B6 mag_sensor[NUM_SENSORS] = {
    TLx493D_A1B6(Wire, TLx493D_IIC_ADDR_A0_e),
    TLx493D_A1B6(Wire, TLx493D_IIC_ADDR_A0_e),
    TLx493D_A1B6(Wire, TLx493D_IIC_ADDR_A0_e),
    TLx493D_A1B6(Wire, TLx493D_IIC_ADDR_A0_e),
    TLx493D_A1B6(Wire, TLx493D_IIC_ADDR_A0_e),
    TLx493D_A1B6(Wire, TLx493D_IIC_ADDR_A0_e),
    TLx493D_A1B6(Wire, TLx493D_IIC_ADDR_A0_e),
    TLx493D_A1B6(Wire, TLx493D_IIC_ADDR_A0_e)
};


#endif

#ifdef __cplusplus
extern "C" {
#endif


void sfun_MagLevTbx_i2cMux_WrappedStart(void)
{
#ifndef MATLAB_MEX_FILE

    //  Initialize I2C communication
    Wire.begin();
    Wire.setClock(400000);  // Set I2C clock speed to 400 kHz

    //  Initialize the I2C multiplexer
    if (!mux_sensors.begin()) {
        //  Failed to initialize multiplexer.
        while (1);
    }

    // Reset mux and wait briefly
    mux_sensors.reset();
    delay(10);

    if(!mux_sensors.isConnected()) {
        //  Failed to connect to multiplexer.
        while (1);
    }
        
#endif
}


void sfun_MagLevTbx_MagSens_WrappedStart(uint8_T sensorId)
{
#ifndef MATLAB_MEX_FILE
        
    //  Enable mux channel for selected mag sensor
    if (!mux_sensors.enableChannel(sensorId)) {
        //  Failed to enable channel
        while (1);
    }

    //  Select mux channel for mag sensor to initialize
    if (!mux_sensors.selectChannel(sensorId)) {
        //  Failed to select channel
        while (1);
    }
    delay(20);

    //  Initialize selected mag sensor
    if (!mag_sensor[sensorId].begin()) {
        //  Failed to intialize sensor
        while (1);
    }

#endif
}


void sfun_MagLevTbx_MagSens_WrappedOutput(uint8_T sensorId, double *y0)
{
#ifndef MATLAB_MEX_FILE

    //  Select mux channel for mag sensor to read
    if (!mux_sensors.selectChannel(sensorId)) {
        //  Failed to select channel
        while (1);
    }

    //  Read magnetic field
    if(!mag_sensor[sensorId].getMagneticField(&y0[0], &y0[1], &y0[2])) {
        //  Failed to read magnetic field data
        while (1);
    }

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
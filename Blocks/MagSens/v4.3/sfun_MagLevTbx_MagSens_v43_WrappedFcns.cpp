/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 * 
 * March 25, 2026
 *
 * Dept. of Information Engineering, University of Padova 
 *
 */

#ifndef MATLAB_MEX_FILE

#include <Arduino.h>
#include "Wire.h"
#include <TLx493D_inc.hpp>
#include <rtwtypes.h>

using namespace ifx::tlx493d;

// Create a TLx493D_A1B6 sensor object
TLx493D_A1B6 mag_sensor = TLx493D_A1B6(Wire, TLx493D_IIC_ADDR_A0_e);


#endif

#ifdef __cplusplus
extern "C" {
#endif


void sfun_MagLevTbx_MagSens_WrappedStart(void)
{
#ifndef MATLAB_MEX_FILE

    //  Initialize I2C communication
    Wire.begin();
    Wire.setClock(400000);  // Set I2C clock speed to 400 kHz

    //  Initialize selected mag sensor
    if (!mag_sensor.begin()) {
        //  Failed to intialize sensor
        while (1);
    }

#endif
}


void sfun_MagLevTbx_MagSens_WrappedOutput(double *y0)
{
#ifndef MATLAB_MEX_FILE

    //  Read magnetic field
    if(!mag_sensor.getMagneticField(&y0[0], &y0[1], &y0[2])) {
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
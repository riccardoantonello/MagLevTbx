/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 * 
 * February 17, 2026
 *
 * Dept. of Information Engineering, University of Padova 
 *
 */

#ifndef MATLAB_MEX_FILE

#include <Arduino.h>
#include <rtwtypes.h>
#include "cobs.h"

#endif


#ifdef __cplusplus
extern "C" {
#endif

    
void sfun_MagLevTbx_UsbSerialPacketSend_WrappedStart()
{
#ifndef MATLAB_MEX_FILE
    /*  
     *  Serial.begin() is optional on Teensy. 
     *  USB hardware initialization is performed before setup() runs.  
     */
#endif
}


void sfun_MagLevTbx_UsbSerialPacketSend_WrappedOutput(uint8_T *pPacketBuffer, \
        uint32_T packetSize, boolean_T waitDTR)
{
#ifndef MATLAB_MEX_FILE
    
    if (waitDTR == true)
    {
        if (Serial.dtr())
            Serial.write( (byte *)pPacketBuffer, packetSize );
    }
    else
    {
        Serial.write( (byte *)pPacketBuffer, packetSize );
    }

#endif   
}


void sfun_MagLevTbx_UsbSerialPacketSend_WrappedTerminate(int8_T serialPort)
{
#ifndef MATLAB_MEX_FILE
    
    /*  
     *  Serial.end() is not required on Teensy. 
     */

#endif      
}


#ifdef __cplusplus
}
#endif
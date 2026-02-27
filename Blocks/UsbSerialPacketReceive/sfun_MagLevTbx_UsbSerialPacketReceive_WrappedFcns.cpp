/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 * 
 * February 18, 2026
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
    
void sfun_MagLevTbx_UsbSerialPacketReceive_WrappedStart()
{
#ifndef MATLAB_MEX_FILE
    /*
     *  Serial.begin() is optional on Teensy. 
     *  USB hardware initialization is performed before setup() runs.  
     */
#endif
}


size_t sfun_MagLevTbx_UsbSerialPacketReceive_WrappedOutput( \
        uint8_T *pPacketBuffer, uint32_T packetSize, \
        boolean_T nullTerminated)
{
#ifndef MATLAB_MEX_FILE
    
    size_t receivedBytesNum = 0;
    
    /*  Read last received packet  */
    while(Serial.available() >= packetSize)
        if(nullTerminated)
        {
            /*  Read serial buffer until either the terminator or the
             *  specified amount of data (packetSize) is found.  
             *  NOTE: if detected, the null terminator is not included 
             *  among the bytes returned by the "readBytesUntil" routine.
             */
            receivedBytesNum = Serial.readBytesUntil(0x00, pPacketBuffer, packetSize); 
            if(receivedBytesNum == packetSize-1)
            {
                /*  A null-terminated packet has been correctly received.
                 *  Note that the "readBytesUntil" routine does not include 
                 *  the null terminator among the returned bytes.
                 * /
                
                /*  Add null terminator to the received packet */
                pPacketBuffer[packetSize-1] = 0x00;
                
                /*  Update received-bytes count  */
                receivedBytesNum++;
            }
            else
            {
                /*  The received packet is either a null-terminated packet 
                 *  smaller than the specified size, or it is not null-terminated 
                 */
                
                /*  Set received-bytes count to zero if the packet is not 
                 *  null-terminated (this prevents false detection of received 
                 *  packets of correct size, but not null terminated)
                 */
                if(receivedBytesNum == packetSize)
                    receivedBytesNum = 0;
            }
        }
        else
        {
            receivedBytesNum = Serial.readBytes(pPacketBuffer, packetSize);
        }
    
    /*  Return number of bytes placed in buffer  */
    return receivedBytesNum;

#endif   
}


void sfun_MagLevTbx_UsbSerialPacketReceive_WrappedTerminate(int8_T serialPort)
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
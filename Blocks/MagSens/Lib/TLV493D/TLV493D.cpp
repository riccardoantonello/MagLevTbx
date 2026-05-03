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
#include <TLV493D.h>

#include <math.h>
#include <rtwtypes.h>

/*  Private functions prototypes ----------------------------------------*/
static bool TLV493D_ConfigSensor(void);

static bool TLV493D_WriteRegisters(uint8_t reg0, uint8_t reg1_woParity, uint8_t reg2, uint8_t reg3);
static bool TLV493D_OddParity(uint8_t reg0, uint8_t reg1_woParityBit, uint8_t reg2, uint8_t reg3);
static inline int16_t TLV493D_SignExtend12(uint16_t value);

#endif


/*  Functions definitions -----------------------------------------------*/
#ifndef MATLAB_MEX_FILE

bool TLV493D_InitSensor(void)
{
    //  Reset only the sensor
    TLV493D_ResetSensor();

    //  Dummy read - synchronizes read pointer after cold power-up or reset
    //  Note: works by reading all 10 internal read registers.
    Wire.requestFrom((uint8_t)TLV493D_A1B6_ADDR, 10, true);
    while (Wire.available()) { 
        Wire.read();
    }

    //  Configure sensor
    if (!TLV493D_ConfigSensor()) {
        return false;
    }

    //  Allow one low-power measurement period before first direct read
    delay(15);

    return true;
}


bool TLV493D_ResetSensor(void)
{
    //  A general reset is trigged by calling the address 0x00 on I2C bus
    Wire.beginTransmission(0x00);
    uint8_t err = Wire.endTransmission(true);

    // allow sensor power-up/reset sequence
    delay(5);   

    return (err == 0);
}


bool TLV493D_GetMagField_mT(double *bx, double *by, double *bz)
{
    uint8_t b[7];

    //  Read registers 0x00..0x06. The measurement frame starts at 0x00
    uint8_t n = Wire.requestFrom((uint8_t)TLV493D_A1B6_ADDR, (uint8_t)7, (uint8_t)true);
    if (n != 7) {
        while (Wire.available()) { (void)Wire.read(); }
        return false;
    }

    for (uint8_t i = 0; i < 7; ++i) {
        if (!Wire.available()) {
            return false;
        }
        b[i] = Wire.read();
    }

    //  Get raw mag field measurements
    //  Bx = byte0[7:0] + byte4[7:4]
    //  By = byte1[7:0] + byte4[3:0]
    //  Bz = byte2[7:0] + byte5[3:0]
    int16_t rawX = TLV493D_SignExtend12(((uint16_t)b[0] << 4) | ((b[4] >> 4) & 0x0F));
    int16_t rawY = TLV493D_SignExtend12(((uint16_t)b[1] << 4) | ( b[4]       & 0x0F));
    int16_t rawZ = TLV493D_SignExtend12(((uint16_t)b[2] << 4) | ( b[5]       & 0x0F));

    //  Convert to [mT] units
    *bx = ((double)rawX) * TLV493D_LSB_mT;
    *by = ((double)rawY) * TLV493D_LSB_mT;
    *bz = ((double)rawZ) * TLV493D_LSB_mT;

    return true;
}


/*  Private functions definitions ---------------------------------------*/
static bool TLV493D_ConfigSensor(void)
{
    //  Sensor configuration (MOD1 & MOD2 wirite registers):
    //  - Master Controlled Mode (MOD1.1 = 1, MOD1.0 = 1)
    //  - Interrupt pad disabled (MOD1.2 = 0)
    //  - Parity test enabled (MOD2.5 = 1)
    //  - Low power period = 12ms (MOD2.6 = 1)
    //  - Temperature measurement enabled (MOD2.7 = 0)
    return TLV493D_WriteRegisters(0x00, 0x03, 0x00, 0x60);
}


static bool TLV493D_WriteRegisters(uint8_t reg0, uint8_t reg1_woParityBit, uint8_t reg2, uint8_t reg3)
{
    uint8_t reg1; 
    
    //  Sanity check - reset odd parity bit (bit 7)
    reg1 = reg1_woParityBit & 0x7F;
    
    //  Add odd parity bit (bit 7)
    if( ~TLV493D_OddParity(reg0, reg1_woParityBit, reg2, reg3) )
        reg1 |= 0x80;

    //  Write registers 
    Wire.beginTransmission(TLV493D_A1B6_ADDR);
    Wire.write(reg0);     //  Reserved
    Wire.write(reg1);     //  MOD1 register
    Wire.write(reg2);     //  Reserved
    Wire.write(reg3);     //  MOD2 register
    return (Wire.endTransmission(true) == 0);
}


static bool TLV493D_OddParity(uint8_t reg0, uint8_t reg1_woParityBit, uint8_t reg2, uint8_t reg3)
{
    uint8_t x = reg0 ^ reg1_woParityBit ^ reg2 ^ reg3;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;

    return x & 0x01;
}


static inline int16_t TLV493D_SignExtend12(uint16_t value)
{
    value &= 0x0FFF;
    if (value & 0x0800) {
        value |= 0xF000;
    }
    return (int16_t)value;
}


#endif
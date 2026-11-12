/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 *
 * Dept. of Information Engineering, University of Padova
 *
 * A1B6 branch: unchanged from the original working driver.
 * A2B6 branch: ported from Infineon's TLx493D reference library
 * (tlx493d_common.c / tlx493d_gen_2_common.c / TLx493D_A2B6.c), reduced
 * to the concrete register writes needed for continuous Bx/By/Bz/Temp
 * reads in Master Controlled Mode - no external library dependency.
 */

#ifndef MATLAB_MEX_FILE

/*  Includes ------------------------------------------------------------*/
#include <Arduino.h>
#include "Wire.h"
#include <TLV493D.h>

#include <math.h>
#include <rtwtypes.h>

#if defined(TLV493D_VARIANT_A1B6)

/* =======================================================================
 *  A1B6 (1st generation) - unchanged from the original driver
 * =====================================================================*/

/*  Private functions prototypes ----------------------------------------*/
static bool TLV493D_ConfigSensor(void);

static bool TLV493D_WriteRegisters(uint8_t reg0, uint8_t reg1_woParity, uint8_t reg2, uint8_t reg3);
static bool TLV493D_OddParity(uint8_t reg0, uint8_t reg1_woParityBit, uint8_t reg2, uint8_t reg3);
static inline int16_t TLV493D_SignExtend12(uint16_t value);


bool TLV493D_InitSensor(void)
{
    //  Reset only the sensor
    TLV493D_ResetSensor();

    //  Dummy read - synchronizes read pointer after cold power-up or reset
    //  Note: works by reading all 10 internal read registers.
    Wire.requestFrom((uint8_t)TLV493D_ADDR, 10, true);
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
    static uint8_t lastFrm = 0xFF;
    static uint16_t stuckCnt = 0;
    uint8_t b[7];
    uint8_t frm;

    //  Read registers 0x00..0x06. The measurement frame starts at 0x00
    uint8_t n = Wire.requestFrom((uint8_t)TLV493D_ADDR, (uint8_t)7, (uint8_t)true);
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

    //  ADC hang-up detection
    //
    //  IMPORTANT NOTE: This hang-up detection mechanism uses static variables
    //  to track missing frames. Consequently, it is not suitable for
    //  multi-sensor configurations, as the state is shared across all sensors.
    //
    //  For multi-sensor operation, the routine must be rewritten to maintain
    //  separate state information for each sensor instance.
    //
    frm = (b[3] >> 2) & 0x03;
    if (frm == lastFrm) {
        if (++stuckCnt > 5) {           // reset after 5 missed frames
            TLV493D_ResetSensor();      // general call 0x00
            TLV493D_ConfigSensor();     // re-write MOD1/MOD2
            stuckCnt = 0;
            lastFrm = 0xFF;
            return false;
        }
    } else {
        stuckCnt = 0;
        lastFrm = frm;
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
    //  Sensor configuration (MOD1 & MOD2 write registers):
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
    if( !TLV493D_OddParity(reg0, reg1_woParityBit, reg2, reg3) )
        reg1 |= 0x80;

    //  Write registers
    Wire.beginTransmission(TLV493D_ADDR);
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


#elif defined(TLV493D_VARIANT_A2B6)

/* =======================================================================
 *  A2B6 (2nd generation) - ported from Infineon's TLx493D reference
 *  library. Register map (verified from Infineon corelib source):
 *
 *    Read frame  (0x00..0x06):
 *      0x00 = Bx MSB (8 bit)      0x01 = By MSB (8 bit)
 *      0x02 = Bz MSB (8 bit)      0x03 = Temp MSB (8 bit)
 *      0x04 = Bx LSB[7:4] | By LSB[3:0]
 *      0x05 = Temp LSB[7:6] | ID[5:4] | Bz LSB[3:0]
 *      0x06 = P FF CF T PD3 PD0 FRM[1:0]   (diagnostic)
 *
 *    Write registers:
 *      0x10 CONFIG = DT AM TRIG[1:0] X2 TL_MAG[1:0] CP
 *      0x11 MOD1   = FP IICADR[1:0] PR CA INT MODE[1:0]
 *
 *  Master Controlled Mode = MODE=0b01. FP/CP are parity bits computed
 *  over the register byte being written (odd/even parity respectively) -
 *  this does NOT require reading factory trim data first.
 * =====================================================================*/

static bool TLV493D_WriteRegister(uint8_t reg, uint8_t value);
static inline uint8_t TLV493D_Parity(uint8_t data);
static inline int16_t TLV493D_SignExtend12(uint16_t value);

/*
bool TLV493D_InitSensor(void)
{
    //  No general-reset or software-reset command exists for this
    //  generation - power-up + straight (re)configuration is the
    //  supported bring-up sequence.
    delay(5);

    //  CONFIG (0x10): DT=0, AM=0 (Bx,By,Bz,Temp), TRIG=0, X2=0 (full
    //  range), TL_MAG=0. CP = even parity over the other 7 bits (=0
    //  here since all other bits are 0).
    uint8_t config = 0x00;

    //  MOD1 (0x11): IICADR=00 (A0), PR=1, CA=0, INT=1 (int pad disabled),
    //  MODE=01 (Master Controlled Mode) -> 0b00010101 before FP.
    uint8_t mod1_woFP = 0x15;
    uint8_t fp = TLV493D_Parity(mod1_woFP) ^ 0x01;   //  odd parity
    uint8_t mod1 = mod1_woFP | (fp << 7);

    if (!TLV493D_WriteRegister(0x10, config)) {
        return false;
    }
    if (!TLV493D_WriteRegister(0x11, mod1)) {
        return false;
    }

    //  Allow one measurement period before first direct read
    delay(15);

    return true;
}*/
bool TLV493D_InitSensor(void)
{
    delay(10);

    // CONFIG
    if (!TLV493D_WriteRegister(0x10, 0x20))
    {
        return false;
    }

    // MOD1
    if (!TLV493D_WriteRegister(0x11, 0x15))
    {
        return false;
    }

    delay(20);

    return true;
}


bool TLV493D_ResetSensor(void)
{
    //  No dedicated reset command for this generation - re-running
    //  init() re-applies the known-good configuration.
    return TLV493D_InitSensor();
}


bool TLV493D_GetMagField_mT(double *bx, double *by, double *bz)
{
    static uint8_t lastFrm = 0xFF;
    static uint16_t stuckCnt = 0;
    uint8_t b[7];

    uint8_t n = Wire.requestFrom((uint8_t)TLV493D_ADDR, (uint8_t)7, (uint8_t)true);
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

    //  ADC hang-up detection (see A1B6 branch note: not multi-sensor safe)
    uint8_t frm = b[6] & 0x03;
    if (frm == lastFrm) {
        if (++stuckCnt > 5) {
            TLV493D_ResetSensor();
            stuckCnt = 0;
            lastFrm = 0xFF;
            return false;
        }
    } else {
        stuckCnt = 0;
        lastFrm = frm;
    }

    //  Bx = byte0[7:0] + byte4[7:4], By = byte1[7:0] + byte4[3:0],
    //  Bz = byte2[7:0] + byte5[3:0]  (same packing scheme as gen 1)
    int16_t rawX = TLV493D_SignExtend12(((uint16_t)b[0] << 4) | ((b[4] >> 4) & 0x0F));
    int16_t rawY = TLV493D_SignExtend12(((uint16_t)b[1] << 4) | ( b[4]       & 0x0F));
    int16_t rawZ = TLV493D_SignExtend12(((uint16_t)b[2] << 4) | ( b[5]       & 0x0F));

    *bx = ((double)rawX) / TLV493D_SENSITIVITY_LSB_PER_mT;
    *by = ((double)rawY) / TLV493D_SENSITIVITY_LSB_PER_mT;
    *bz = ((double)rawZ) / TLV493D_SENSITIVITY_LSB_PER_mT;

    return true;
}


static bool TLV493D_WriteRegister(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(TLV493D_ADDR);
    Wire.write(reg);
    Wire.write(value);
    return (Wire.endTransmission(true) == 0);
}


static inline uint8_t TLV493D_Parity(uint8_t data)
{
    data ^= data >> 4;
    data ^= data >> 2;
    data ^= data >> 1;
    return data & 0x01;
}


static inline int16_t TLV493D_SignExtend12(uint16_t value)
{
    value &= 0x0FFF;
    if (value & 0x0800) {
        value |= 0xF000;
    }
    return (int16_t)value;
}

#endif /* TLV493D_VARIANT_* */

#endif /* MATLAB_MEX_FILE */
/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 *
 * Dept. of Information Engineering, University of Padova
 *
 */

#ifndef _TLV493D_H_
#define _TLV493D_H_

/*  ----------------------------------------------------------------------
 *  Select which sensor is populated on THIS unit. Uncomment exactly one.
 *
 *  A1B6 (1st gen) and A2B6 (2nd gen) use different I2C register maps -
 *  this selects the correct read/config logic, not just an address.
 *  --------------------------------------------------------------------*/
#define TLV493D_VARIANT_A1B6
// #define TLV493D_VARIANT_A2B6

#if defined(TLV493D_VARIANT_A1B6)

    /*  TLV493D-A1B6 default 7-bit I2C address (ADDR pin low at power-up) */
    const uint8_t TLV493D_ADDR   = 0x5E;
    /*  TLV493D-A1B6 LSB to mT conversion gain  */
    const double  TLV493D_LSB_mT = 0.098;

#elif defined(TLV493D_VARIANT_A2B6)

    /*  TLE493D-A2B6 default 7-bit I2C address (A0 address option)  */
    const uint8_t TLV493D_ADDR   = 0x35;
    /*  TLE493D-A2B6 full-range sensitivity: mT = raw / 7.7  */
    const double  TLV493D_SENSITIVITY_LSB_PER_mT = 7.7;

#else
    #error "Define exactly one TLV493D_VARIANT_* above"
#endif

/*  Functions prototypes ------------------------------------------------*/
bool TLV493D_InitSensor(void);
bool TLV493D_ResetSensor(void);
bool TLV493D_GetMagField_mT(double *bx, double *by, double *bz);

#endif
/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 * 
 * May 02, 2026
 *
 * Dept. of Information Engineering, University of Padova 
 *
 */

#ifndef _TLV493D_H_
#define _TLV493D_H_

/*  Defines -------------------------------------------------------------*/

/*  TLV493D-A1B6 default 7-bit I2C address  */
const uint8_t TLV493D_A1B6_ADDR = 0x5E;

/*  TLV493D-A1B6 LSB to mT conversion gain  */
const double TLV493D_LSB_mT = 0.098;

/*  Functions prototypes ------------------------------------------------*/
bool TLV493D_InitSensor(void);
bool TLV493D_ResetSensor(void);
bool TLV493D_GetMagField_mT(double *bx, double *by, double *bz);

#endif 


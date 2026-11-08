/*
 * TMAG5170 register map, frame layout, CRC and conversions.
 *
 * * Author: Theodor Johansson (tajohans@ntnu.no)
 *
 * Ported from the Zephyr RTOS TMAG5170 driver:
 *   https://github.com/zephyrproject-rtos/zephyr/tree/main/drivers/sensor/ti/tmag5170
 *   Copyright (c) 2023 Michal Morsisko
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef TMAG5170_REGISTERS_H
#define TMAG5170_REGISTERS_H

#include "TMAG5170.h"

#include <stdint.h>

namespace TMAG5170 {

/*  Register map
 *  Fields are listed from MSB to LSB as in the
 *  datasheet register tables (section 7.6.1).
 */
struct bitfield {
  uint8_t msb;
  uint8_t lsb;
};

// clang-format off

namespace DEVICE_CONFIG {
constexpr uint8_t  ADDRESS        = 0x00;
//                 RESERVED       = {15, 15};
constexpr bitfield CONV_AVG       = {14, 12};
//                 RESERVED       = {11, 10};
constexpr bitfield MAG_TEMPCO     = {9, 8};
//                 RESERVED       = { 7,  7};
constexpr bitfield OPERATING_MODE = {6, 4};
constexpr bitfield T_CH_EN        = {3, 3};
constexpr bitfield T_RATE         = {2, 2};
constexpr bitfield T_HLT_EN       = {1, 1};
//                 RESERVED       = { 0,  0};
} // namespace DEVICE_CONFIG

namespace SENSOR_CONFIG {
constexpr uint8_t  ADDRESS   = 0x01;
constexpr bitfield ANGLE_EN  = {15, 14};
constexpr bitfield SLEEPTIME = {13, 10};
constexpr bitfield MAG_CH_EN = {9, 6};
constexpr bitfield Z_RANGE   = {5, 4};
constexpr bitfield Y_RANGE   = {3, 2};
constexpr bitfield X_RANGE   = {1, 0};
} // namespace SENSOR_CONFIG

namespace SYSTEM_CONFIG {
constexpr uint8_t  ADDRESS      = 0x02;
//                 RESERVED     = {15, 14};
constexpr bitfield DIAG_SEL     = {13, 12};
//                 RESERVED     = {11, 11};
constexpr bitfield TRIGGER_MODE = {10, 9};
constexpr bitfield DATA_TYPE    = {8, 6};
constexpr bitfield DIAG_EN      = {5, 5};
//                 RESERVED     = { 4,  3};
constexpr bitfield Z_HLT_EN     = {2, 2};
constexpr bitfield Y_HLT_EN     = {1, 1};
constexpr bitfield X_HLT_EN     = {0, 0};
} // namespace SYSTEM_CONFIG

namespace ALERT_CONFIG {
constexpr uint8_t  ADDRESS     = 0x03;
//                 RESERVED    = {15, 14};
constexpr bitfield ALERT_LATCH = {13, 13};
constexpr bitfield ALERT_MODE  = {12, 12};
constexpr bitfield STATUS_ALRT = {11, 11};
//                 RESERVED    = {10,  9};
constexpr bitfield RSLT_ALRT   = {8, 8};
//                 RESERVED    = { 7,  6};
constexpr bitfield THRX_COUNT  = {5, 4};
constexpr bitfield T_THRX_ALRT = {3, 3};
constexpr bitfield Z_THRX_ALRT = {2, 2};
constexpr bitfield Y_THRX_ALRT = {1, 1};
constexpr bitfield X_THRX_ALRT = {0, 0};
} // namespace ALERT_CONFIG

namespace X_THRX_CONFIG {
constexpr uint8_t  ADDRESS        = 0x04;
constexpr bitfield X_HI_THRESHOLD = {15, 8};
constexpr bitfield X_LO_THRESHOLD = {7, 0};
} // namespace X_THRX_CONFIG

namespace Y_THRX_CONFIG {
constexpr uint8_t  ADDRESS        = 0x05;
constexpr bitfield Y_HI_THRESHOLD = {15, 8};
constexpr bitfield Y_LO_THRESHOLD = {7, 0};
} // namespace Y_THRX_CONFIG

namespace Z_THRX_CONFIG {
constexpr uint8_t  ADDRESS        = 0x06;
constexpr bitfield Z_HI_THRESHOLD = {15, 8};
constexpr bitfield Z_LO_THRESHOLD = {7, 0};
} // namespace Z_THRX_CONFIG

namespace T_THRX_CONFIG {
constexpr uint8_t  ADDRESS        = 0x07;
constexpr bitfield T_HI_THRESHOLD = {15, 8};
constexpr bitfield T_LO_THRESHOLD = {7, 0};
} // namespace T_THRX_CONFIG

namespace CONV_STATUS {
constexpr uint8_t  ADDRESS     = 0x08;
//                 RESERVED    = {15, 14};
constexpr bitfield RDY         = {13, 13};
constexpr bitfield A           = {12, 12};
constexpr bitfield T           = {11, 11};
constexpr bitfield Z           = {10, 10};
constexpr bitfield Y           = {9, 9};
constexpr bitfield X           = {8, 8};
//                 RESERVED    = { 7,  7};
constexpr bitfield SET_COUNT   = {6, 4};
//                 RESERVED    = { 3,  2};
constexpr bitfield ALRT_STATUS = {1, 0};
} // namespace CONV_STATUS

namespace X_CH_RESULT {
constexpr uint8_t  ADDRESS     = 0x09;
constexpr bitfield X_CH_RESULT = {15, 0};
} // namespace X_CH_RESULT

namespace Y_CH_RESULT {
constexpr uint8_t  ADDRESS     = 0x0A;
constexpr bitfield Y_CH_RESULT = {15, 0};
} // namespace Y_CH_RESULT

namespace Z_CH_RESULT {
constexpr uint8_t  ADDRESS     = 0x0B;
constexpr bitfield Z_CH_RESULT = {15, 0};
} // namespace Z_CH_RESULT

namespace TEMP_RESULT {
constexpr uint8_t  ADDRESS     = 0x0C;
constexpr bitfield TEMP_RESULT = {15, 0};
} // namespace TEMP_RESULT

namespace AFE_STATUS {
constexpr uint8_t  ADDRESS   = 0x0D;
constexpr bitfield CFG_RESET = {15, 15};
//                 RESERVED  = {14, 13};
constexpr bitfield SENS_STAT = {12, 12};
constexpr bitfield TEMP_STAT = {11, 11};
constexpr bitfield ZHS_STAT  = {10, 10};
constexpr bitfield YHS_STAT  = {9, 9};
constexpr bitfield XHS_STAT  = {8, 8};
//                 RESERVED  = { 7,  2};
constexpr bitfield TRIM_STAT = {1, 1};
constexpr bitfield LDO_STAT  = {0, 0};
} // namespace AFE_STATUS

namespace SYS_STATUS {
constexpr uint8_t  ADDRESS        = 0x0E;
constexpr bitfield ALRT_LVL       = {15, 15};
constexpr bitfield ALRT_DRV       = {14, 14};
constexpr bitfield SDO_DRV        = {13, 13};
constexpr bitfield CRC_STAT       = {12, 12};
constexpr bitfield FRAME_STAT     = {11, 11};
constexpr bitfield OPERATING_STAT = {10, 8};
//                 RESERVED       = { 7,  6};
constexpr bitfield VCC_OV         = {5, 5};
constexpr bitfield VCC_UV         = {4, 4};
constexpr bitfield TEMP_THX       = {3, 3};
constexpr bitfield ZCH_THX        = {2, 2};
constexpr bitfield YCH_THX        = {1, 1};
constexpr bitfield XCH_THX        = {0, 0};
} // namespace SYS_STATUS

namespace TEST_CONFIG {
constexpr uint8_t  ADDRESS     = 0x0F;
//                 RESERVED    = {15,  6};
constexpr bitfield VER         = {5, 4};
//                 RESERVED    = { 3,  3};
constexpr bitfield CRC_DIS     = {2, 2};
constexpr bitfield OSC_CNT_CTL = {1, 0};
} // namespace TEST_CONFIG

namespace OSC_MONITOR {
constexpr uint8_t  ADDRESS   = 0x10;
constexpr bitfield OSC_COUNT = {15, 0};
} // namespace OSC_MONITOR

namespace MAG_GAIN_CONFIG {
constexpr uint8_t  ADDRESS        = 0x11;
constexpr bitfield GAIN_SELECTION = {15, 14};
//                 RESERVED       = {13, 11};
constexpr bitfield GAIN_VALUE     = {10, 0};
} // namespace MAG_GAIN_CONFIG

namespace MAG_OFFSET_CONFIG {
constexpr uint8_t  ADDRESS          = 0x12;
constexpr bitfield OFFSET_SELECTION = {15, 14};
constexpr bitfield OFFSET_VALUE1    = {13, 7};
constexpr bitfield OFFSET_VALUE2    = {6, 0};
} // namespace MAG_OFFSET_CONFIG

namespace ANGLE_RESULT {
constexpr uint8_t  ADDRESS      = 0x13;
constexpr bitfield ANGLE_RESULT = {15, 0};
} // namespace ANGLE_RESULT

namespace MAGNITUDE_RESULT {
constexpr uint8_t  ADDRESS          = 0x14;
constexpr bitfield MAGNITUDE_RESULT = {15, 0};
} // namespace MAGNITUDE_RESULT

// clang-format on

/*  Command bits (frame bits 7:4): CMD0 triggers a conversion at CS rising edge  */
#define TMAG5170_CMD_NONE               0x0U
#define TMAG5170_CMD_TRIGGER_CONVERSION 0x1U

/*  Read flag (frame bit 31)  */
#define TMAG5170_READ_FLAG 0x80U

/*  CRC-4 (polynomial x^4 + x + 1, seed 0xF)  */
#define TMAG5170_CRC_SEED 0x0FU
#define TMAG5170_CRC_POLY 0x03U


/*  Bitfield access  */
static constexpr uint16_t fieldmask(bitfield field) {
  return (uint16_t)(((1UL << (field.msb - field.lsb + 1)) - 1UL) << field.lsb);
}

template <typename T> static constexpr uint16_t set_field(bitfield field, T value) {
  return (uint16_t)(((uint16_t)value << field.lsb) & fieldmask(field));
}

static constexpr uint16_t get_field(bitfield field, uint16_t reg) {
  return (uint16_t)((reg & fieldmask(field)) >> field.lsb);
}

/*  Convert a 16-bit two's complement X/Y/Z_CH_RESULT code to [mT]: B = code * range / 2^15  */
template <typename Range> static constexpr double code_mT(uint16_t code, Range range) {
  return ((double)(int16_t)code) * full_scale_mT(range) / 32768.0;
}

/*  Full-scale magnetic range [mT] of an X/Y/Z_RANGE setting  */
static constexpr double full_scale_mT(TMAG5170A1::Range range) {
  switch (range) {
  case TMAG5170A1::Range::PM_25mT:
    return 25.0;
  case TMAG5170A1::Range::PM_100mT:
    return 100.0;
  default:
    return 50.0;
  }
}

static constexpr double full_scale_mT(TMAG5170A2::Range range) {
  switch (range) {
  case TMAG5170A2::Range::PM_75mT:
    return 75.0;
  case TMAG5170A2::Range::PM_300mT:
    return 300.0;
  default:
    return 150.0;
  }
}

/*  CRC-4 over the 32-bit frame with its CRC field (bits 3:0) set to zero.
 *  Check value: the frame 0x0F000400 has CRC 0x7.
 */

static inline uint8_t CRC4(const uint8_t frame[4]) {
  uint32_t word = ((uint32_t)frame[0] << 24) | //
                  ((uint32_t)frame[1] << 16) | //
                  ((uint32_t)frame[2] << 8) |  //
                  ((uint32_t)(frame[3] & 0xF0));
  uint8_t crc = TMAG5170_CRC_SEED;

  for (int8_t i = 31; i >= 0; --i) {
    uint8_t inv = ((word >> i) & 0x01) ^ ((crc >> 3) & 0x01);
    crc         = ((crc << 1) & 0x0F) ^ (inv ? TMAG5170_CRC_POLY : 0x00);
  }

  return crc;
}

/* Extract register data of a regular 32-bit RX frame

From the datasheet (Figure 7-11) we know that during a regular frame, the information is
returned in the following manner:
- 31:24 - Last eight status bits (STAT[11:4])
-  23:8 - 16-bit data
-   7:4 - First four status bits (STAT[3:0])
-   3:0 - CRC
 */
static inline uint16_t rx_data(const uint8_t frame[4]) { return (uint16_t)(((uint16_t)frame[1] << 8) | frame[2]); }

/*  SET_COUNT, the rolling 3-bit count of conversion sets, from STAT[2:0] (frame bits 6:4) of an RX frame.
 *  STAT[2:0] carry SET_COUNT when the previous TX frame had CMD1 = 0 (see datasheet Figures 7-10 and 7-11).
 */
static inline uint8_t rx_set_count(const uint8_t frame[4]) { return (uint8_t)((frame[3] >> 4) & 0x07); }

/*  Check a received frame. A frame of all 0x00 or all 0xFF means MISO was not driven: no sensor
 *  answered. Neither has a valid CRC (0x9 and 0xE), so no valid frame is reported as NO_RESPONSE.
 */
static inline Status check_rx_frame(const uint8_t frame[4]) {
  const bool all_low  = (frame[0] | frame[1] | frame[2] | frame[3]) == 0x00;
  const bool all_high = (frame[0] & frame[1] & frame[2] & frame[3]) == 0xFF;

  if (all_low || all_high) {
    return Status::NO_RESPONSE;
  }

  return (frame[3] & 0x0F) == CRC4(frame) ? Status::OK : Status::CRC_ERROR;
}

} // namespace TMAG5170

#endif // TMAG5170_REGISTERS_H

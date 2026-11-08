/*
 * TMAG5170 3D linear Hall-effect sensor driver (SPI) for Teensy/Arduino.
 *
 * Author: Theodor Johansson (tajohans@ntnu.no)
 *
 * Ported from the Zephyr RTOS TMAG5170 driver:
 *   https://github.com/zephyrproject-rtos/zephyr/tree/main/drivers/sensor/ti/tmag5170
 *   Copyright (c) 2023 Michal Morsisko
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef TMAG5170_H
#define TMAG5170_H

#include <stdint.h>

namespace TMAG5170 {

/*  Device configuration ----------------------------------------------*/

/* Conversion averaging (CONV_AVG)
From the datasheet:
Enables additional sampling of the sensor data to reduce the noise
effect (or to increase resolution)
0h = 1x - 10.0Ksps (3-axes) or 20Ksps (1 axis)
1h = 2x - 5.7Ksps (3-axes) or 13.3Ksps (1 axis)
2h = 4x - 3.1Ksps (3-axes) or 8.0Ksps (1 axis)
3h = 8x - 1.6Ksps (3-axes) or 4.4Ksps (1 axis)
4h = 16x - 0.8Ksps (3-axes) or 2.4Ksps (1 axis)
5h = 32x - 0.4Ksps (3-axes) or 1.2Ksps (1 axis)
6h = Code not used, defaults to 000b if selected
7h = Code not used, defaults to 000b if selected
*/
enum class ConversionAveraging : uint8_t {
  X1  = 0x0,
  X2  = 0x1,
  X4  = 0x2,
  X8  = 0x3,
  X16 = 0x4,
  X32 = 0x5
};

/* Temperature coefficient (MAG_TEMPCO)
From the datasheet:
Temperature coefficient of sense magnet
0h = 0%/ deg C (Current sensor applications)
1h = 0.12%/deg C (NdBFe)
2h = 0.03% /deg C (SmCo)
3h = 0.2%/deg C (Ceramic)
*/
enum class TemperatureCoefficient : uint8_t {
  NONE    = 0x0,
  NDFEB   = 0x1,
  SMCO    = 0x2,
  CERAMIC = 0x3,
};

/* Operating mode (OPERATING_MODE)
From the datasheet:
Selects operating mode
0h = Configuration mode, Default (TRIGGER_MODE active)
1h = Stand-by mode (TRIGGER_MODE active)
2h = Active measure mode (Continuous conversion)
3h = Active trigger mode (TRIGGER_MODE active)
4h = Wake-up and sleep mode (duty-cycled mode)
5h = Sleep mode
6h = Deep sleep mode (wakes up at CS signal from controller)
7h = Code not used, defaults to 000b if selected
*/
enum class OperatingMode : uint8_t {
  CONFIGURATION     = 0x0,
  STANDBY           = 0x1,
  ACTIVE_MEASURE    = 0x2,
  ACTIVE_TRIGGER    = 0x3,
  WAKE_UP_AND_SLEEP = 0x4,
  SLEEP             = 0x5,
  DEEP_SLEEP        = 0x6,
};

/* Temperature Channel (T_CH_EN)
From the datasheet:
Enables data acquisition of the temperature channel
0h = Temp channel disabled, Default
1h = Temp channel enabled
*/
enum class TemperatureChannelEnable : uint8_t {
  DISABLED = 0x0,
  ENABLED  = 0x1,
};

/* Temperature conversion rate (T_RATE)
From the datasheet:
Temperature conversion rate. It is linked to the CONV_AVG field
0h = Same as other sensors per CONV_AVG, Default
1h = Once per conversion set
*/
enum class TemperatureConversionRate : uint8_t {
  SAME_AS_CONV_AVG    = 0x0,
  ONCE_PER_CONVERSION = 0x1,
};

/* Temperature limit check (T_HLT_EN)
From the datasheet:
Enables temperature limit check
0h = Temperature limit check off, Default
1h = Temperature limit check on
*/
enum class TemperatureLimitCheck : uint8_t {
  DISABLED = 0x0,
  ENABLED  = 0x1,
};

/*  Sensor configuration ----------------------------------------------*/

/* Magnetic Angle (ANGLE_EN)
From the datasheet:
Enable angle calculation using two axis data
0h = No angle calculation (default)
1h = X-Y-angle calculation enabled
2h = Y-Z-angle calculation enabled
3h = X-Z-angle calculation enabled
*/
enum class AngleCalculations : uint8_t {
  NONE = 0x0,
  XY   = 0x1,
  YZ   = 0x2,
  XZ   = 0x3,
};

/* Sleep Time (SLEEPTIME)
From the datasheet:
Selects the time spent in low power mode between conversions
when OPERATING_MODE =010b
0h = 1ms
1h = 5ms
2h = 10ms
3h = 15ms
4h = 20ms
5h = 30ms
6h = 50ms
7h = 100ms
8h = 500ms
9h = 1000ms
Ah = Code not used, defaults to 0000b if selected
Bh = Code not used, defaults to 0000b if selected
Ch = Code not used, defaults to 0000b if selected
Dh = Code not used, defaults to 0000b if selected
Eh = Code not used, defaults to 0000b if selected
Fh = Code not used, defaults to 0000b if selected
*/
enum class Sleeptime : uint8_t {
  _1MS    = 0x0,
  _5MS    = 0x1,
  _10MS   = 0x2,
  _15MS   = 0x3,
  _20MS   = 0x4,
  _30MS   = 0x5,
  _50MS   = 0x6,
  _100MS  = 0x7,
  _500MS  = 0x8,
  _1000MS = 0x9,
};

/* Magnetic Channels (MAG_CH_EN)
From the datasheet:
Enables data acquisition of the magnetic axis channel(s)
0h = All magnetic channels OFF (default)
1h = X channel enabled
2h = Y channel enabled
3h = X, Y channel enabled
4h = Z channel enabled
5h = Z, X channel enabled
6h = Y, Z channel enabled
7h = X, Y, Z channel enabled
8h = XYX channel enabled
9h = YXY channel enabled
Ah = YZY channel enabled
Bh = ZYZ channel enabled
Ch = ZXZ channel enabled
Dh = XZX channel enabled
Eh = XYZYX channel enabled
Fh = XYZZYX channel enabled
*/
enum class MagneticChannels : uint8_t {
  NONE   = 0x0,
  X      = 0x1,
  Y      = 0x2,
  XY     = 0x3,
  Z      = 0x4,
  ZX     = 0x5,
  YZ     = 0x6,
  XYZ    = 0x7,
  XYX    = 0x8,
  YXY    = 0x9,
  YZY    = 0xA,
  ZYZ    = 0xB,
  ZXZ    = 0xC,
  XZX    = 0xD,
  XYZYX  = 0xE,
  XYZZYX = 0xF,
};

/*  Measurement ranges (X_RANGE/Y_RANGE/Z_RANGE)
Both versions use the same register codes, but with different full-scale
ranges

From datasheet:
Enables different magnetic ranges to support magnetic fields from
±25mT to ±300mT
0h = ±50mT (TMAG5170A1)/ ±150mT(TMAG5170A2), Default
1h = ±25mT (TMAG5170A1)/ ±75mT(TMAG5170A2)
2h = ±100mT (TMAG5170A1)/ ±300mT(TMAG5170A2)
3h = Code not used, defaults to 00b if selected
*/

/*  System configuration ----------------------------------------------*/

/* Diagnostic Mode (DIAG_SEL)
From datasheet:
Selects a diagnostic mode run
0h = Run all data path diagnostics all together, Default
1h = Run only enabled data path diagnostics all together
2h = Run all data path diagnostics in sequence
3h = Run only enabled data path diagnostics in sequence
*/
enum class DiagnosticMode : uint8_t {
  ALL_TOGETHER             = 0x0,
  ONLY_ENABLED_TOGETHER    = 0x1,
  ALL_IN_SEQUENCE          = 0x2,
  ONLY_ENABLED_IN_SEQUENCE = 0x3,
};

/* Trigger Mode (TRIGGER_MODE)
From datasheet:
Selects a condition which initiates a single conversion based
off already configured registers. A running conversion completes
before executing a trigger. Redundant triggers are ignored.
TRIGGER_MODE is available only during the modes explicitly
mentioned in OPERATING_MODE.
0h = Conversion start at SPI command, Default
1h = Conversion start at CS pulse
2h = Conversion start at ALERT pulse
3h = Code not used, defaults to 00b if selected
*/
enum class TriggerMode : uint8_t {
  SPI   = 0x0,
  CS    = 0x1,
  ALERT = 0x2,
};

/* Data Type (DATA_TYPE)
From datasheet:
Data Type to be accessed from results registers via SPI
0h = Default 32-bit register access
1h = 12-Bit XY data access
2h = 12-Bit XZ data access
3h = 12-Bit ZY data access
4h = 12-Bit XT data access
5h = 12-Bit YT data access
6h = 12-Bit ZT data access
7h = 12-Bit AM data access
*/
enum class DataType : uint8_t {
  _32_BIT    = 0x0,
  _12_BIT_XY = 0x1,
  _12_BIT_XZ = 0x2,
  _12_BIT_ZY = 0x3,
  _12_BIT_XT = 0x4,
  _12_BIT_YT = 0x5,
  _12_BIT_ZT = 0x6,
  _12_BIT_AM = 0x7,
};

/* AFE Diagnostics (DIAG_EN)
From datasheet:
Enables user controlled AFE diagnostic tests
0h = Execution of AFE diagnostics is disabled, Default
1h = Execution of AFE diagnostics is enabled
*/
enum class AFEDiagnostics : uint8_t {
  DISABLED = 0x0,
  ENABLED  = 0x1,
};

/* Magnetic Field Limit Check (Z/Y/X_HLT_EN)
From datasheet:
Enables magnetic field limit check on Z/Y/X axis
0h = Z/Y/X axis limit check off, Default
1h = Z/Y/X axis limit check on
*/
enum class MagneticFieldLimitCheck : uint8_t {
  DISABLED = 0x0,
  ENABLED  = 0x1,
};

/*  ALERT configuration ----------------------------------------------*/

/* Alert Latch (ALERT_LATCH)
From datasheet:
Latched ALERT mode select
0h = ALERT sources are not latched. ALERT is asserted only while
     the source of the ALERT response is present
1h = ALERT sources are latched. ALERT response is latched when
     the source of the ALERT is asserted until cleared on Read of
     the corresponding status register (AFE_STATUS, SYS_STATUS, or
     result registers)
*/
enum class AlertLatch : uint8_t {
  DISABLED = 0x0,
  ENABLED  = 0x1,
};

/* Alert Mode (ALERT_MODE)
From datasheet:
ALERT mode select
0h = Interrupt mode
1h = Switch mode. This mode overrides any interrupt function
     (ALERT trigger is also disabled), and implements Hall switch function
     based off the *_THRX_ALRT settings. In the switch mode the
     corresponding X_HLT_EN, Y_HLT_EN, Z_HLT_EN need to be set.
*/
enum class AlertMode : uint8_t {
  INTERRUPT = 0x0,
  SWITCH    = 0x1,
};

/* Status Alert (STATUS_ALRT)
From datasheet:
Enable ALERT response when any flag in the AFE_STATUS or
SYS_STATUS registers are set
0h = ALERT is not asserted when any of the AFE_STATUS or
     SYS_STATUS bit is set
1h = ALERT output is asserted when any of the AFE_STATUS or
     SYS_STATUS bit is set
*/
enum class AlertStatus : uint8_t {
  DISABLED = 0x0,
  ENABLED  = 0x1,
};

/* Result Alert (RSLT_ALRT)
From datasheet:
Enable ALERT response when the configured set of conversions is
complete
0h = ALERT is not used to signal when the configured set of
     conversions are complete
1h = ALERT output is asserted when the configured set of
     conversions are complete
*/
enum class AlertResult : uint8_t {
  DISABLED = 0x0,
  ENABLED  = 0x1,
};

/* Alert Required Conversions (THRX_COUNT)
From datasheet:
Number of conversions above the HIGH threshold or below the LOW
threshold before the ALERT response is initiated
0h = 1-Conversion result
1h = 2-Conversion results
2h = 3-Conversion results
3h = 4-Conversion results
*/
enum class AlertRequiredConversions : uint8_t {
  x1 = 0x0,
  x2 = 0x1,
  x3 = 0x2,
  x4 = 0x3,
};

/* Threshold Alert (Z/Y/X_THRX_ALRT)
From datasheet:
Z/Y/X-Channel threshold ALERT enable
0h = ALERT is not used to signal when Z/Y/X-Axis magnetic thresholds
     are crossed
1h = ALERT output is asserted when Z/Y/X-Axis magnetic thresholds are
     crossed
*/
enum class AlertThreshold : uint8_t {
  DISABLED = 0x0,
  ENABLED  = 0x1,
};

/*  Threshold configuration ----------------------------------------------*/

/* Axis Threshold Config (Z/Y/X_THRX_CONFIG)
From datasheet:
HI_THRESHOLD Z/Y/X-Axis maximum magnetic field threshold. User input as 2's
             complement 8-bit binary number. The threshold in mT can be
             calculated as: (Z/Y/X_RANGE/128)*Z/Y/X_HI_THRESHOLD. Default to 98%
             of the full-scale
LO_THRESHOLD Z/Y/X-Axis minimum magnetic field threshold. User input as 2's
             complement 8-bit binary number. The threshold in mT can be
             calculated as: (Z/Y/X_RANGE/128)*Z/Y/X_LO_THRESHOLD. Default to
             -98% of the full-scale
*/

/* Temperature Threshold Config (T_THRX_CONFIG)
From datasheet:
HI_THRESHOLD Temperature maximum threshold. User input as 2's complement 8-
             bit binary number. Each LSB in this field corresponds to 4.267°C.
             Default value of 67h represents 172°C.
LO_THRESHOLD Temperature minimum threshold. User input as 2's complement 8-
             bit binary number. Each LSB in this field corresponds to 4.267°C.
             Default value of 32h represents -53°C.
*/

/*  Conversion status ----------------------------------------------*/

/* Conversion Ready (RDY)
From datasheet:
Conversion data buffer is ready.
0h = Conversion data not valid (result registers hold previous
     conversion value)
1h = Conversion data valid
*/
enum class ConversionReady : uint8_t {
  NOT_VALID = 0x0,
  VALID     = 0x1,
};

/* Channel Data Current (A/T/Z/Y/X)
From datasheet:
Angle/Magnitude, Temperature, Z/Y/X-Channel data from current conversion
0h = Data is not current
1h = Data is current
*/
enum class ChannelDataCurrent : uint8_t {
  NOT_CURRENT = 0x0,
  CURRENT     = 0x1,
};

/* Set Count (SET_COUNT)
From datasheet:
Rolling count of conversion data sets (3-bit)
*/

/* Alert Condition (ALRT_STATUS)
From datasheet:
State of ALERT response
0h = No ALERT conditions
1h = AFE status flag set
2h = SYS status flag set
3h = Flags set in both AFE and SYS status registers
*/
enum class AlertCondition : uint8_t {
  NONE        = 0x0,
  AFE         = 0x1,
  SYS         = 0x2,
  AFE_AND_SYS = 0x3,
};

/*  Conversion results ----------------------------------------------*/

/* Channel Results (X/Y/Z_CH_RESULT, TEMP_RESULT)
From datasheet:
X/Y/Z-Channel data conversion results (16-bit)
Temperature sensor data conversion results (16-bit)

Note: the magnetic results are 2's complement, B = (Z/Y/X_RANGE/32768)*RESULT.
      The Zephyr driver converts the temperature as
      T = 25°C + (TEMP_RESULT - 17522)/60.
*/

/*  AFE status ----------------------------------------------*/

/* Config Reset (CFG_RESET)
From datasheet:
Device power up status. This bit is reset when microcontroller reads
the AFE_STATUS register.
0h = Device reset has been acknowledged and cleared
1h = Device has experienced a hardware reset after a power down
     or brown-out
*/
enum class ConfigReset : uint8_t {
  ACKNOWLEDGED = 0x0,
  RESET        = 0x1,
};

/* AFE Diagnostic Status (SENS_STAT, TEMP_STAT, Z/Y/XHS_STAT, TRIM_STAT,
LDO_STAT)
From datasheet:
SENS_STAT  Analog front end sensor diagnostic status
TEMP_STAT  Temperature sensor diagnostic status
Z/Y/XHS_STAT Z/Y/X-Axis hall sensor diagnostic status
TRIM_STAT  Trim data error
LDO_STAT   LDO error
0h = No error detected
1h = Diagnostic test failed / error detected

Also used for the error flags in SYS_STATUS.
*/
enum class FaultStatus : uint8_t {
  OK    = 0x0,
  FAULT = 0x1,
};

/*  System status ----------------------------------------------*/

/* Alert Level (ALRT_LVL)
From datasheet:
Reflects the current state of the ALERT pin feed-back path
0h = The input ALERT logic level is low
1h = The input ALERT logic level is high
*/
enum class AlertLevel : uint8_t {
  LOGIC_LOW  = 0x0,
  LOGIC_HIGH = 0x1,
};

/* System Error Flags (ALRT_DRV, SDO_DRV, CRC_STAT, FRAME_STAT, VCC_OV, VCC_UV)
From datasheet:
ALRT_DRV   Each time the open drain ALERT signal is driven, the feedback
           circuit checks if the ALERT output goes Low. An error flag is
           generated at the ALRT_DRV bit if the output doesn't go Low.
SDO_DRV    The Logic value driven output on SDO was not the value of the SDO
           Pin Feed-back path when SDO is being driven by the device
CRC_STAT   Cyclic redundancy check error
FRAME_STAT Incorrect number of clocks in SPI frame
VCC_OV     VCC over-voltage detection in active or stand-by mode
VCC_UV     VCC under voltage detection in active or stand-by mode
0h = No error detected
1h = Error detected

Mapped to TMAG5170_Fault_Status.
*/

/* Operating State (OPERATING_STAT)
From datasheet:
Reports the status of operating mode
0h = Config state
1h = Standby state
2h = Active measure (Continuous Mode) state
3h = Active triggered mode state
4h = DCM active state
5h = DCM Sleep state
6h = Sleep state

Note: codes 4h-6h do not match OPERATING_MODE.
*/
enum class OperatingState : uint8_t {
  CONFIGURATION  = 0x0,
  STANDBY        = 0x1,
  ACTIVE_MEASURE = 0x2,
  ACTIVE_TRIGGER = 0x3,
  DCM_ACTIVE     = 0x4,
  DCM_SLEEP      = 0x5,
  SLEEP          = 0x6,
};

/* Threshold Crossing (TEMP_THX, Z/Y/XCH_THX)
From datasheet:
Temperature/Z/Y/X-Channel threshold crossing detected
0h = No threshold crossing detected
1h = Threshold crossing detected
*/
enum class ThresholdCrossing : uint8_t {
  NOT_DETECTED = 0x0,
  DETECTED     = 0x1,
};

/*  Test configuration ----------------------------------------------*/

/* Device Version (VER)
From datasheet:
Indicates the version of the device
0h = A1 rev
1h = A2 rev
2h = reserved
3h = reserved
*/
enum class Version : uint8_t {
  A1 = 0x0,
  A2 = 0x1,
};

/* CRC (CRC_DIS)
From datasheet:
Enable or disable CRC in SPI communication
0h = CRC enabled in SPI communication (Default)
1h = CRC disabled in SPI communication
*/
enum class CRC : uint8_t {
  ENABLED  = 0x0,
  DISABLED = 0x1,
};

/* Oscillator Count Control (OSC_CNT_CTL)
From datasheet:
Oscillator count control - starts, stops, and resets the counter driven
by the HFOSC or LFOSC oscillator to facilitate oscillator frequency
and integrity checks
0h = Reset OSC counter (default)
1h = Start OSC counter driven by HFOSC
2h = Start OSC counter driven by LFOSC
3h = Stop OSC counter
*/
enum class OscillatorCountControl : uint8_t {
  RESET       = 0x0,
  START_HFOSC = 0x1,
  START_LFOSC = 0x2,
  STOP        = 0x3,
};

/*  Oscillator monitor ----------------------------------------------*/

/* Oscillator Count (OSC_COUNT)
From datasheet:
Oscillator Counter. The number of selected oscillator clock cycles
that have been counted since Oscillator Counter was started. The
HFOSC and LFOSC clock roll-over the 16-bit counter once reaching
the max value.
*/

/*  Magnetic gain configuration ----------------------------------------------*/

/* Gain Axis (GAIN_SELECTION)
From datasheet:
Enables the selection of a particular Hall axis for amplitude
correction to get accurate angle measurement
0h = No axis is selected (Default)
1h = X-axis is selected
2h = Y-axis is selected
3h = Z-axis is selected
*/
enum class GainAxis : uint8_t {
  NONE = 0x0,
  X    = 0x1,
  Y    = 0x2,
  Z    = 0x3,
};

/* Gain Value (GAIN_VALUE)
From datasheet:
11-bit gain value determined by controller to adjust a particular
Hall axis value. The gain value is anywhere between 0 and 2. Gain is
calculated as 'user entered value/1024'.
*/

/*  Magnetic offset configuration ----------------------------------------------*/

/* Offset Selection (OFFSET_SELECTION)
From datasheet:
Enables the selection of a particular Hall axis for offset correction to
get accurate angle measurement. No axis is selected if ANGLE_EN = 00b.
0h = No axis is selected for offset correction (Default)
1h = Only OFFSET_VALUE1 is used for offset correction
2h = Only OFFSET_VALUE2 is used for offset correction
3h = Both OFFSET_VALUE1 and OFFSET_VALUE2 are used for offset correction

OFFSET_VALUE1 is applied to X when ANGLE_EN = 01b or 11b, and to Y when
ANGLE_EN = 10b.
OFFSET_VALUE2 is applied to Y when ANGLE_EN = 01b, and to Z when
ANGLE_EN = 10b or 11b.
*/
enum class OffsetSelection : uint8_t {
  NONE   = 0x0,
  VALUE1 = 0x1,
  VALUE2 = 0x2,
  BOTH   = 0x3,
};

/* Offset Values (OFFSET_VALUE1/OFFSET_VALUE2)
From datasheet:
7-bit, 2's complement offset value determined by controller to adjust a
particular Hall axis value. The range of possible offset valid entries
can be +/-64. The offset value is calculated from the user input as
the 7 LSB bits of a 11-bit range per SENSOR_CONFIG register
setting for the corresponding axis. Default offset value is 0.
*/

/*  Angle and magnitude results ----------------------------------------------*/

/* Angle Result (ANGLE_RESULT)
From datasheet:
Angle measurement result in degree. The data is displayed from 0 to
360 degree in 13 LSB bits. The 4 LSB bits allocated for fraction of an
angle in the format (xxxx/16).

Note: angle [deg] = (ANGLE_RESULT & 0x1FFF)/16.
*/

/* Magnitude Result (MAGNITUDE_RESULT)
From datasheet:
Resultant vector magnitude (during angle measurement) result. This
value should be constant during 360 degree measurements
*/


/*  Configuration -------------------------------------------------------*/

/*  SPI port and clock (TMAG5170 supports up to 10 MHz, SPI mode 0)  */
#define TMAG5170_SPI_PORT SPI
constexpr uint32_t SPI_CLOCK_HZ = 10000000;

/*  Width of the (active-low) ALERT trigger pulse [us]  */
constexpr uint32_t ALERT_PULSE_US = 1;
static_assert(
  ALERT_PULSE_US >= 1 && ALERT_PULSE_US <= 25,
  "ALERT_PULSE_US must be within tw_trigger, 1 to 25 us (see datasheet section 6.3)"
);


/*  Types ---------------------------------------------------------------*/
struct TMAG5170A1 {
  static constexpr Version VERSION = Version::A1;
  enum class Range : uint8_t {
    PM_50mT  = 0x0,
    PM_25mT  = 0x1,
    PM_100mT = 0x2
  };
};
struct TMAG5170A2 {
  static constexpr Version VERSION = Version::A2;
  enum class Range : uint8_t {
    PM_150mT = 0x0,
    PM_75mT  = 0x1,
    PM_300mT = 0x2
  };
};


/*  Per-sensor state (one instance per chip-select line)  */
template <typename Revision> struct Sensor {
  using Range = typename Revision::Range;

  uint8_t                   cs_pin;
  MagneticChannels          magnetic_channels;
  Range                     x_range;
  Range                     y_range;
  Range                     z_range;
  TriggerMode               trigger_mode;
  ConversionAveraging       conversion_averaging;
  TemperatureChannelEnable  temperature_channel;
  TemperatureCoefficient    temperature_coefficient;
  AngleCalculations         angle_calculations;
  TemperatureConversionRate temperature_conversion_rate;
  Sleeptime                 sleep_time;
  OperatingMode             operating_mode;
};

/*  One X/Y/Z reading. All three axes come from the same conversion set.  */
struct Reading {
  double  x_mT;
  double  y_mT;
  double  z_mT;
  uint8_t set_count; // CONV_STATUS.SET_COUNT of the set: advances by 1 (mod 8) per conversion
};


/*  Status --------------------------------------------------------------*/

// Result of a driver call
enum class Status : uint8_t {
  OK                    = 0x0,
  NO_RESPONSE           = 0x1, // RX frame all 0x00 or all 0xFF
  CRC_ERROR             = 0x2, // RX frame CRC does not match its contents
  WRONG_REVISION        = 0x3, // TEST_CONFIG.VER is not the revision the sensor was initialised as
  VERIFICATION_MISMATCH = 0x4, // Register read does not match register write
  MIXED_CONVERSIONS     = 0x5, // SET_COUNT changed between the X, Y and Z frames of one read
};

static inline const char *status_name(Status status) {
  switch (status) {
  case Status::OK:
    return "OK";
  case Status::NO_RESPONSE:
    return "NO_RESPONSE";
  case Status::CRC_ERROR:
    return "CRC_ERROR";
  case Status::WRONG_REVISION:
    return "WRONG_REVISION";
  case Status::VERIFICATION_MISMATCH:
    return "VERIFICATION_MISMATCH";
  case Status::MIXED_CONVERSIONS:
    return "MIXED_CONVERSIONS";
  }
  return "UNKNOWN";
}


/*  Functions prototypes ------------------------------------------------*/

// Configure a sensor and start it in sensor->operating_mode
template <typename Revision> Status init_sensor(Sensor<Revision> *sensor, uint8_t cs_pin);

// Read the X, Y and Z results [mT]. `reading` is only written when the returned status is OK.
template <typename Revision> Status get_magfield_mT(const Sensor<Revision> *sensor, Reading *reading);

// Configure the ALERT line shared by the sensors as an open-drain output, released
void init_alert(uint8_t alert_pin);

// Start a conversion on every sensor on the ALERT line that is in TRIGGER_MODE = ALERT
void trigger_alert(uint8_t alert_pin);

// Direct register control
void   write_register(uint8_t cs_pin, uint8_t reg, uint16_t data);
Status write_verify_register(uint8_t cs_pin, uint8_t reg, uint16_t data);
Status read_register(uint8_t cs_pin, uint8_t reg, uint16_t *data, uint8_t cmd);

} // namespace TMAG5170

#endif // TMAG5170_H

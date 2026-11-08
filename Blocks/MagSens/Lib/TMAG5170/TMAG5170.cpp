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

#ifndef MATLAB_MEX_FILE

/*  Includes ------------------------------------------------------------*/
#include "TMAG5170.h"
#include "TMAG5170_Hal.h"
#include "TMAG5170_Registers.h"

namespace TMAG5170 {

/*  Private functions prototypes ----------------------------------------*/
static TMAG5170::Status transfer(uint8_t cs_pin, uint8_t tx[4], uint8_t rx[4]);
static TMAG5170::Status read_frame(uint8_t cs_pin, uint8_t reg, uint8_t cmd, uint8_t rx[4]);

static constexpr bool
conversion_triggered_by_SPI(TMAG5170::OperatingMode operating_mode, TMAG5170::TriggerMode trigger_mode) {
  return operating_mode == TMAG5170::OperatingMode::ACTIVE_TRIGGER && trigger_mode == TMAG5170::TriggerMode::SPI;
}

#endif


/*  Functions definitions -----------------------------------------------*/
#ifndef MATLAB_MEX_FILE
/* Initialize sensor for SPI communication

Note that any fields not explicitly set will default to 0
*/
template <typename Revision> Status init_sensor(Sensor<Revision> *sensor, uint8_t cs_pin) {
  Status   status;
  uint16_t reg;

  // Initialize chip-select line and SPI port
  sensor->cs_pin = cs_pin;
  TMAG5170_HAL::init_spi(cs_pin);

  // Allow sensor power-up
  TMAG5170_HAL::delay_ms(1);

  // Return to configuration mode (in case of a warm restart).
  // Waking up from deep-sleep can take up to 500us.
  write_register(
    cs_pin,
    DEVICE_CONFIG::ADDRESS,
    set_field(DEVICE_CONFIG::OPERATING_MODE, OperatingMode::CONFIGURATION)
  );
  TMAG5170_HAL::delay_us(500);

  // Check that the sensor responds, and that the fitted device matches the configured version (A1 or A2)
  status = read_register(cs_pin, TEST_CONFIG::ADDRESS, &reg, TMAG5170_CMD_NONE);
  if (status != Status::OK) {
    return status;
  }
  if (get_field(TEST_CONFIG::VER, reg) != (uint16_t)Revision::VERSION) {
    return Status::WRONG_REVISION;
  }

  // Sensor configuration (SENSOR_CONFIG register):
  const uint16_t sensor_config = set_field(SENSOR_CONFIG::ANGLE_EN, sensor->angle_calculations) |
                                 set_field(SENSOR_CONFIG::SLEEPTIME, sensor->sleep_time) |
                                 set_field(SENSOR_CONFIG::MAG_CH_EN, sensor->magnetic_channels) |
                                 set_field(SENSOR_CONFIG::Z_RANGE, sensor->z_range) |
                                 set_field(SENSOR_CONFIG::Y_RANGE, sensor->y_range) |
                                 set_field(SENSOR_CONFIG::X_RANGE, sensor->x_range);

  status = write_verify_register(cs_pin, SENSOR_CONFIG::ADDRESS, sensor_config);
  if (status != Status::OK) {
    return status;
  }

  // System configuration (SYSTEM_CONFIG register):
  const uint16_t system_config = set_field(SYSTEM_CONFIG::TRIGGER_MODE, sensor->trigger_mode);
  status                       = write_verify_register(cs_pin, SYSTEM_CONFIG::ADDRESS, system_config);
  if (status != Status::OK) {
    return status;
  }

  // Alert configuration (ALERT_CONFIG register):
  status = write_verify_register(cs_pin, ALERT_CONFIG::ADDRESS, 0x0000);
  if (status != Status::OK) {
    return status;
  }

  // Device configuration (DEVICE_CONFIG register):
  const uint16_t device_config = set_field(DEVICE_CONFIG::CONV_AVG, sensor->conversion_averaging) |
                                 set_field(DEVICE_CONFIG::MAG_TEMPCO, sensor->temperature_coefficient) |
                                 set_field(DEVICE_CONFIG::OPERATING_MODE, sensor->operating_mode) |
                                 set_field(DEVICE_CONFIG::T_CH_EN, sensor->temperature_channel) |
                                 set_field(DEVICE_CONFIG::T_RATE, sensor->temperature_conversion_rate);
  write_register(cs_pin, DEVICE_CONFIG::ADDRESS, device_config);

  // In SPI trigger mode, each readout triggers the next conversion.
  // Without an initial trigger, the first sensor read would come back without sensible values.
  if (conversion_triggered_by_SPI(sensor->operating_mode, sensor->trigger_mode)) {
    status = read_register(cs_pin, SYS_STATUS::ADDRESS, &reg, TMAG5170_CMD_TRIGGER_CONVERSION);
    if (status != Status::OK) {
      return status;
    }
    TMAG5170_HAL::delay_ms(5);
  }

  return Status::OK;
}

template <typename Revision> Status get_magfield_mT(const Sensor<Revision> *sensor, Reading *reading) {
  Status  status;
  uint8_t rx_x[4], rx_y[4], rx_z[4];

  // In active trigger mode, the last read triggers the next conversion,
  // which then runs during the remainder of the sample period
  const uint8_t final_cmd = conversion_triggered_by_SPI(sensor->operating_mode, sensor->trigger_mode)
                              ? TMAG5170_CMD_TRIGGER_CONVERSION
                              : TMAG5170_CMD_NONE;

  // Read conversion results
  status = read_frame(sensor->cs_pin, X_CH_RESULT::ADDRESS, TMAG5170_CMD_NONE, rx_x);
  if (status != Status::OK) {
    return status;
  }
  status = read_frame(sensor->cs_pin, Y_CH_RESULT::ADDRESS, TMAG5170_CMD_NONE, rx_y);
  if (status != Status::OK) {
    return status;
  }
  status = read_frame(sensor->cs_pin, Z_CH_RESULT::ADDRESS, final_cmd, rx_z);
  if (status != Status::OK) {
    return status;
  }

  // Every frame carries SET_COUNT. If it changed between frames, a conversion finished mid-read
  // and the results come from different conversion sets.
  const uint8_t set_count = rx_set_count(rx_x);
  if (rx_set_count(rx_y) != set_count || rx_set_count(rx_z) != set_count) {
    return Status::MIXED_CONVERSIONS;
  }

  // Convert to [mT] units
  reading->x_mT      = code_mT(rx_data(rx_x), sensor->x_range);
  reading->y_mT      = code_mT(rx_data(rx_y), sensor->y_range);
  reading->z_mT      = code_mT(rx_data(rx_z), sensor->z_range);
  reading->set_count = set_count;

  return Status::OK;
}

template Status init_sensor(Sensor<TMAG5170A1> *sensor, uint8_t cs_pin);
template Status init_sensor(Sensor<TMAG5170A2> *sensor, uint8_t cs_pin);
template Status get_magfield_mT(const Sensor<TMAG5170A1> *sensor, Reading *reading);
template Status get_magfield_mT(const Sensor<TMAG5170A2> *sensor, Reading *reading);


/*  The sensors' ALERT outputs are open-drain on one line with a pull-up, so the line is only ever pulled
 *  low or released. Driving it high would fight a sensor that is pulling it low.
 */
void init_alert(uint8_t alert_pin) { TMAG5170_HAL::init_open_drain(alert_pin); }


void trigger_alert(uint8_t alert_pin) {
  TMAG5170_HAL::write_pin(alert_pin, false); // pull low
  TMAG5170_HAL::delay_us(ALERT_PULSE_US);
  TMAG5170_HAL::write_pin(alert_pin, true);  // release
}


/*  Note: the response to a write is not checked;
    use TMAG5170_WriteVerifyRegister() to read the register back.
 */
void write_register(uint8_t cs_pin, uint8_t reg, uint16_t data) {
  uint8_t tx[4] = {(uint8_t)(reg & 0x7F), (uint8_t)(data >> 8), (uint8_t)(data & 0xFF), 0x00};
  uint8_t rx[4];

  transfer(cs_pin, tx, rx);
}


/*  Write a register and read it back. Returns the read's status if it fails,
 *  VERIFICATION_MISMATCH if the readback differs from `data`, else OK.
 */
Status write_verify_register(uint8_t cs_pin, uint8_t reg, uint16_t data) {
  Status   status;
  uint16_t readback;

  write_register(cs_pin, reg, data);

  status = read_register(cs_pin, reg, &readback, TMAG5170_CMD_NONE);
  if (status != Status::OK) {
    return status;
  }

  return readback == data ? Status::OK : Status::VERIFICATION_MISMATCH;
}


Status read_register(uint8_t cs_pin, uint8_t reg, uint16_t *data, uint8_t cmd) {
  uint8_t rx[4];

  const Status status = read_frame(cs_pin, reg, cmd, rx);
  if (status != Status::OK) {
    return status;
  }

  *data = rx_data(rx);

  return Status::OK;
}

/*  Private functions definitions ---------------------------------------*/

/*  Read one register. Returns the whole RX frame, whose status bits get_magfield_mT needs.  */
static Status read_frame(uint8_t cs_pin, uint8_t reg, uint8_t cmd, uint8_t rx[4]) {
  uint8_t tx[4] = {(uint8_t)(TMAG5170_READ_FLAG | (reg & 0x7F)), 0x00, 0x00, (uint8_t)((cmd & 0x0F) << 4)};

  return transfer(cs_pin, tx, rx);
}

/*  Transfer one 32-bit frame. Adds the CRC to the TX frame and checks the RX frame
 *  (OK, NO_RESPONSE or CRC_ERROR).
 *
 *  TX frame: [31] R/W | [30:24] address | [23:8] data | [7:4] CMD | [3:0] CRC
 *  RX frame: [31:24] status | [23:8] data | [7:4] status | [3:0] CRC
 */
static Status transfer(uint8_t cs_pin, uint8_t tx[4], uint8_t rx[4]) {
  tx[3] = (tx[3] & 0xF0) | CRC4(tx);

  TMAG5170_HAL::transfer(cs_pin, tx, rx);

  return check_rx_frame(rx);
}

} // namespace TMAG5170

#endif

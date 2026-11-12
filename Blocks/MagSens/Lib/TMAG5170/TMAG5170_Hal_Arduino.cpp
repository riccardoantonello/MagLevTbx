/*
 * TMAG5170 hardware abstraction layer for Teensy/Arduino.
 */

#ifndef MATLAB_MEX_FILE

/* Includes ------------------------------------------------------------*/
#include "TMAG5170.h"
#include "TMAG5170_Hal.h"

#include <Arduino.h>
#include <SPI.h>


/* Functions definitions -----------------------------------------------*/

namespace TMAG5170_HAL {

void init_spi(uint8_t cs_pin) {
  pinMode(cs_pin, OUTPUT);
  digitalWrite(cs_pin, HIGH);
  TMAG5170_SPI_PORT.begin();
}


void transfer(uint8_t cs_pin, const uint8_t tx[4], uint8_t rx[4]) {
  TMAG5170_SPI_PORT.beginTransaction(SPISettings(TMAG5170::SPI_CLOCK_HZ, MSBFIRST, SPI_MODE0));
  digitalWrite(cs_pin, LOW);
  for (uint8_t i = 0; i < 4; ++i) {
    rx[i] = TMAG5170_SPI_PORT.transfer(tx[i]);
  }
  digitalWrite(cs_pin, HIGH);
  TMAG5170_SPI_PORT.endTransaction();
}


// Until the pin is an output, digitalWrite() only selects its pull resistor, so the mode is set first
void init_open_drain(uint8_t pin) {
  pinMode(pin, OUTPUT_OPENDRAIN);
  digitalWrite(pin, HIGH);
}


void write_pin(uint8_t pin, bool high) { digitalWrite(pin, high ? HIGH : LOW); }


void delay_ms(uint32_t ms) { delay(ms); }


void delay_us(uint32_t us) { delayMicroseconds(us); }

} // namespace TMAG5170_HAL

#endif

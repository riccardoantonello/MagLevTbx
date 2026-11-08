/*
 * TMAG5170 hardware abstraction layer.
 */

#ifndef TMAG5170_HAL_H
#define TMAG5170_HAL_H

#include <stdint.h>

namespace TMAG5170_HAL {

// Configure the chip-select line (deasserted) and start the SPI port
void init_spi(uint8_t cs_pin);

// Transfer one 32-bit frame (MSB first, SPI mode 0) with the chip-select line asserted
void transfer(uint8_t cs_pin, const uint8_t tx[4], uint8_t rx[4]);

// Configure an open-drain output pin, released (high impedance)
void init_open_drain(uint8_t pin);

// Set a digital output pin high or low. On an open-drain pin, high releases the line.
void write_pin(uint8_t pin, bool high);

// Blocking delays
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

} // namespace TMAG5170_HAL


#endif // TMAG5170_HAL_H

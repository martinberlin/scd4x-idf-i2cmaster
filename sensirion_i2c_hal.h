#ifndef SENSIRION_I2C_HAL_H
#define SENSIRION_I2C_HAL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Select the current i2c bus by index.
 * Optional on single-bus setups.
 */
int16_t sensirion_i2c_hal_select_bus(uint8_t bus_idx);

/**
 * Initialize I2C HAL.
 *
 * For ESP32-C5 in this repo: this will initialize the shared i2c_bus component.
 * It is safe to call multiple times (i2c_bus_init() should be idempotent).
 */
int16_t sensirion_i2c_hal_init(int gpio_sda, int gpio_scl);

/**
 * Free resources initialized by sensirion_i2c_hal_init().
 *
 * With the shared-bus model, this is a no-op and returns 0.
 */
int16_t sensirion_i2c_hal_free(void);

int16_t sensirion_i2c_hal_read(uint8_t address, uint8_t* data, uint16_t count);
int16_t sensirion_i2c_hal_write(uint8_t address, const uint8_t* data, uint16_t count);

void sensirion_i2c_hal_sleep_usec(uint32_t useconds);

#ifdef __cplusplus
}
#endif

#endif /* SENSIRION_I2C_HAL_H */
/*
 * ESP-IDF (ESP32-C5) I2C HAL for Sensirion drivers using driver_ng (i2c_master)
 * and the shared i2c_bus component.
 *
 * This avoids legacy driver/i2c.h APIs (i2c_driver_install, i2c_cmd_link_create, etc.)
 * to prevent driver conflicts on newer targets.
 */

#include "sensirion_i2c_hal.h"

#include "sensirion_common.h"
#include "sensirion_config.h"

#include "i2c_bus.h"
#include "driver/i2c_master.h"

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifndef I2C_FREQ
// sensirion_config.h usually provides I2C_FREQ, but keep a fallback
#define I2C_FREQ 100000
#endif

#ifndef SENSIRION_I2C_TIMEOUT_MS
#define SENSIRION_I2C_TIMEOUT_MS 1000
#endif

static inline i2c_master_dev_handle_t sensirion_dev(uint8_t addr7)
{
    return i2c_bus_get_dev(addr7, I2C_FREQ);
}

int16_t sensirion_i2c_hal_select_bus(uint8_t bus_idx)
{
    (void)bus_idx;
    return NOT_IMPLEMENTED_ERROR;
}

int16_t sensirion_i2c_hal_init(int gpio_sda, int gpio_scl)
{
    // Initialize shared bus once; safe if called multiple times
    esp_err_t err = i2c_bus_init(gpio_sda, gpio_scl);
    return (err == ESP_OK) ? 0 : (int16_t)err;
}

int16_t sensirion_i2c_hal_free(void)
{
    // Shared bus lifetime is owned by the application; no-op here
    return 0;
}

int16_t sensirion_i2c_hal_read(uint8_t address, uint8_t* data, uint16_t count)
{
    if (!data || count == 0) return ESP_ERR_INVALID_ARG;

    i2c_master_dev_handle_t dev = sensirion_dev(address);
    if (!dev) return ESP_ERR_INVALID_STATE;

    esp_err_t err = i2c_master_receive(dev, data, count, SENSIRION_I2C_TIMEOUT_MS);
    return (err == ESP_OK) ? 0 : (int16_t)err;
}

int16_t sensirion_i2c_hal_write(uint8_t address, const uint8_t* data, uint16_t count)
{
    if (!data || count == 0) return ESP_ERR_INVALID_ARG;

    i2c_master_dev_handle_t dev = sensirion_dev(address);
    if (!dev) return ESP_ERR_INVALID_STATE;

    esp_err_t err = i2c_master_transmit(dev, data, count, SENSIRION_I2C_TIMEOUT_MS);
    return (err == ESP_OK) ? 0 : (int16_t)err;
}

void sensirion_i2c_hal_sleep_usec(uint32_t useconds)
{
    // <10ms precision is fine for Sensirion
    uint32_t msec = useconds / 1000;
    if (useconds % 1000) {
        msec++;
    }
    vTaskDelay(pdMS_TO_TICKS(msec));
}
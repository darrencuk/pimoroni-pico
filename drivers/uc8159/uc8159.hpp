#pragma once

#include <initializer_list>

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "common/pimoroni_common.hpp"
#include "common/pimoroni_bus.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"

namespace pimoroni {

  class UC8159 : public DisplayDriver {
    //--------------------------------------------------
    // Variables
    //--------------------------------------------------
  private:

    // highest possible resolution is 160x296 which at 1 bit per pixel
    // requires 5920 bytes of frame buffer
    //uint8_t frame_buffer[5920] = {0};
    uint8_t *frame_buffer;

    spi_inst_t *spi = PIMORONI_SPI_DEFAULT_INSTANCE;

    // interface pins with our standard defaults where appropriate
    uint CS     = SPI_BG_FRONT_CS;
    uint DC     = 27;
    uint SCK    = SPI_DEFAULT_SCK;
    uint MOSI   = SPI_DEFAULT_MOSI;
    uint BUSY   = 26;
    uint RESET  = 25;

  public:
    enum colour : uint8_t {
      BLACK = 0,
      WHITE = 1,
      GREEN = 2,
      BLUE = 3,
      RED = 4,
      YELLOW = 5,
      ORANGE = 6,
      CLEAN = 7
    };

    UC8159(uint16_t width, uint16_t height) : UC8159(width, height, {PIMORONI_SPI_DEFAULT_INSTANCE, SPI_BG_FRONT_CS, SPI_DEFAULT_SCK, SPI_DEFAULT_MOSI, PIN_UNUSED, 27, PIN_UNUSED}) {};

    UC8159(uint16_t width, uint16_t height, SPIPins pins, uint busy=26, uint reset=25) :
      DisplayDriver(width, height, ROTATE_0),
      spi(pins.spi),
      CS(pins.cs), DC(pins.dc), SCK(pins.sck), MOSI(pins.mosi), BUSY(busy), RESET(reset) {
        init();
      }


    //--------------------------------------------------
    // Methods
    //--------------------------------------------------
  public:
    void busy_wait();
    void reset();
    void power_off();
  
    bool is_busy() override;
    void update(PicoGraphics *graphics) override;

  private:
    void init();
    void setup();
    void update(const void *data, bool blocking = true);
    void command(uint8_t reg, size_t len, const uint8_t *data);
    void command(uint8_t reg, std::initializer_list<uint8_t> values);
    void command(uint8_t reg, const uint8_t data) {command(reg, 0, &data);};
    void command(uint8_t reg) {command(reg, 0, nullptr);};
    void data(size_t len, const uint8_t *data);
  };

}

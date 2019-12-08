#pragma once

#include "../../dep/serial/include/serial/serial.hpp"

namespace arduino {
    struct Device {
        serial::Device serial_device;
        double tick_begin = 0.0;
        unsigned int rsize, tsize;
        unsigned char *rdata_handle, tdata_handle;
        const unsigned char is_valid, tick, key;
        void (*on_update)(Device &);
    };

    Device connect(const unsigned int baud, const unsigned char key);
} // namespace arduino

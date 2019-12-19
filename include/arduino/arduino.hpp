#pragma once

#include "../../dep/serial/include/serial/serial.hpp"

namespace arduino {
    static constexpr unsigned int TICKS_PER_SECOND = 50;
    static constexpr double TICK_INTERVAL = 1.0 / TICKS_PER_SECOND;
    static constexpr unsigned int HANDSHAKE_ATTEMPTS = 10;

    struct Device {
        serial::Device _serial_device;
        double _tick_begin = 0.0;
        unsigned int rsize, tsize;
        unsigned char *rdata_handle, *_rdata_buffer, *tdata_handle, *_tdata_buffer;
        unsigned char is_valid, tick, key;
        void (*on_update)(Device &);
    };

    void connect(Device &device, const unsigned int rsize, const unsigned int tsize, const unsigned int baud,
                 const unsigned char key);
    static inline Device connect(const unsigned int rsize, const unsigned int tsize, const unsigned int baud,
                                 const unsigned char key) {
        Device device = {};
        connect(device, rsize, tsize, baud, key);
        return device;
    }
    void disconnect(Device &device);
    void update(Device &device);
} // namespace arduino

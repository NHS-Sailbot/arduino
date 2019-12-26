#pragma once

#include <HENRY/serial.hpp>

namespace HENRY {
    static constexpr unsigned int TICKS_PER_SECOND = 100;
    static constexpr double TICK_INTERVAL = 1.0 / TICKS_PER_SECOND;
    static constexpr unsigned int HANDSHAKE_ATTEMPTS = 10;

    struct Arduino {
        HENRY::Serial _serial_device;
        double _tick_begin = 0.0;
        unsigned int _rsize, _tsize;
        unsigned char *rdata_handle, *_rdata_buffer, *tdata_handle, *_tdata_buffer;
        unsigned char is_valid, tick, m_key;
        void (*on_update)(Arduino &);

        Arduino() = default;
        Arduino(const unsigned int rsize, const unsigned int tsize, const unsigned int baud, const unsigned char key);
        ~Arduino();

        void connect(const unsigned int rsize, const unsigned int tsize, const unsigned int baud, const unsigned char key);
        void disconnect();
        void update();
    };
} // namespace HENRY

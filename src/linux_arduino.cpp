#include <HENRY/arduino.hpp>

#define DEBUG_ENABLE_TIMING
#define DEBUG_ENABLE_LOGGING
#include <debug/debug.hpp>

namespace HENRY {
    static void default_update(Arduino &) {}

    static bool open_device(Arduino &ard, const char *const filepath, const unsigned int baud) {
        DEBUG_BEGIN_FUNC_PROFILE;

        if (ard._serial_device.open(filepath, baud)) {
            debug::log::message("Attempting to shake hands with %s...", filepath);

            for (unsigned int i = 0; i < HANDSHAKE_ATTEMPTS; ++i) {
                ard._serial_device.write(ard._tdata_buffer, ard._tsize);
                debug::timer::sleep(TICK_INTERVAL);
                ard._serial_device.read(ard._rdata_buffer, ard._rsize);
                debug::timer::sleep(TICK_INTERVAL);

                for (unsigned int i = 0; i < ard._rsize; ++i)
                    debug::log::info("%0*x ", 2, ard._rdata_buffer[i]);
                debug::log::info(": %0*x\n", 2, ard.m_key);

                if (ard._rdata_buffer[0] == ard.m_key && ard._rdata_buffer[ard._rsize - 1] == ard.m_key) {
                    debug::log::success("Opened arduino '%s' at %d baud, using key '%x'.", filepath, baud, ard.m_key);
                    ard._tick_begin = debug::timer::now();
                    ard.is_valid = true;
                    return true;
                }
            }
            ard._serial_device.close();
        }
        ard.is_valid = false;
        return false;
    }

    Arduino::Arduino(const unsigned int rsize, const unsigned int tsize, const unsigned int baud, const unsigned char key) {
        connect(rsize, tsize, baud, key);
    }
    Arduino::~Arduino() { disconnect(); }

    void Arduino::connect(const unsigned int rsize, const unsigned int tsize, const unsigned int baud,
                          const unsigned char key) {
        DEBUG_BEGIN_FUNC_PROFILE;

        if (!on_update) on_update = default_update;
        _rsize = rsize + 2, _tsize = tsize + 2, m_key = key;
        _rdata_buffer = new unsigned char[_rsize];
        _tdata_buffer = new unsigned char[_tsize];
        for (tdata_handle = _tdata_buffer + 1; tdata_handle < _tdata_buffer - 1; ++tdata_handle)
            *tdata_handle = 0;
        rdata_handle = _rdata_buffer + 1, tdata_handle = _tdata_buffer + 1;

        char filepath[] = "/dev/ttyACM0";
        for (; filepath[11] < '1' + 1; ++filepath[11])
            if (open_device(*this, filepath, baud)) return;
        filepath[9] = 'M', filepath[10] = 'A', filepath[11] = '0';
        for (; filepath[11] < '1' + 1; ++filepath[11])
            if (open_device(*this, filepath, baud)) return;
        filepath[8] = 'U', filepath[9] = 'S', filepath[10] = 'B', filepath[11] = '0';
        for (; filepath[11] < '5' + 1; ++filepath[11])
            if (open_device(*this, filepath, baud)) return;
        filepath[8] = 'S', filepath[9] = '0', filepath[10] = '\0';
        for (; filepath[9] < '9' + 1; ++filepath[9])
            if (open_device(*this, filepath, baud)) return;

        debug::log::error("Failed to find arduino at %d baud with the key '%x'", baud, key);
    }

    void Arduino::disconnect() {
        DEBUG_BEGIN_FUNC_PROFILE;

        if (is_valid) {
            if (_rdata_buffer) delete _rdata_buffer;
            if (_tdata_buffer) delete _tdata_buffer;
            rdata_handle = nullptr, _rdata_buffer = nullptr;
            tdata_handle = nullptr, _tdata_buffer = nullptr;

            is_valid = false;

            _serial_device.close();
        }
    }

    void Arduino::update() {
        const double current_time = debug::timer::now();
        if (current_time - _tick_begin > TICK_INTERVAL) {
            DEBUG_BEGIN_FUNC_PROFILE;

            switch (tick) {
            case 0:
                _serial_device.write(_tdata_buffer, _tsize);
                tick = 1;
                break;
            case 1:
                _serial_device.read(_rdata_buffer, _rsize);
                if (_rdata_buffer[0] == m_key && _rdata_buffer[_rsize - 1] == m_key) {
                    _tick_begin = debug::timer::now();
                    is_valid = true;
                } else {
                    debug::log::error("failed to find the key (%x) when reading the device.", m_key);
                    is_valid = false;
                }
                DEBUG_BEGIN_PROFILE(User_defined_update_function);
                on_update(*this);
                DEBUG_END_PROFILE(User_defined_update_function);
                tick = 0;
                break;
            }

            _tick_begin += TICK_INTERVAL;
        }
    }
} // namespace HENRY

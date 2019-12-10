#include <arduino/arduino.hpp>
#include <debug/debug.hpp>

namespace arduino {
    static void default_update(Device &) {}

    static bool open_device(Device &device, const char *const filepath, const unsigned int baud) {
        DEBUG_BEGIN_FUNC_PROFILE;
        if (serial::open(device.serial_device, filepath, baud)) {
            debug::log::message("Attempting to shake hands with %s...", filepath);

            for (unsigned int i = 0; i < HANDSHAKE_ATTEMPTS; ++i) {
                serial::write(device.serial_device, device.tdata_buffer, device.tsize);
                debug::timer::sleep(TICK_INTERVAL);
                serial::read(device.serial_device, device.rdata_buffer, device.rsize);

                for (unsigned int i = 0; i < device.rsize; ++i)
                    debug::log::info("%0*x ", 2, device.rdata_buffer[i]);
                debug::log::info(": %0*x\n", 2, device.key);

                if (device.rdata_buffer[0] == device.key && device.rdata_buffer[device.rsize - 1] == device.key) {
                    debug::log::success("Opened device %s at baudrate %d, using key %x.", filepath, baud, device.key);
                    device.tick_begin = debug::timer::now();
                    device.is_valid = true;
                    return true;
                }
                debug::timer::sleep(TICK_INTERVAL);
            }
            serial::close(device.serial_device);
        }
        device.is_valid = false;
        return false;
    }

    void connect(Device &device, const unsigned int rsize, const unsigned int tsize, const unsigned int baud,
                 const unsigned char key) {
        DEBUG_BEGIN_FUNC_PROFILE;
        if (!device.on_update) device.on_update = default_update;
        device.rsize = rsize + 2, device.tsize = tsize + 2, device.key = key;
        device.rdata_buffer = new unsigned char[device.rsize];
        device.tdata_buffer = new unsigned char[device.tsize];

        char filepath[] = "/dev/ttyACM0";
        for (; filepath[11] < '1' + 1; ++filepath[11])
            if (open_device(device, filepath, baud)) return;
        filepath[9] = 'M', filepath[10] = 'A', filepath[11] = '0';
        for (; filepath[11] < '1' + 1; ++filepath[11])
            if (open_device(device, filepath, baud)) return;
        filepath[8] = 'U', filepath[9] = 'S', filepath[10] = 'B', filepath[11] = '0';
        for (; filepath[11] < '5' + 1; ++filepath[11])
            if (open_device(device, filepath, baud)) return;
        filepath[8] = 'S', filepath[9] = '0', filepath[10] = '\0';
        for (; filepath[9] < '9' + 1; ++filepath[9])
            if (open_device(device, filepath, baud)) return;
    }
} // namespace arduino

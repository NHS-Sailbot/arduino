#pragma once

namespace arduino {
    struct Device {
        bool is_open;
    };

    Device connect(const unsigned int baud, const unsigned char key);
} // namespace arduino

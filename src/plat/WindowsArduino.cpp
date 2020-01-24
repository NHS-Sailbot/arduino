#include <Henry/Arduino.hpp>

namespace Henry { namespace Platform {
    unsigned int getDeviceCount() { return 0; }
    char *getDevicePathByIndex(const unsigned int index) {
        static char tPath[] = "";
        return tPath;
    }
}} // namespace Henry::Platform

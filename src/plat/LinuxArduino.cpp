#include <Henry/Arduino.hpp>

namespace Henry { namespace Platform {
	unsigned int getDeviceCount() { return 20; }
	char *getDevicePathByIndex(const unsigned int index) {
		static char tPath[] = "/dev/ttyACM0";
		switch (index) {
		case 0:
		case 1: tPath[8] = 'A', tPath[9] = 'C', tPath[10] = 'M', tPath[11] = '0' + index, tPath[12] = '\0'; break;
		case 2:
		case 3: tPath[8] = 'A', tPath[9] = 'M', tPath[10] = 'A', tPath[11] = '0' + index - 2, tPath[12] = '\0'; break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9: tPath[8] = 'U', tPath[9] = 'S', tPath[10] = 'B', tPath[11] = '0' + index - 4, tPath[12] = '\0'; break;
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19: tPath[8] = 'S', tPath[9] = '0' + index - 10, tPath[10] = '\0'; break;
		default: break;
		}
		return tPath;
	}
}} // namespace Henry::Platform

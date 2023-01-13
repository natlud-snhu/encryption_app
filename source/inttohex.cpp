#include "inttohex.h"
#include <sstream>
#include <string>
#include <iomanip>

std::string intToHex(unsigned int num) {
	std::ostringstream ss;
	ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << num;
	return ss.str();
};
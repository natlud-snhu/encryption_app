#include "session.h"
#include <array>

bool IDStruct::operator==(const IDStruct& rh) {
	return (this->num == rh.num);
};
bool IDStruct::operator==(const std::uint32_t& rh) {
	return (this->num == rh);
};
IDStruct::IDStruct(std::uint32_t number) { //ID constructor
	this->num = number;
	this->chars = { unsigned char((number >> 24) & 0xFF), unsigned char((number >> 16) & 0xFF), unsigned char((number >> 8) & 0xFF), unsigned char(number & 0xFF) };
};
IDStruct::IDStruct() {};

std::array<unsigned char, 4> Session::generateSeed() {
	std::array<unsigned char, 4> seed = { 0x00, 0x00, 0x00, 0x00 };
	//fills each of the four seed bytes
	std::uint32_t random = this->mtwister();
	seed.at(0) = random >> 24; //random number between 0 and 255
	seed.at(1) = 1 + (((random >> 16) & 0xFF) % 8); //random number between 1-8
	seed.at(2) = ((random >> 6) & 0xFF) % seed.at(1); //random number within [1]'s range
	seed.at(3) = (random & 0xFF) % 4; //random number between 0-3
	return seed;
};

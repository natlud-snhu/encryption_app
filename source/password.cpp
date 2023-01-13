#include "password.h"
#include <string>
#include <random>
#include <stdint.h>
#include <array>
#include <math.h>

const char* alphanum[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

std::string createPassword() {
	std::string output = "0000000000000000"; //password length is 16
	std::random_device rd; //random number generator
	std::uniform_int_distribution<int> distrib( 0, 61 );
	for (int i = 0; i < 16; i++) { //iterate through 16 chars
		output[i] = *alphanum[distrib(rd)]; //set char to random alphanum
	};
	return output;
};
unsigned int convertPassword(std::string pass) {
	unsigned int convert = 0;
	for (int i = 0; i < pass.size(); i++) { //converts string character by character to int
		//convert += int(pass.at(i)) << (8 * i);
		convert += (unsigned int(pass.at(i) % 10) * unsigned int(pow(10, i)));
	};
	return convert;
};

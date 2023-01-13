#pragma once
#include <string>
#include <random>
#include <array>
#include <vector>

struct IDStruct {
	std::uint32_t num; //numerical representation
	std::array<unsigned char, 4> chars; //char array
	IDStruct(std::uint32_t number);
	IDStruct();
	bool operator==( const IDStruct& rh);
	bool operator==(const std::uint32_t& rh);
};

class Session {
protected:
	std::mt19937 mtwister;//internal random machine
	IDStruct ID; //unique id given to identify doneness
	std::string password; //string version of the password
	unsigned int password_int; //integer version of password
	std::vector<std::array<unsigned char, 4>> ID_seeds = {}; //ID seeds
	std::vector<std::array<unsigned char, 4>> text_seeds = {}; //plain text seeds
	std::vector<unsigned char> encrypted_text;
	std::string plain_text;
	std::array<unsigned char, 4> generateSeed(); //uses mt19937 to create seeds
public:
	virtual void printState() {}; //prints state of current session
	virtual void saveToFile(std::string file_path) {}; //saves contents of session to file
};
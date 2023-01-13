#include "password.h"
#include "session.h"
#include "decryptionsession.h"
#include "inttohex.h"
#include <string>
#include <array>
#include <vector>
#include <stdint.h>
#include <random>
#include <iostream>
#include <fstream>

DecryptionSession::DecryptionSession() {};
DecryptionSession::DecryptionSession(std::string file) {
	readFromFile(file);
};
void DecryptionSession::tryDecryption(std::string pass) {
	this->password = pass;
	//convert password to int
	this->password_int = convertPassword(pass);
	//startup mt19937 machine
	this->mtwister = std::mt19937(password_int);
	this->ID = IDStruct(mtwister());
	//reset arrays to default
	this->ID_seeds = {};
	this->text_seeds = {};
	this->plain_text = {};

	//generate seed for ID
	for (int i = 0; i < this->ID.chars.size(); i++) {
		this->ID_seeds.push_back(generateSeed());
	};
	//count how many seeds are required
	countSeeds();
	//attempt to decrypt text
	decryptAll();
};
DecryptionSession::DecryptionSession(std::string file, std::string pass) {
	this->password = pass;
	//convert password to int
	this->password_int = convertPassword(pass);
	//startup mt19937 machine
	this->mtwister = std::mt19937(password_int);
	this->ID = IDStruct(mtwister());
	
	//generate seed for ID
	for (int i = 0; i < this->ID.chars.size(); i++) {
		this->ID_seeds.push_back(generateSeed());
	};
	
	//get data from file
	readFromFile(file);
	//count how many seeds are required
	
	countSeeds();
	//attempt to decrypt text
	decryptAll();
	
};
void DecryptionSession::countSeeds() {
	//count double words
	unsigned short size = (this->encrypted_text.size()) / 4;
	unsigned short filled = 0;
	//count length of ID seed garble
	for (int i = 0; i < this->ID_seeds.size(); i++) {
		filled += this->ID_seeds.at(i)[1];
	}
	//determine text seeds
	unsigned short x = 0;
	while (filled < size) { //until all seeds are generated, continue
		this->text_seeds.push_back(generateSeed());
		filled += this->text_seeds.at(x)[1];
		x++;
	}
};
void DecryptionSession::decryptAll() {
	unsigned short index = 0;
	//decrypt text
	for (int i = 0; i < this->text_seeds.size(); i++) {
		unsigned short letter_spot = this->text_seeds.at(i)[2];//% this->text_seeds.at(i)[1];
		letter_spot = (letter_spot * 4) + this->text_seeds.at(i)[3];
		int character = int(this->encrypted_text.at(index + letter_spot)) - int(this->text_seeds.at(i)[0]);
		if (character < 0) {
			character = 256 + character;
		}
		char letter = (this->encrypted_text.at(index + letter_spot) - this->text_seeds.at(i)[0]);
		this->plain_text += letter;
		index += (this->text_seeds.at(i)[1] * 4);
	};
	try {
		std:uint32_t ID = 0;
		for (int i = 0; i < this->ID_seeds.size(); i++) {
			unsigned short letter_spot = this->ID_seeds.at(i)[2];
			letter_spot = (letter_spot * 4) + this->ID_seeds.at(i)[3];
			int character = int(this->encrypted_text.at(index + letter_spot)) - int(this->ID_seeds.at(i)[0]);
			if (character < 0) {
				character = 256 + character;
			}
			ID += (unsigned int(character) << (24 - (i * 8))); //perform bit manipulation to reconstruct ID
			index += (this->ID_seeds.at(i)[1] * 4);
		};
		if (!(this->ID == ID)) { //IDs must be equal for decryption to have been successful
			this->decrypted = false;
			return;
		};
	} catch (std::exception a) {
		this->decrypted = false;
		return;
	};
	this->decrypted = true;
};
void DecryptionSession::printState() {
	std::cout << "ID: " << this->ID.num << "\n";
	std::cout << "ID Array: {" << intToHex(unsigned int(this->ID.chars[0])) << ", " << intToHex(unsigned int(this->ID.chars[1])) << ", " << intToHex(unsigned int(this->ID.chars[2])) << ", " << intToHex(unsigned int(this->ID.chars[3])) << "}\n";
	std::cout << "Password: " << this->password << "\n";
	std::cout << "Int Password: " << this->password_int << "\n";
	std::cout << "Encrypted text: \n";
	for (int i = 0; i < this->encrypted_text.size(); i += 4) {
		if (i + 7 < this->encrypted_text.size()) {
			std::cout << i << "-" << i + 8 << ": {" << intToHex(unsigned int(this->encrypted_text.at(i))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 1))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 2))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 3))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 4))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 5))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 6))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 7))) << "}\n";
			i += 4;
		}
		else {
			std::cout << i << "-" << i + 4 << ": {" << intToHex(unsigned int(this->encrypted_text.at(i))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 1))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 2))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 3))) << "}\n";
		};
	};
	for (int i = 0; i < 4; i += 1) {
		std::cout << "ID Seed #" << i << ": {" << intToHex(unsigned int(this->ID_seeds.at(i)[0])) << ", " << intToHex(unsigned int(this->ID_seeds.at(i)[1])) << ", " << intToHex(unsigned int(this->ID_seeds.at(i)[2])) << ", " << intToHex(unsigned int(this->ID_seeds.at(i)[3])) << "}\n";
	};
	for (int i = 0; i < this->text_seeds.size(); i++) {
		std::cout << "Text Seed #" << i << ": {" << intToHex(unsigned int(this->text_seeds.at(i)[0])) << ", " << intToHex(unsigned int(this->text_seeds.at(i)[1])) << ", " << intToHex(unsigned int(this->text_seeds.at(i)[2])) << ", " << intToHex(unsigned int(this->text_seeds.at(i)[3])) << "}\n";
	};
	std::cout << "Plain text: " << this->plain_text << "\n";
}

std::vector<std::string> DecryptionSession::textToHex() {
	std::vector<std::string> return_vector = {};
	for (int i = 0; i < this->encrypted_text.size(); i++) { //iterates through text
		return_vector.push_back(intToHex(unsigned int(this->encrypted_text.at(i))));
	};
	return return_vector;
};

void DecryptionSession::readFromFile(std::string file_path) {
	//manage file system
	std::ifstream read_file;
	read_file.open(file_path, std::ios::in | std::ios::binary);
	read_file.seekg(0, read_file.end);
	int size = read_file.tellg();
	read_file.seekg(0);
	// allocate memory for file content
	char* buffer = new char[size];
	// read content of infile
	read_file.read(buffer, size);
	std::vector<unsigned char> data;
	data.insert(data.end(), buffer, buffer + size);
	//close file
	delete buffer;
	read_file.close();
	this->encrypted_text = data;
};
void DecryptionSession::saveToFile(std::string file_path) {
	std::ofstream save_file;
	save_file.open(file_path, std::ios::out);
	save_file << this->plain_text;
	save_file.close();
};
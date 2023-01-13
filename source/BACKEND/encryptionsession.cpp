#include "encryptionsession.h"
#include "password.h"
#include <string>
#include <array>
#include <vector>
#include <stdint.h>
#include <random>
#include <iostream>
#include <fstream>
#include "inttohex.h"

std::array<unsigned char, 4> EncryptionSession::generateGarble() {
	std::uint32_t random = this->mtwister();
	std::array<unsigned char, 4> garble = { 0x00, 0x00, 0x00, 0x00 };
	garble.at(0) = random >> 24;
	garble.at(1) = (random >> 16) & 0xFF;
	garble.at(2) = (random >> 8) & 0xFF;
	garble.at(3) = random & 0xFF;
	return garble;
}
std::vector<unsigned char> EncryptionSession::encryptChar(unsigned char letter, std::array<unsigned char, 4>* seed) {
	//uses the seed to encrypt the letter

	std::vector<unsigned char> encrypted_sequence = {};
	//step 1: add seed 1 to char
	unsigned short character = letter;
	character += (*seed).at(0) % 256;
	letter = character;
	//letter = (letter + (*seed).at(0)) % 255;
	//step 2: create garble
	for (int i = 0; i < (*seed).at(1); i++) { //only create enough garble as defined by the seed
		std::array<unsigned char, 4> garble = generateGarble();
		for (int i = 0; i < 4; i++) { //iterate through garble and add to the sequence
			encrypted_sequence.push_back(garble.at(i));
		};
	};
	//step 3: find insertion point
	int insertion_point = ((*seed).at(2) % (*seed).at(1)) * 4; //makes sure that the seed insertion point is within bounds of garble length
	insertion_point += (*seed).at(3); //adds offsets
	//step 4: insert
	encrypted_sequence.at(insertion_point) = letter;
	return encrypted_sequence;
};
void EncryptionSession::encryptAll() {
	std::vector<unsigned char> char_vector;
	//encrypt text
	for (int i = 0; i < this->plain_text.size(); i++) {
		std::vector<unsigned char> temp_vector = encryptChar(this->plain_text.at(i), &this->text_seeds.at(i));
		for (int x = 0; x < temp_vector.size(); x++) {
			char_vector.push_back(temp_vector.at(x));
		};
	};
	
	//encrypt ID
	for (int i = 0; i < this->ID.chars.size(); i++) {
		std::vector<unsigned char> temp_vector = encryptChar(this->ID.chars.at(i), &this->ID_seeds.at(i));
		for (int x = 0; x < temp_vector.size(); x++) {
			char_vector.push_back(temp_vector.at(x));
		};
	};
	
	this->encrypted_text = char_vector;
};
EncryptionSession::EncryptionSession(std::string text, std::string password) {
	this->password = password;
	this->plain_text = text;
	//convert password to int
	this->password_int = convertPassword(password);
	//startup mt19937 machine
	this->mtwister = std::mt19937(password_int);
	this->ID = IDStruct(mtwister());
	//generate seed for ID
	for (int i = 0; i < this->ID.chars.size(); i++) {
		this->ID_seeds.push_back(generateSeed());
	};
	//generate seeds for each letter
	for (int i = 0; i < text.size(); i++) {
		this->text_seeds.push_back(generateSeed());
	};
	//encrypt the text
	this->encryptAll(); 
}

void EncryptionSession::printState() {
	std::cout << "ID: " << this->ID.num << "\n";
	std::cout << "ID Array: {" << intToHex(unsigned int(this->ID.chars[0])) << ", " << intToHex(unsigned int(this->ID.chars[1])) << ", " << intToHex(unsigned int(this->ID.chars[2])) << ", " << intToHex(unsigned int(this->ID.chars[3])) << "}\n";
	std::cout << "Password: " << this->password << "\n";
	std::cout << "Int Password: " << this->password_int << "\n";
	std::cout << "Plain text: " << this->plain_text << "\n";
	std::cout << "Hex of text: \n";
	for (int i = 0; i < this->plain_text.size(); i++) {
		std::cout << this->plain_text.at(i) << ": " << intToHex(unsigned int(this->plain_text.at(i))) << "\n";
	};
	for (int i = 0; i < this->text_seeds.size(); i++) {
		std::cout << "Text Seed #" << i << ": {" << intToHex(unsigned int(this->text_seeds.at(i)[0])) << ", " << intToHex(unsigned int(this->text_seeds.at(i)[1])) << ", " << intToHex(unsigned int(this->text_seeds.at(i)[2])) << ", " << intToHex(unsigned int(this->text_seeds.at(i)[3])) << "}\n";
	};
	for (int i = 0; i < 4; i += 1) {
		std::cout << "ID Seed #" << i << ": {" << intToHex(unsigned int(this->ID_seeds.at(i)[0])) << ", " << intToHex(unsigned int(this->ID_seeds.at(i)[1])) << ", " << intToHex(unsigned int(this->ID_seeds.at(i)[2])) << ", " << intToHex(unsigned int(this->ID_seeds.at(i)[3])) << "}\n";
	};
	std::cout << "Encrypted text: \n";
	for (int i = 0; i < this->encrypted_text.size(); i += 4) {
		if (i + 7 < this->encrypted_text.size()) {
			std::cout << i << "-" << i + 8 << ": {" << intToHex(unsigned int(this->encrypted_text.at(i))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 1))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 2))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 3))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 4))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 5))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 6))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 7))) << "}\n";
			i += 4;
		} else {
			std::cout << i << "-" << i + 4 << ": {" << intToHex(unsigned int(this->encrypted_text.at(i))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 1))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 2))) << ", " << intToHex(unsigned int(this->encrypted_text.at(i + 3))) << "}\n";
		};
	};
};
void EncryptionSession::saveToFile(std::string file_path) {
	//manage file system
	//typedef std::basic_ofstream<unsigned char, std::char_traits<unsigned char> > uofstream;
	std::ofstream save_file(file_path, std::ios::out | std::ios::binary);
	//create header
	// TODO???
	//add encrypted text
	std::copy((this->encrypted_text).cbegin(), (this->encrypted_text).cend(), std::ostream_iterator<unsigned char>(save_file));
	//close file
	save_file.close();
};
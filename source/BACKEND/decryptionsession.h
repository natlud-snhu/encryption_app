#pragma once
#include "password.h"
#include "session.h"
#include <string>
#include <array>
#include <vector>
#include <stdint.h>
#include <random>
#include <iostream>
#include <fstream>

class DecryptionSession : public Session {
private:
	std::array<unsigned char, 4> generateGarble(); //uses mt19937 to create random garble
	void countSeeds(); //use the encrypted_text to count and generate the required number of seeds
	void decryptAll(); //attempts to decrypt encrypted_text
public:
	DecryptionSession();
	DecryptionSession(std::string file);
	DecryptionSession(std::string file, std::string pass);
	void printState(); 
	void saveToFile(std::string file_path); 
	void readFromFile(std::string fle_path);
	void tryDecryption(std::string pass); //finishes decryption for session made with only a filepath
	bool decrypted = 0; //whether file has been decrypted
};
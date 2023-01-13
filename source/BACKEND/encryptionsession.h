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


class EncryptionSession : public Session
{
private:
	std::array<unsigned char, 4> generateGarble(); //uses mt19937 to create random garble
	std::vector<unsigned char> encryptChar(unsigned char letter, std::array<unsigned char, 4>* seed); //encrypts a single letter
	void encryptAll(); //itereates through encryptChar and organizes data
public:
	EncryptionSession(std::string text, std::string password);
	void printState(); //outputs the current state of the session to the console
	void saveToFile(std::string file_path); //saves encrypted text to a file_path
};


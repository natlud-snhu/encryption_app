#pragma once
#include <string>
#include <stdint.h>
#include <random>
#include <array>


std::string createPassword(); //uses random to create a secure password
unsigned int convertPassword(std::string pass); //converts password from string to int
//std::array<unsigned char, 4> generateSeed(std::mt19937* mtwister); //uses mt19937 to create seeds
//std::array<unsigned char, 4> generateGarble(std::mt19937* mtwister); //uses mt19937 to create random garble


#include <string>
#include "validation.h"

bool isPasswordValid(std::string pass) {
	//make sure length is 16
	if (pass.size() != 16) {
		return false;
	};
	//make sure alphanumerical values only
	for (int i = 0; i < pass.size(); i++) {
		if (!isalnum(pass.at(i))) {
			return false;
		};
	};
	return true;
};
bool isInputValid(std::string input) {
	//makes sure string isn't empty
	if (input.empty()) {
		return false;
	};
	//makes sure string isn't too long
	if (input.size() > 1000) {
		return false;
	};
	return true;
};

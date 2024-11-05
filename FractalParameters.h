#pragma once
#include <map>
#include <string>
#include "Complex.h"


// make a boilerplate template for fractal parameters with some flexibility
struct fractalParameters
{
	std::map<std::string, int> integerParameters;
	std::map<std::string, double> doubleParameters;
	std::map<std::string, complex> complexParameters;
};
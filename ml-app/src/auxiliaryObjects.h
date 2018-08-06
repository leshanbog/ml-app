#pragma once

#include <vector>
#include <string>

#include "auxiliaryObjects/ArgsForAlg.h"
#include "auxiliaryObjects/DataFrame.h"
#include "auxiliaryObjects/Obj.h"
#include "auxiliaryObjects/UsefulMath.h"


using std::string;
using std::vector;

#define DEBUG_TRACE1(x) std::cout << "DEBUG: " << x << std::endl;
#define DEBUG_TRACE2(x,y) std::cout << "DEBUG: " << x << y << std::endl;
#define CONSOLE_OUTPUT(x) std::cout << x;
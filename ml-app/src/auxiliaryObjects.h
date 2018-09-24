#pragma once

#include <vector>
#include <string>

#ifdef L_DEBUG
	#include <iostream>
#endif

#include "auxiliaryObjects/ArgsForAlg.h"
#include "auxiliaryObjects/DataFrame.h"
#include "auxiliaryObjects/Obj.h"
#include "auxiliaryObjects/UsefulMath.h"


using std::string;
using std::vector;


#define CONSOLE_OUTPUT(x) std::cout << x;

#ifdef L_DEBUG
        #define DEBUG_LOG(x) std::cout << x << '\n';
        #define DEBUG_TRACE2(x,y) std::cout << x << ' ' << y << '\n';
        #define DEBUG_TRACE3(x,y,z) std::cout << x << ' ' << y << ' ' << z << '\n';
        #define DEBUG_TRACE4(a,b,c,d) std::cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';

        #define DEBUG_COND_LOG(c, a) if (c) DEBUG_LOG(a);
        #define DEBUG_COND_TRACE2(c, a, b) if (c) DEBUG_TRACE2(a,b)
        #define DEBUG_COND_TRACE3(c, a, b, d) if (c) DEBUG_TRACE2(a,b, d)
        #define DEBUG_COND_TRACE4(c, a, b, x, y) if (c) DEBUG_TRACE2(a,b,x,y)

        #define DEBUG_ASSERT_TRUE(x) if (!(x)) throw std::runtime_error(x);
#else
    #define DEBUG_LOG(x)
    #define DEBUG_TRACE2(x,y)
    #define DEBUG_TRACE3(x,y,z)
    #define DEBUG_TRACE4(x,y,z,a)

    #define DEBUG_COND_LOG(c, a)
    #define DEBUG_COND_TRACE2(c, a, b)
    #define DEBUG_COND_TRACE3(c, a, b, d)
    #define DEBUG_COND_TRACE4(c, a, b, x, y)

    #define DEBUG_ASSERT_TRUE(x)
#endif
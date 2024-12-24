#include <gtest.h>
#include <iostream>
#include "translator.h"

int main(int argc, char** argv) {
    std::cout << "The arithmetic translator supports arithmetic operations: + - * / ^,\
        \nvariables, constants  pi  and  e, as well as mathematical functions:\
        \nsine,  cosine,  tangent,  modulus  and  arctangent\
        \n\nEnter an arithmetic expression:\n";
    std::string str;
    while (true) {
        getline(std::cin, str);
        if (str == "RunTests") {
            ::testing::InitGoogleTest(&argc, argv);
            return RUN_ALL_TESTS();
        }
        GetResult(str);
    }
}
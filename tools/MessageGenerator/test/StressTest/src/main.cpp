#include "StressTest.hxx"

#include <iostream>

int main(const int argc,
         const char** argv) {
    StressTest stressTest;
    stressTest.setSimpleFloat(3.14);
    std::cout << stressTest.getSimpleFloat() << std::endl;
}
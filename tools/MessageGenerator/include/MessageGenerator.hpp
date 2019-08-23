#pragma once

class MessageGenerator {
public :
    MessageGenerator(const int argc,
                     char** argv);
    
private :
    void printUsageAndExit();
    
    bool javaRequired;
    bool cppRequired;
    bool verbose;
};
#include "FileParser.hpp"
#include "Definitions.hxx"
#include "CppGenerator.hpp"
#include "JavaGenerator.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

FileParser::FileParser() {

}

bool FileParser::parse(const std::string& file,
                       const Language language,
                       const std::string& outputFolder) {
    try {
        xml_schema::Properties properties;
        auto definitions(parseDefinitions(file, 0, properties));
    
        switch (language) {
            case CPP : {
                cppGenerator.generate(*definitions, outputFolder);

                break;
            }

            case JAVA : {
                //JavaOutputter::generate(definitions, outputFolder);

                break;
            }

            default : {
                std::cerr << "Unknown language enum \"" << language << '\"';

                break;
            }
        }   
    } catch (const ::xml_schema::Exception &e) {
        std::cerr << e.what();
    }
}


#include "FileParser.hpp"
#include "Definitions.hxx"
#include "CppGenerator.hpp"
#include "JavaGenerator.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#include "CppGenerator.hpp"

FileParser::FileParser() {

}

bool FileParser::parse(const std::string& file,
                       const Language language,
                       const std::string& outputFolder) {
    bool returnVal = false;
    
    try {
        xml_schema::Properties properties;
        auto definitions(parseDefinitions(file, 0, properties));
    
        switch (language) {
            case Language::CPP : {
                CppGenerator cppGenerator(*definitions);
                returnVal = cppGenerator.generate(outputFolder);

                break;
            }

            case Language::JAVA : {
                //JavaOutputter::generate(definitions, outputFolder);

                break;
            }

            default : {
                std::cerr << "Unknown language enum \"" << static_cast<std::underlying_type<Language>::type>(language) << '\"';

                break;
            }
        }   
    } catch (const xml_schema::Exception &exception) {
        std::cerr << exception.what();
    }
    
    return returnVal;
}


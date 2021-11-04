#include "Generator.hpp"

const std::string Generator::TAB = "    ";

Generator::Generator(const Messaging::Definitions& definitions) : definitions(definitions),
                                                       namespaceOn(false) {
    
}

std::string Generator::insertTabs(const uint8_t numTabs) {
    std::ostringstream os;
    
    for (uint8_t index = 0; index < numTabs; ++index) {
        os << TAB;
    }
    
    if (namespaceOn) {
        os << TAB;
    }
    
    return os.str();
}

void Generator::setNamespace(bool on) {
    namespaceOn = on;
}
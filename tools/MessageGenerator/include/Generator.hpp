#pragma once

#include "Definitions.hxx"

class Generator {
    public:
        Generator(const Messaging::Definitions& definitions);
        
        virtual bool generate(const std::string& outputFolder) = 0;
        
    protected:
        void setNamespace(bool on = true);
        
        std::string insertTabs(const uint8_t numTabs = 0);
        
        const Messaging::Definitions& definitions;
        
    private:        
        bool namespaceOn;
        
        const static std::string TAB;
};
#pragma once

#include "Definitions.hxx"

class Generator {
    public:
        Generator();
        
        virtual bool generate(const Definitions& definitions,
                              const std::string& outputFolder) = 0;
        
    protected:
        void setNamespace(bool on = true);
        
        std::string insertTabs(const uint8_t numTabs = 0);
        
    private:        
        bool namespaceOn;
        
        const static std::string TAB;
};
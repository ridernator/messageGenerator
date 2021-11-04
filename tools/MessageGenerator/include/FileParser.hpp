#pragma once

#include <string>

#include "Language.hpp"

class FileParser {
public:
    FileParser();

    bool parse(const std::string& file,
               const Language language,
               const std::string& outputFolder);
};
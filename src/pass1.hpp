#pragma once

#include <string>
#include <map>

#include "lex.hpp"

class Pass1 {
public:
    explicit Pass1(std::string input);
    std::map<std::string, int> run();
private:
    Lex *lex;
};


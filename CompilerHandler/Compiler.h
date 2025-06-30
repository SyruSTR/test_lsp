//
// Created by grusted on 6/30/25.
//

#ifndef COMPILER_H
#define COMPILER_H

#include <fstream>
#include <iosfwd>
#include "../external/json.hpp"

class Compiler {
public:
    Compiler(const std::string &compiler_log_path);
    ~Compiler();

    nlohmann::json run(const std::string &checked_file) const;
private:
    std::ofstream compiler_log;
    std::string compiler_log_path;
    std::string compiler_path = "../external/IFJ23-compiler/compiler";
};



#endif //COMPILER_H

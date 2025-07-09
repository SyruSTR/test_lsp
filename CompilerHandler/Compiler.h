//
// Created by grusted on 6/30/25.
//

#ifndef COMPILER_H
#define COMPILER_H

#include <fstream>
#include <iosfwd>

class Compiler {
public:
    explicit Compiler(const std::string &compiler_log_path);
    ~Compiler();

    std::string run(const std::string &file_content, bool debug = false) const;
private:
    std::ofstream compiler_log;
    std::string compiler_log_path;
    std::string compiler_path;
};



#endif //COMPILER_H

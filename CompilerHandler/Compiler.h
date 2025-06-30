//
// Created by grusted on 6/30/25.
//

#ifndef COMPILER_H
#define COMPILER_H

#include <fstream>
#include <iosfwd>
#include <thread>

class Compiler {
public:
    Compiler(const std::string &compiler_log_path);
    ~Compiler();

    void run(const std::string &checked_file);
    void stop();
private:
    std::thread worker_;
    std::mutex mtx_;
    std::atomic<pid_t> child_pid_{-1};
    std::ofstream compiler_log;
    std::string compiler_log_path;
    std::string compiler_path = "../external/IFJ23-compiler/compiler";
};



#endif //COMPILER_H

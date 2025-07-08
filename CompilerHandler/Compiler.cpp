//
// Created by grusted on 6/30/25.
//

#include "Compiler.h"

#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <filesystem>
#include <sstream>


Compiler::Compiler(const std::string &compiler_log_path){
    this->compiler_log_path = compiler_log_path;

    std::filesystem::path current_dir = std::filesystem::current_path();
    current_dir = current_dir.parent_path();
    this->compiler_path = current_dir / "external" / "IFJ23-compiler" / "compiler";
}

Compiler::~Compiler() {

}


std::string Compiler::run(const std::string &file_content, bool debug) const {
    int pipe_stdin[2];
    int pipe_stderr[2];
    int save_stdout = dup(STDOUT_FILENO);

    if (pipe(pipe_stdin) == -1 || pipe(pipe_stderr) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    const pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // child
        dup2(pipe_stdin[0], STDIN_FILENO);
        close(pipe_stdin[0]);
        close(pipe_stdin[1]);

        dup2(pipe_stderr[1], STDERR_FILENO);
        close(pipe_stderr[0]);
        close(pipe_stderr[1]);

        // redirect stdout to /dev/null
        int devnull = open("/dev/null", O_WRONLY);
        dup2(devnull, STDOUT_FILENO);
        close(devnull);

        execl(compiler_path.c_str(), "compiler",debug ? "-d" : "", "1>&2", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }
    // parent

    close(pipe_stdin[0]);
    close(pipe_stderr[1]);

    // Write to child
    std::stringstream str_stream(file_content);
    std::string line;
    while (std::getline(str_stream, line)) {
        // compiler needs a '\n'
        line.append("\n");
        if (auto status = write(pipe_stdin[1], &line[0], line.size()); status == -1) {
            if (errno == EPIPE) {
                std::cerr << "Child process closed stdin (EPIPE)\n";
            }
            else {
                std::cerr << "Write failed: " << strerror(errno) << "\n";
            }
        }
    }

    close(pipe_stdin[1]);  // EOF

    // Read from child stderr
    std::string full_stderr;
    const int bufferSize = 512;
    std::string test_str_buff(bufferSize,'\0');
    ssize_t bytesRead;
    while ((bytesRead = read(pipe_stderr[0], &test_str_buff[0], test_str_buff.capacity() - 1)) > 0) {
        test_str_buff[bytesRead] = '\0';
        full_stderr.append(test_str_buff, 0,bytesRead);
    }

    close(pipe_stderr[0]);

    wait(nullptr); // Wait for child

    // restore stdout
    dup2(save_stdout, STDOUT_FILENO);
    close(save_stdout);

    return full_stderr;
}



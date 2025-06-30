//
// Created by grusted on 6/30/25.
//

#include "Compiler.h"

#include <cstring>
#include <iostream>
#include <sys/wait.h>


Compiler::Compiler(const std::string &compiler_log_path){
    this->compiler_log_path = compiler_log_path;
}

Compiler::~Compiler() {

}


void Compiler::run(const std::string &checked_file) {
    int pipe_stdin[2];
    int pipe_stderr[2];

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

        execl(compiler_path.c_str(), "compiler", "1>&2", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }
    // parent

    close(pipe_stdin[0]);
    close(pipe_stderr[1]);

    // Write to child
    std::ifstream file(checked_file, std::ios::out);
    std::string line;

    while (std::getline(file, line)) {
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
    std::string test_str_buff(1024,'\0');
    ssize_t bytesRead;
    while ((bytesRead = read(pipe_stderr[0], &test_str_buff[0], test_str_buff.capacity() - 1)) > 0) {
        test_str_buff[bytesRead] = '\0';
        printf("Received from child: %s", test_str_buff.c_str());
    }
    close(pipe_stderr[0]);

    wait(nullptr); // Wait for child
}



//
// Created by grusted on 6/28/25.
//



#include <fstream>
#include <iostream>

#include "Compiler.h"
#include "CompilerOutput.h"


    using json = nlohmann::json;

    int main() {
        // int pipe_stdin[2];
        // int pipe_stderr[2];
        //
        // if (pipe(pipe_stdin) == -1 || pipe(pipe_stderr) == -1) {
        //     perror("pipe");
        //     exit(EXIT_FAILURE);
        // }
        //
        // const pid_t pid = fork();
        // if (pid == -1) {
        //     perror("fork");
        //     exit(EXIT_FAILURE);
        // }
        //
        //
        // if (pid == 0) {
        //     // child
        //     dup2(pipe_stdin[0], STDIN_FILENO);
        //     close(pipe_stdin[0]);
        //     close(pipe_stdin[1]);
        //
        //     dup2(pipe_stderr[1], STDERR_FILENO);
        //     close(pipe_stderr[0]);
        //     close(pipe_stderr[1]);
        //
        //     // Replace process with "cat"
        //     execl("../external/IFJ23-compiler/compiler", "compiler", "1>&2", NULL);
        //     perror("execl");
        //     exit(EXIT_FAILURE);
        // } else {
        //     // parent
        //     close(pipe_stdin[0]);
        //     close(pipe_stderr[1]);
        //
        //     // Write to child
        //
        //     std::ifstream file("/tmp/example.txt", std::ios::out);
        //     std::string line;
        //
        //     while (std::getline(file, line)) {
        //         if (auto status = write(pipe_stdin[1], &line[0], line.size()); status == -1) {
        //             if (errno == EPIPE) {
        //                 std::cerr << "Child process closed stdin (EPIPE)\n";
        //             }
        //             else {
        //                 std::cerr << "Write failed: " << strerror(errno) << "\n";
        //             }
        //         }
        //     }
        //     close(pipe_stdin[1]);  // EOF
        //
        //     // Read from child stderr
        //     std::string test_str_buff(1024,'\0');
        //     ssize_t bytesRead;
        //     while ((bytesRead = read(pipe_stderr[0], &test_str_buff[0], test_str_buff.capacity() - 1)) > 0) {
        //         test_str_buff[bytesRead] = '\0';
        //         printf("Received from child: %s", test_str_buff.c_str());
        //     }
        //     close(pipe_stderr[0]);
        //
        //     wait(nullptr); // Wait for child
        // }

        const std::string raw_json = R"(
{
        "error_code": 99,
        "message": "Timeout error"
}

    )";

        auto j = json::parse(raw_json);

        lsp_test::CompilerOutput _comp_output = json::parse(raw_json);
        // from_json(j, eb);

        std::cout << "error_code: " << _comp_output.error_code << "\n";
        if (_comp_output.message.has_value()) std::cout << "message: " << _comp_output.message.value() << "\n";

        return 0;

        // Compiler compiler("/tmp/log-compiler.txt");
        // file:///tmp/Taylor.swift
        // auto msg = compiler.run("let a = 2 \nl");
        // if (msg.empty()) {
        //     return 0;
        // }
        //
        // auto raw_msg = nlohmann::json::parse(msg);
        //
        // lsp_test::compiler_output output = raw_msg;
        // //
        // std::cout << output.get_token_string_representation() << " " << output.token_type_;
        // return 0;


    }
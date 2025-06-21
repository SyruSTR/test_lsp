//
// Created by grusted on 6/16/25.
//

#ifndef SERVERINFO_H
#define SERVERINFO_H
#include <string>

namespace lsp_test {
    struct ServerInfo {
        std::string name;
        std::string version;

        ServerInfo(std::string name, std::string version) : name(name), version(version) {}
        ServerInfo() {
            this->name = "my-lsp-server";
            this->version = "0.0.1";
        }
    };

    void inline to_json(json &j, const ServerInfo &info) {
        j = json{
            {"name", info.name},
            {"version",info.version},
        };
    }
}

#endif //SERVERINFO_H

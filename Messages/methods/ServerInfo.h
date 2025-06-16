//
// Created by grusted on 6/16/25.
//

#ifndef SERVERINFO_H
#define SERVERINFO_H
#include <string>

struct ServerInfo {
    std::string name;
    std::optional<std::string> version;

    ServerInfo(std::string name, std::optional<std::string> version) : name(name), version(version) {}
};

#endif //SERVERINFO_H

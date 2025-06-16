//
// Created by grusted on 6/16/25.
//

#ifndef SERVERINFO_H
#define SERVERINFO_H
#include <string>

struct ServerInfo {
    std::string name;
    std::optional<std::string> version;
};

#endif //SERVERINFO_H

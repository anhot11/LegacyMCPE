#pragma once

class PlayerInfo {
public:
    std::string name;
    int latency;

    PlayerInfo(const std::string& name) {
        this->name = name;
        latency = 0;
    }
};
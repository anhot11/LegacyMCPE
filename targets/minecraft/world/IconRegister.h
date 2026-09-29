#pragma once

class Icon;

class IconRegister {
public:
    // 4J Stu - register is a reserved keyword in C++
    virtual Icon* registerIcon(const std::string& name) = 0;
    virtual int getIconType() = 0;
};
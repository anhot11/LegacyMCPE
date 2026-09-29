#pragma once

#include <exception>
#include <string>

class EOFException : public std::exception {};

class IllegalArgumentException : public std::exception {
public:
    std::string information;

    IllegalArgumentException(const std::string& information);
};

class IOException : public std::exception {
public:
    std::string information;

    IOException(const std::string& information);
};

class RuntimeException : public std::exception {
public:
    RuntimeException(const std::string& information);
};
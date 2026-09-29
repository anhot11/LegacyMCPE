#pragma once

class DescFormatter {
public:
    virtual ~DescFormatter() = default;
    virtual std::string format(const std::string& i18nValue) = 0;
};

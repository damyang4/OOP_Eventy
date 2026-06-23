#pragma once

#include <ctime>
#include <string>

class DateTime {
private:
    std::time_t rawTime;

public:
    DateTime();

    explicit DateTime(std::time_t time);

    // Parses a string format "YYYY-MM-DD_HH:MM" into a DateTime object
    static DateTime parse(const std::string& str);

    std::string toString() const;

    std::time_t getRawTime() const;

    bool isBefore(const DateTime& other) const;
};
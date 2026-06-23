#include "DateTime.h"

#include <sstream>

DateTime::DateTime() {
        rawTime = std::time(nullptr);
    }
    

DateTime::DateTime(std::time_t time) : rawTime(time) {}

// Parses a string format "YYYY-MM-DD_HH:MM" into a DateTime object
static DateTime parse(const std::string& str) {
    std::tm t = {};
    std::istringstream ss(str);
    // Using standard stream extraction matching the format
    char d1, d2, breakChar, t1;
    int year, month, day, hour, minute;
    
    if (ss >> year >> d1 >> month >> d2 >> day >> breakChar >> hour >> t1 >> minute) {
        t.tm_year = year - 1900;
        t.tm_mon = month - 1;
        t.tm_mday = day;
        t.tm_hour = hour;
        t.tm_min = minute;
        t.tm_isdst = -1; // Determine automatically
        return DateTime(std::mktime(&t));
    }
    return DateTime(0); // Invalid fallback
}

std::string DateTime::toString() const {
    std::tm* t = std::localtime(&rawTime);
    char buffer[64];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", t);
    return std::string(buffer);
}

std::time_t DateTime::getRawTime() const { return rawTime; }

bool DateTime::isBefore(const DateTime& other) const {
    return rawTime < other.rawTime;
}
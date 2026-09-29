#include "Stat.h"

#include <unordered_map>
#include <utility>
#include <vector>

#include "NumberFormatters.h"
#include "StatFormatter.h"
#include "Stats.h"
#include "util/StringHelpers.h"

Stat::DefaultFormat* Stat::defaultFormatter = new DefaultFormat();
Stat::TimeFormatter* Stat::timeFormatter = new TimeFormatter();
Stat::DistanceFormatter* Stat::distanceFormatter = new DistanceFormatter();

// 4J Stu - Changed this to take in a printf format string instead
DecimalFormat* Stat::decimalFormat = new DecimalFormat("%0(3).2f");

void Stat::_init() { awardLocallyOnly = false; }

Stat::Stat(int id, const std::string& name, StatFormatter* formatter)
    : id(id), name(name), formatter(formatter) {
    _init();
}

Stat::Stat(int id, const std::string& name)
    : id(id), name(name), formatter(defaultFormatter) {
    _init();
}

Stat* Stat::setAwardLocallyOnly() {
    awardLocallyOnly = true;
    return this;
}

Stat* Stat::postConstruct() {
    // if (Stats::statsById->containsKey(id))
    //{
    // throw new RuntimeException("Duplicate stat id: \"" +
    // Stats::statsById->get(id)->name + "\" and \"" + name + "\" at id " + id);
    // 4J - TODO
    //}
    Stats::all->push_back(this);

    std::pair<int, Stat*> id1(id, this);
    Stats::statsById->emplace(std::move(id1));

    return this;
}

bool Stat::isAchievement() { return false; }

std::string Stat::format(int value) {
    return ((StatFormatter*)formatter)->format(value);
}

std::string Stat::toString() { return name; }

std::string Stat::TimeFormatter::format(int value) {
    double seconds = value / 20.0;
    double minutes = seconds / 60.0;
    double hours = minutes / 60.0;
    double days = hours / 24.0;
    double years = days / 365.0;

    if (years > 0.5) {
        return decimalFormat->format(years) + " y";
    } else if (days > 0.5) {
        return decimalFormat->format(days) + " d";
    } else if (hours > 0.5) {
        return decimalFormat->format(hours) + " h";
    } else if (minutes > 0.5) {
        return decimalFormat->format(minutes) + " m";
    }

    return toWString<double>(seconds) + " s";
}

std::string Stat::DefaultFormat::format(int value) {
    return NumberFormat::format(value);  // numberFormat->format(value);
}

std::string Stat::DistanceFormatter::format(int cm) {
    double meters = cm / 100.0;
    double kilometers = meters / 1000.0;

    if (kilometers > 0.5) {
        return decimalFormat->format(kilometers) + " km";

    } else if (meters > 0.5) {
        return decimalFormat->format(meters) + " m";
    }
    return toWString<int>(cm) + " cm";
}

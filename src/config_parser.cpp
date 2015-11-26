// Copyright Nikita Chudinov on 21.11.15.
//

#include "config_parser.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <vector>
#include <string>

std::vector<ConfigUnit> ConfigParser::ParseConfig(std::string config_filename) {
    using boost::property_tree::ptree;
    ptree pt;

    read_json(config_filename, pt);
    std::vector<ConfigUnit> result;

    auto units = pt.equal_range("");
    for (auto unit_iter = units.first; unit_iter != units.second; ++unit_iter) {
        ConfigUnit unit;

        auto paths_pointer = unit_iter->second.find("paths");
        auto paths_iter = paths_pointer->second.equal_range("");

        for (auto path = paths_iter.first; path != paths_iter.second; ++path) {
            unit.paths.push_back(path->second.get<std::string>(""));
        }

        auto regex_pointer = unit_iter->second.find("regex");
        auto regex_iter = regex_pointer->second.equal_range("");

        for (auto expr = regex_iter.first; expr != regex_iter.second; ++expr) {
            unit.regex.push_back(expr->second.get<std::string>(""));
        }

        result.push_back(unit);
    }

    return result;
}

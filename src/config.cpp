// Copyright 2015 Nikita Chudinov

#include "config.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

ConfigParser::ConfigParser(std::string config_file) {
    using boost::property_tree::ptree;
    ptree pt;

    read_json(config_file, pt);
    sleep_duration = pt.get("sleep_duration", 3600);
    path_list_file = pt.get("path_list_file", "path_list.json");

    std::string s = pt.get("log_severity", "warning");

    if (!s.compare("warning")) {
        log_severity = plog::warning;
    } else if (!s.compare("info")) {
        log_severity = plog::info;
    }

    log_filename = pt.get("log_filename", "integrity.log");
}

int ConfigParser::GetSleepDuration() const {
    return sleep_duration;
}

std::string ConfigParser::GetPathListFile() const {
    return path_list_file;
}

plog::Severity ConfigParser::GetLogSeverity() const {
    return log_severity;
}

std::string ConfigParser::GetLogFilename() const {
    return log_filename;
}

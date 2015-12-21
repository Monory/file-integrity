// Copyright 2015 Nikita Chudinov
#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

#include <string>
#include "logging.h"

class ConfigParser {
public:
    ConfigParser(const std::string &config_file);
    int GetSleepDuration() const;
    std::string GetPathListFile() const;
    std::string GetLogFilename() const;
    plog::Severity GetLogSeverity() const;
private:
    int sleep_duration = 3600;
    std::string path_list_file = "path_list.json";
    plog::Severity log_severity = plog::warning;
    std::string log_filename = "integrity.log";
};


#endif  // SRC_CONFIG_H_

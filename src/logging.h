// Copyright 2015 Nikita Chudinov
#ifndef SRC_LOGGING_H_
#define SRC_LOGGING_H_

#include <plog/Log.h>
#include <plog/Util.h>

namespace plog {
class LogFormatter {
public:
    static util::nstring header();
    static util::nstring format(const Record &record);
};
}


#endif  // SRC_LOGGING_H_

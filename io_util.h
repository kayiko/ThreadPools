//
// Created by WYZ on 2024/4/16.
//

#ifndef THREADPOOLS_IO_UTIL_H
#define THREADPOOLS_IO_UTIL_H

#endif //THREADPOOLS_IO_UTIL_H
#pragma once
#include <iostream>
#include <optional>

#include "status.h"
#include "visibility.h"
ARROW_EXPORT
std::optional<std::string> GetEnvVar(const char* name);
ARROW_EXPORT
Status SetEnvVar(const char* name, const char* value);
ARROW_EXPORT
Status SetEnvVar(const std::string& name, const std::string& value);
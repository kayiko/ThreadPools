//
// Created by WYZ on 2024/4/16.
//
#include "io_util.h"
#include <stdlib.h>

int setenv(const char *name, const char *value, int overwrite)
{
    int errcode = 0;
//    if(!overwrite) {
//        size_t envsize = 0;
//        errcode = _getenv_s(&envsize, NULL, 0, name);
//        if(errcode || envsize) return errcode;
//    }
    return _putenv_s(name, value);
}

std::optional<std::string> GetEnvVar(const char* name) {
    char* c_str = getenv(name);
    if (c_str == nullptr) {
        return {};
    }
    return std::string(c_str);
}

Status SetEnvVar(const char* name, const char* value) {
    if (setenv(name, value, 1) == 0) {
        return Status::OK();
    } else {
        return Status::Invalid("failed setting environment variable");
    }
}

Status SetEnvVar(const std::string& name, const std::string& value) {
    return SetEnvVar(name.c_str(), value.c_str());
}

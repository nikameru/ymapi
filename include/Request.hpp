#pragma once

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Request
{
    void initialize();
    void addParams(std::string &url, json params);
    std::string get(std::string url);
};
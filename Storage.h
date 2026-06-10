#ifndef STORAGE_H
#define STORAGE_H

#include <fstream>
#include <sstream>
#include <string>
#include "../models/Manager.h"

class Storage {
public:
    static bool save(const Manager& manager, const std::string& path = "finance_data.json") {
        std::ofstream f(path);
        if (!f.is_open()) return false;
        f << manager.toJson();
        return f.good();
    }

    static bool load(Manager& manager, const std::string& path = "finance_data.json") {
        std::ifstream f(path);
        if (!f.is_open()) return false;
        std::ostringstream ss;
        ss << f.rdbuf();
        return manager.fromJson(ss.str());
    }
};

#endif

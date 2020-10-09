//
// Created by alex on 09.10.20.
//

#ifndef BCG_GRAPHICS_BCG_FILE_WATCHER_H
#define BCG_GRAPHICS_BCG_FILE_WATCHER_H

#include <unordered_map>
#include <filesystem>
#include <functional>

namespace bcg {

struct file_watcher {
    std::unordered_map<std::string, std::function<bool(bool)>> watched;

    bool trigger(bool force_trigger = false);

    void watch(std::string, std::function<void()> &cb);
};
}

namespace bcg {

void file_watcher::watch(std::string filename, std::function<void()> &cb) {
    auto time = std::filesystem::last_write_time(filename);
    std::function<bool(bool)> function = [filename, cb, time](bool force = false) mutable {
        auto last_mod_time = std::filesystem::last_write_time((filename);
        if (force || time != last_mod_time) {
            time = last_mod_time;
            cb();
            return true;
        }
        return false;
    };
}

bool file_watcher::trigger(bool force_trigger) {
    bool some_file_changed = false;
    for (auto &item : watched) {
        some_file_changed |= item.second(force_trigger);
    }
    return some_file_changed;
}
}

#endif //BCG_GRAPHICS_BCG_FILE_WATCHER_H

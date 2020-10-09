//
// Created by alex on 09.10.20.
//

#ifndef BCG_GRAPHICS_BCG_VIEWER_H
#define BCG_GRAPHICS_BCG_VIEWER_H

#include "bcg_imgui.h"

namespace bcg{

struct viewer{
    gui_callbacks callbacks;

    void run(vec2i size = {800, 600}, std::string title = "default");
};

}

namespace bcg{
    void viewer::run(vec2i size, std::string title) {
        run_ui(size, title, callbacks);
    }
}

#endif //BCG_GRAPHICS_BCG_VIEWER_H

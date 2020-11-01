//
// Created by alex on 28.10.20.
//

#ifndef BCG_GRAPHICS_BCG_RENDERER_H
#define BCG_GRAPHICS_BCG_RENDERER_H

#include <unordered_map>
#include <vector>
#include <string>

#include "../systems/bcg_systems.h"
#include "../systems/bcg_events.h"
#include "../bcg_opengl.h"
#include "entt/entt.hpp"



namespace bcg {

struct viewer_state;

struct renderer : public system {
    explicit renderer(viewer_state *state = nullptr);

    renderer(std::string name, viewer_state *state);

    ~renderer() override = default;

protected:
    virtual void on_begin_frame(const event::begin_frame &event){}

    virtual void on_render(const event::render &event){}

    virtual void on_end_frame(const event::end_frame &event){}

    std::unordered_map<std::string, unsigned int> programs;
    std::vector<entt::entity> entities_to_draw;
    ogl_state gl_state;
};

}

#endif //BCG_GRAPHICS_BCG_RENDERER_H
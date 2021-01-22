//
// Created by alex on 21.01.21.
//

#include "bcg_gui_locally_optimal_projection.h"
#include "bcg_viewer_state.h"
#include "sampling/bcg_sampling_locally_optimal_projection.h"
#include "bcg_entity_hierarchy.h"
#include "bcg_gui_octree_sampling.h"

namespace bcg{

using namespace entt::literals;

void gui_locally_optimal_projection(viewer_state *state){
    static float attraction_radius = 0;
    static float repulsion_weight = 0;
    ImGui::InputFloat("attraction_radius", &attraction_radius);
    if(ImGui::InputFloat("repulsion_weight", &repulsion_weight)){
        repulsion_weight = std::max(0.0f, std::min(repulsion_weight, 0.5f));
    }
    static std::vector<std::string> names = lop_method_names();
    static int method_idx = 0;
    static bool use_density_weight = false;
    ImGui::Checkbox("density weight", &use_density_weight);
    draw_combobox(&state->window, "method", method_idx, names);

    static float feature_radius = 0;
    if(static_cast<LopType>(method_idx) == LopType::flop){
        ImGui::InputFloat("feature_radius", &feature_radius);
    }

    if(ImGui::CollapsingHeader("Generate Sampling")){
        gui_octree_sampling(state);
    }

    auto id = state->picker.entity_id;
    if(ImGui::Button("Init")){
        if(state->scene.valid(id)){
            auto &hierarchy = state->scene.get_or_emplace<entity_hierarchy>(state->picker.entity_id);
            entt::entity child_id = entt::null;
            for (const auto &child : hierarchy.children) {
                if (state->scene.has<entt::tag<"subsampled"_hs>>(child.first)) {
                    child_id = child.first;
                    break;
                }
            }

            if(state->scene.valid(child_id)) {
                auto *ref_vertices = state->get_vertices(id);
                auto *sampling_vertices = state->get_vertices(child_id);
                switch (static_cast<LopType>(method_idx)) {
                    case LopType::lop : {
                        auto &projection = state->scene.get_or_emplace<lop>(id);
                        projection.init(*ref_vertices, *sampling_vertices, use_density_weight);
                        break;
                    }
                    case LopType::wlop : {
                        auto &projection = state->scene.get_or_emplace<wlop>(id);
                        projection.init(*ref_vertices, *sampling_vertices, use_density_weight);
                        break;
                    }
                    case LopType::flop : {
                        auto &projection = state->scene.get_or_emplace<flop>(id);
                        projection.init(*ref_vertices, *sampling_vertices, use_density_weight);
                        break;
                    }
                    case LopType::clop : {
                        auto &projection = state->scene.get_or_emplace<clop>(id);
                        projection.init(*ref_vertices, *sampling_vertices, use_density_weight);
                        break;
                    }
                }
            }
        }
    }
    static bool update_every_frame = false;
    ImGui::Checkbox("update_every_frame", &update_every_frame);
    if(update_every_frame || ImGui::Button("compute_step")){
        if(state->scene.valid(id)) {
            switch (static_cast<LopType>(method_idx)) {
                case LopType::lop : {
                    auto &projection = state->scene.get_or_emplace<lop>(id);
                    projection.compute_step();
                    break;
                }
                case LopType::wlop : {
                    auto &projection = state->scene.get_or_emplace<wlop>(id);
                    projection.compute_step();
                    break;
                }
                case LopType::flop : {
                    auto &projection = state->scene.get_or_emplace<flop>(id);
                    projection.compute_step();
                    break;
                }
                case LopType::clop : {
                    auto &projection = state->scene.get_or_emplace<clop>(id);
                    projection.compute_step();
                    break;
                }
            }
        }
    }
}

}
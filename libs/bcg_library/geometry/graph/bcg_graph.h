//
// Created by alex on 10.10.20.
//

#ifndef BCG_GRAPHICS_BCG_GRAPH_H
#define BCG_GRAPHICS_BCG_GRAPH_H

#include "geometry/point_cloud/bcg_point_cloud.h"

namespace bcg {

struct halfedge_graph : public point_cloud {
    struct vertex_connectivity {
        halfedge_handle h;

        friend std::ostream &operator<<(std::ostream &stream, const vertex_connectivity &value) {
            stream << "h: " << value.h.idx;
            return stream;
        }

        operator size_t() const {
            return h.idx;
        }
    };

    struct halfedge_connectivity {
        vertex_handle v;
        halfedge_handle nh;
        halfedge_handle ph;
        face_handle f;

        friend std::ostream &operator<<(std::ostream &stream, const halfedge_connectivity &value) {
            stream << "v: " << value.v.idx;
            stream << " nh: " << value.nh.idx;
            stream << " ph: " << value.ph.idx;
            stream << " f: " << value.f.idx;
            return stream;
        }
    };

    halfedge_container halfedges;
    edge_container edges;
    property<vertex_connectivity, 1> vconn;
    property<halfedge_connectivity, 4> hconn;
    property<bool, 1> halfedges_deleted;
    property<bool, 1> edges_deleted;
    size_t size_halfedges_deleted;
    size_t size_edges_deleted;

    halfedge_graph();

    ~halfedge_graph() override = default;

    void assign(const halfedge_graph &other);

    halfedge_graph &operator=(const halfedge_graph &other);

    size_t num_edges() const;

    size_t num_halfedges() const;

    bool is_valid(halfedge_handle h) const;

    bool is_valid(edge_handle e) const;

    bool has_garbage() const override;

    void garbage_collection() override;

    bool is_isolated(vertex_handle v) const;

    virtual bool is_boundary(vertex_handle v) const;

    virtual bool is_boundary(halfedge_handle h) const;

    virtual bool is_boundary(edge_handle e) const;

    void delete_vertex(vertex_handle v) override;

    halfedge_handle get_halfedge(vertex_handle v) const;

    void set_halfedge(vertex_handle v, halfedge_handle h);

    vertex_handle get_to_vertex(halfedge_handle h) const;

    void set_vertex(halfedge_handle h, vertex_handle v);

    vertex_handle get_from_vertex(halfedge_handle h) const;

    halfedge_handle get_next(halfedge_handle h) const;

    void set_next(halfedge_handle h, halfedge_handle nh);

    halfedge_handle get_prev(halfedge_handle h) const;

    halfedge_handle get_opposite(halfedge_handle h) const;

    halfedge_handle rotate_cw(halfedge_handle h) const;

    halfedge_handle rotate_ccw(halfedge_handle h) const;

    edge_handle get_edge(halfedge_handle h) const;

    halfedge_handle get_halfedge(edge_handle e, bool i) const;

    vertex_handle get_vertex(edge_handle e, bool i) const;

    size_t get_valence(vertex_handle v) const;

    bcg_scalar_t get_length(edge_handle e) const;

    point_cloud::position_t get_center(halfedge_handle h) const;

    point_cloud::position_t get_center(edge_handle e) const;

    point_cloud::position_t get_vector(halfedge_handle h) const;

    point_cloud::position_t get_vector(edge_handle e) const;

    halfedge_handle add_edge(vertex_handle v0, vertex_handle v1);

    virtual void remove_edge(edge_handle e);

    halfedge_handle find_halfedge(vertex_handle v0, vertex_handle v1) const;

    edge_handle find_edge(vertex_handle v0, vertex_handle v1) const;

    struct vertex_around_vertex_circulator {
        const halfedge_graph *ds;
        halfedge_handle halfedge;
        bool active;

        explicit vertex_around_vertex_circulator(const halfedge_graph *ds = nullptr, vertex_handle v = vertex_handle());

        bool operator==(const vertex_around_vertex_circulator &rhs) const;

        bool operator!=(const vertex_around_vertex_circulator &rhs) const;

        vertex_around_vertex_circulator &operator++();

        vertex_around_vertex_circulator &operator--();

        vertex_handle operator*() const;

        operator bool() const;

        vertex_around_vertex_circulator &begin();

        vertex_around_vertex_circulator &end();
    };

    vertex_around_vertex_circulator get_vertices(vertex_handle v) const;

    struct halfedge_around_vertex_circulator {
        const halfedge_graph *ds;
        halfedge_handle halfedge;
        bool active;

        explicit halfedge_around_vertex_circulator(const halfedge_graph *ds = nullptr,
                                                   vertex_handle v = vertex_handle());

        bool operator==(const halfedge_around_vertex_circulator &rhs) const;

        bool operator!=(const halfedge_around_vertex_circulator &rhs) const;

        halfedge_around_vertex_circulator &operator++();

        halfedge_around_vertex_circulator &operator--();

        halfedge_handle operator*() const;

        operator bool() const;

        halfedge_around_vertex_circulator &begin();

        halfedge_around_vertex_circulator &end();
    };

    halfedge_around_vertex_circulator get_halfedges(vertex_handle v) const;

    std::vector<VectorI<2>> get_connectivity() const;

    halfedge_handle new_edge(vertex_handle v0, vertex_handle v1);


    edge_handle find_closest_edge(const halfedge_graph::position_t &point);

    std::vector<edge_handle> find_closest_k_edges(const halfedge_graph::position_t &point, size_t k);

    std::vector<edge_handle> find_closest_edges_radius(const halfedge_graph::position_t &point, bcg_scalar_t radius);

    edge_handle find_closest_edge_in_neighborhood(vertex_handle v, const halfedge_graph::position_t &point);

    std::string to_string() const override;
protected:
    void mark_edge_deleted(edge_handle e);
};

std::ostream &operator<<(std::ostream &stream, const halfedge_graph &graph);

}

#endif //BCG_GRAPHICS_BCG_GRAPH_H

#pragma once

#ifndef BCG_GRAPHICS_BCG_MESH_UNILATERAL_HUBER_H
#define BCG_GRAPHICS_BCG_MESH_UNILATERAL_HUBER_H

#include "geometry/mesh/bcg_mesh.h"

namespace bcg {
	void mesh_unilateral_huber(halfedge_mesh& mesh,
		bcg_scalar_t sigma_g, size_t parallel_grain_size = 1024);
}

#endif //BCG_GRAPHICS_BCG_MESH_UNILATERAL_HUBER_H
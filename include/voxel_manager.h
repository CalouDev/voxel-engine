#ifndef VOXEL_MANAGER_H
#define VOXEL_MANAGER_H

#include <vector>

#include "globals.h"
#include "voxel.h"

class VoxelManager {
private:
    std::vector<Voxel> manager;

public:
    VoxelManager();

    void add(Voxel voxel);
    void use();
    void destroy();

    std::vector<Voxel> getManager() const { return manager; }
};

#endif

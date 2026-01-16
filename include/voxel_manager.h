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

    void use();
};

#endif

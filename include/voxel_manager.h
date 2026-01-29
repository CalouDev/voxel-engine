#ifndef VOXEL_MANAGER_H
#define VOXEL_MANAGER_H

#include "globals.h"
#include "voxel.h"
#include "flat_voxel.h"

class VoxelManager {
private:
    std::vector<Voxel> voxel_manager;
    std::vector<FlatVoxel> flat_voxel_manager;
public:
    VoxelManager() = default;

    void addVoxel(Voxel voxel);
    void addFlatVoxel(FlatVoxel voxel);
    void use();
    void destroy();

    std::vector<Voxel> getVoxelManager() const { return voxel_manager; }
    std::vector<FlatVoxel> getFlatVoxelManager() const { return flat_voxel_manager; }
};

#endif

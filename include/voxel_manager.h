#ifndef VOXEL_MANAGER_H
#define VOXEL_MANAGER_H

/*
Merge voxel_manager and flat_voxel_manager into a single object (voxel), with two constructors:
 - 3 coords for voxel
 - 2 coords for flat voxel
*/

#include <vector>

#include "globals.h"
#include "voxel.h"
#include "flat_voxel.h"

class VoxelManager {
private:
    std::vector<Voxel> voxel_manager;
    std::vector<FlatVoxel> flat_voxel_manager;
public:
    VoxelManager();

    void addVoxel(Voxel voxel);
    void addFlatVoxel(FlatVoxel voxel);
    void use();
    void destroy();

    std::vector<Voxel> getVoxelManager() const { return voxel_manager; }
    std::vector<FlatVoxel> getFlatVoxelManager() const { return flat_voxel_manager; }
};

#endif

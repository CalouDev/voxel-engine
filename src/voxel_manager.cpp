#include "../include/voxel_manager.h"

VoxelManager::VoxelManager() {}

void VoxelManager::addVoxel(Voxel voxel) {
    voxel_manager.push_back(voxel);
}

void VoxelManager::addFlatVoxel(FlatVoxel voxel) {
    flat_voxel_manager.push_back(voxel);
}

void VoxelManager::use() {
    for (Voxel& v : voxel_manager) {
        v.use();
    }

    for (FlatVoxel& v : flat_voxel_manager) {
        v.use();
    }
}

void VoxelManager::destroy() {
    for (Voxel& v : voxel_manager) {
        v.destroy();
    }

    for (FlatVoxel& v : flat_voxel_manager) {
        v.destroy();
    }
}

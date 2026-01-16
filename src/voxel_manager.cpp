#include "../include/voxel_manager.h"

VoxelManager::VoxelManager() {}

void VoxelManager::add(Voxel voxel) {
    manager.push_back(voxel);
}

void VoxelManager::use() {
    for (Voxel& v : manager) {
        v.use();
    }
}

void VoxelManager::destroy() {
    for (Voxel& v : manager) {
        v.destroy();
    }
}

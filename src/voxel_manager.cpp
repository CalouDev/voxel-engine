#include "../include/voxel_manager.h"

VoxelManager::VoxelManager() {}

void VoxelManager::use() {
    for (Voxel v : manager) {
        v.use();
    }
}

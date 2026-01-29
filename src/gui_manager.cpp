#include "../include/gui_manager.h"

void GuiManager::initialize() {
    manager["inventory"] = GuiComponent(glm::vec2(WINDOW::WIDTH/2.0f - TEX::GUI::INVENTORY::WIDTH/2.0f, 10.0f),
                                        TEX::GUI::INVENTORY::SIDE,
                                        glm::vec2(TEX::GUI::INVENTORY::TEX_WIDTH, TEX::GUI::INVENTORY::TEX_HEIGHT),
                                        TEX::GUI::INVENTORY::WIDTH, TEX::GUI::INVENTORY::HEIGHT);

    manager["inventory_case_selected"] = GuiComponent(glm::vec2(WINDOW::WIDTH/2.0f - TEX::GUI::INVENTORY::WIDTH/2.0f - 1.0f, 8.0f),
                                                        TEX::GUI::INVENTORY_CASE_SELECTED::SIDE,
                                                        glm::vec2(TEX::GUI::INVENTORY_CASE_SELECTED::TEX_WIDTH, TEX::GUI::INVENTORY_CASE_SELECTED::TEX_HEIGHT),
                                                        TEX::GUI::INVENTORY_CASE_SELECTED::WIDTH, TEX::GUI::INVENTORY_CASE_SELECTED::HEIGHT);
}

void GuiManager::bind(std::string gui_name) {
    glBindVertexArray(manager[gui_name].getVAO());
}

void GuiManager::destroy() {
    for (auto& [_, gui] : manager) {
        gui.destroy();
    }
}

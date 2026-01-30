#include "../include/gui_manager.h"

void GuiManager::initialize() {
    manager.push_back(GuiComponent(glm::vec2(WINDOW::WIDTH/2.0f - TEX::GUI::INVENTORY::WIDTH/2.0f, 10.0f),
                                        TEX::GUI::INVENTORY::SIDE,
                                        glm::vec2(TEX::GUI::INVENTORY::TEX_WIDTH, TEX::GUI::INVENTORY::TEX_HEIGHT),
                                        TEX::GUI::INVENTORY::WIDTH, TEX::GUI::INVENTORY::HEIGHT));

    manager.push_back(GuiComponent(glm::vec2(WINDOW::WIDTH/2.0f - TEX::GUI::INVENTORY::WIDTH/2.0f - 2.0f, 7.0f),
                                                        TEX::GUI::INVENTORY_CASE_SELECTED::SIDE,
                                                        glm::vec2(TEX::GUI::INVENTORY_CASE_SELECTED::TEX_WIDTH, TEX::GUI::INVENTORY_CASE_SELECTED::TEX_HEIGHT),
                                                        TEX::GUI::INVENTORY_CASE_SELECTED::WIDTH, TEX::GUI::INVENTORY_CASE_SELECTED::HEIGHT));

    manager.push_back(GuiComponent(glm::vec2(WINDOW::WIDTH/2.0f - TEX::GUI::CROSSHAIR::WIDTH/2.0f, WINDOW::HEIGHT/2.0f + TEX::GUI::CROSSHAIR::HEIGHT/2.0f),
                                    TEX::GUI::CROSSHAIR::SIDE,
                                    glm::vec2(TEX::GUI::CROSSHAIR::TEX_WIDTH, TEX::GUI::CROSSHAIR::TEX_HEIGHT),
                                    TEX::GUI::CROSSHAIR::WIDTH, TEX::GUI::CROSSHAIR::HEIGHT));
}

void GuiManager::drawAll() {
    for (auto& gui : manager) {
        glBindVertexArray(gui.getVAO());
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void GuiManager::destroy() {
    for (auto& gui : manager) {
        gui.destroy();
    }
}

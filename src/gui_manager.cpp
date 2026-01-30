#include "../include/gui_manager.h"

void GuiManager::initialize() {
    for (GuiData gui : GUI) {
        manager.push_back(GuiComponent(gui));
    }
}

void GuiManager::drawAll() {
    for (GuiComponent& gui : manager) {
        glBindVertexArray(gui.getVAO());
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void GuiManager::destroy() {
    for (GuiComponent& gui : manager) {
        gui.destroy();
    }
}

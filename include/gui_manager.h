#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "globals.h"
#include "gui_component.h"

class GuiManager {
private:
    std::vector<GuiComponent> manager;
public:
    GuiManager() = default;

    void initialize();
    void drawAll();
    void destroy();

    inline std::vector<GuiComponent> getManager() { return manager; }
};

#endif

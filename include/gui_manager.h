#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "globals.h"
#include "gui_component.h"

class GuiManager {
private:
    std::unordered_map<std::string, GuiComponent> manager;
public:
    GuiManager() = default;

    void initialize();
    void bind(std::string gui_name);
    void destroy();

    inline std::unordered_map<std::string, GuiComponent> getManager() { return manager; }
};

#endif

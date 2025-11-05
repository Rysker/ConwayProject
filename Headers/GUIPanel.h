#ifndef GUIPANEL_H
#define GUIPANEL_H

#include "imgui.h"
#include "AppState.h"
#include "ICommand.h"
#include <map>
#include <string>
#include <memory>

class GUIPanel
{
    public:
        GUIPanel(AppState& state, const char* simName);
        void draw(std::map<std::string, std::unique_ptr<ICommand>>& commands);
        char* getFilenameBufferPtr();

    private:
        AppState& m_state;

        const char* m_simName;
        char m_filenameBuffer[128];
        float m_menuWidth;
};

#endif //GUIPANEL_H
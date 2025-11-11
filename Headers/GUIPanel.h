#ifndef GUIPANEL_H
#define GUIPANEL_H

#include "imgui.h"
#include "AppState.h"
#include "ICommand.h"
#include "IAppControls.h"
#include <map>
#include <string>
#include <memory>
#include <vector>

class GUIPanel
{
    public:
        GUIPanel(AppState& state, IAppControls& appControls);
        void draw(std::map<std::string, std::unique_ptr<ICommand>>& commands);
        char* getFilenameBufferPtr();

    private:
        AppState& state_;
        IAppControls& appControls_;

        int currentImplIndex_;
        char filenameBuffer_[128];
        float menuWidth_;
        std::vector<const char*> implementations_;
};

#endif //GUIPANEL_H
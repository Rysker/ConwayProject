#ifndef PAUSECOMMAND_H
#define PAUSECOMMAND_H

#include "ICommand.h"
#include "IAppControls.h"

class PauseCommand : public ICommand
{
    IAppControls& controls_;

    public:
        PauseCommand(IAppControls& controls);
        void execute() override;
};

#endif //PAUSECOMMAND_H
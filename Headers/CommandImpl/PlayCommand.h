#ifndef PLAYCOMMAND_H
#define PLAYCOMMAND_H

#include "ICommand.h"
#include "IAppControls.h"

class PlayCommand : public ICommand
{
    IAppControls& controls_;

    public:
        PlayCommand(IAppControls& controls);
        void execute() override;
};

#endif //PLAYCOMMAND_H
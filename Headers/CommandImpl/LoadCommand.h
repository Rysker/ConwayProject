#ifndef LOADCOMMAND_H
#define LOADCOMMAND_H

#include "ICommand.h"
#include "IAppControls.h"

class LoadCommand : public ICommand
{
    IAppControls& controls_;
    const char* filenameBuffer_;

    public:
        LoadCommand(IAppControls& controls, const char* buffer);
        void execute() override;
};

#endif //LOADCOMMAND_H
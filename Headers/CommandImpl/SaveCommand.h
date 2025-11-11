#ifndef SAVECOMMAND_H
#define SAVECOMMAND_H

#include "ICommand.h"
#include "IAppControls.h"

class SaveCommand : public ICommand
{
    IAppControls& controls_;
    const char* filenameBuffer_;

    public:
        SaveCommand(IAppControls& controls, const char* buffer);
        void execute() override;
};

#endif //SAVECOMMAND_H
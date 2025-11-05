#ifndef CLEARCOMMAND_H
#define CLEARCOMMAND_H

#include "ICommand.h"
#include "IAppControls.h"

class ClearCommand : public ICommand
{
    IAppControls& m_controls;

    public:
        ClearCommand(IAppControls& controls);
        void execute() override;
};

#endif //CLEARCOMMAND_H
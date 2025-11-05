#ifndef SAVECOMMAND_H
#define SAVECOMMAND_H

#include "ICommand.h"
#include "IAppControls.h"

class SaveCommand : public ICommand
{
    IAppControls& m_controls;
    const char* m_filenameBuffer;

    public:
        SaveCommand(IAppControls& controls, const char* buffer);
        void execute() override;
};

#endif //SAVECOMMAND_H
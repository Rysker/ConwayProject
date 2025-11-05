#include "CommandImpl/ClearCommand.h"

ClearCommand::ClearCommand(IAppControls& controls): m_controls(controls){}

void ClearCommand::execute()
{
    m_controls.clearBoard();
}
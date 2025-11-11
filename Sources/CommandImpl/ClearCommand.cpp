#include "CommandImpl/ClearCommand.h"

ClearCommand::ClearCommand(IAppControls& controls): controls_(controls){}

void ClearCommand::execute()
{
    controls_.clearBoard();
}
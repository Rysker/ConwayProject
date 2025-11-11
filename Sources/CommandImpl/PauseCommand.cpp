#include "CommandImpl/PauseCommand.h"

PauseCommand::PauseCommand(IAppControls& controls): controls_(controls){}

void PauseCommand::execute()
{
    controls_.pause();
}
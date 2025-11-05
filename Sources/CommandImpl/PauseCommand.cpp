#include "CommandImpl/PauseCommand.h"

PauseCommand::PauseCommand(IAppControls& controls): m_controls(controls){}

void PauseCommand::execute()
{
    m_controls.pause();
}
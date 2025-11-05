#include "CommandImpl/PlayCommand.h"

PlayCommand::PlayCommand(IAppControls& controls): m_controls(controls){}

void PlayCommand::execute()
{
    m_controls.play();
}
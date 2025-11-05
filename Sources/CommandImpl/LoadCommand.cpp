#include "CommandImpl/LoadCommand.h"

LoadCommand::LoadCommand(IAppControls& controls, const char* buffer): m_controls(controls), m_filenameBuffer(buffer){}

void LoadCommand::execute()
{
    m_controls.loadState(m_filenameBuffer);
}
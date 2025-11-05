#include "CommandImpl/SaveCommand.h"

SaveCommand::SaveCommand(IAppControls& controls, const char* buffer): m_controls(controls), m_filenameBuffer(buffer){}

void SaveCommand::execute()
{
    m_controls.saveState(m_filenameBuffer);
}
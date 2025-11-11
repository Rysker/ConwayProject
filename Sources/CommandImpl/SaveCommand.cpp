#include "CommandImpl/SaveCommand.h"

SaveCommand::SaveCommand(IAppControls& controls, const char* buffer): controls_(controls), filenameBuffer_(buffer){}

void SaveCommand::execute()
{
    controls_.saveState(filenameBuffer_);
}
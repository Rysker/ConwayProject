#include "CommandImpl/LoadCommand.h"

LoadCommand::LoadCommand(IAppControls& controls, const char* buffer): controls_(controls), filenameBuffer_(buffer){}

void LoadCommand::execute()
{
    controls_.loadState(filenameBuffer_);
}
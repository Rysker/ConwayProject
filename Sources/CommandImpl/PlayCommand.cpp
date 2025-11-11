#include "CommandImpl/PlayCommand.h"

PlayCommand::PlayCommand(IAppControls& controls): controls_(controls){}

void PlayCommand::execute()
{
    controls_.play();
}
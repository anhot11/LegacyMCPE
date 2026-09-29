#include "TrialMode.h"

#include "app/common/Tutorial/FullTutorial.h"
#include "app/common/Tutorial/FullTutorialMode.h"

class ClientConnection;
class Minecraft;

TrialMode::TrialMode(int iPad, Minecraft* minecraft,
                     ClientConnection* connection)
    : FullTutorialMode(iPad, minecraft, connection) {
    tutorial = new FullTutorial(iPad, true);
}
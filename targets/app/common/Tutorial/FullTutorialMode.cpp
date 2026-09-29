#include "FullTutorialMode.h"

#include "FullTutorial.h"
#include "app/common/Tutorial/Tutorial.h"
#include "app/common/Tutorial/TutorialMode.h"
#include "minecraft/client/Minecraft.h"

class ClientConnection;

FullTutorialMode::FullTutorialMode(int iPad, Minecraft* minecraft,
                                   ClientConnection* connection)
    : TutorialMode(iPad, minecraft, connection) {
    tutorial = new FullTutorial(iPad);
    minecraft->playerStartedTutorial(iPad);
}

bool FullTutorialMode::isTutorial() {
    return !tutorial->m_fullTutorialComplete;
}
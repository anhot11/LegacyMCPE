#include "ConsoleGameMode.h"

#include "app/common/Tutorial/Tutorial.h"
#include "app/common/Tutorial/TutorialMode.h"

class ClientConnection;
class Minecraft;

ConsoleGameMode::ConsoleGameMode(int iPad, Minecraft* minecraft,
                                 ClientConnection* connection)
    : TutorialMode(iPad, minecraft, connection) {
    tutorial = new Tutorial(iPad);
}
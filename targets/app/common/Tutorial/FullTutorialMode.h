#pragma once
#include "TutorialMode.h"

class ClientConnection;
class Minecraft;

class FullTutorialMode : public TutorialMode {
public:
    FullTutorialMode(int iPad, Minecraft* minecraft,
                     ClientConnection* connection);

    virtual bool isImplemented() { return true; }

    virtual bool isTutorial();
};
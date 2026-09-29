#pragma once
#include <memory>

#include "AbstractContainerScreen.h"

class Container;

class ContainerScreen : public AbstractContainerScreen {
private:
    std::shared_ptr<Container> inventory;
    std::shared_ptr<Container> container;

    int containerRows;

public:
    ContainerScreen(std::shared_ptr<Container> inventory,
                    std::shared_ptr<Container> container);

protected:
    virtual void renderLabels() override;
    virtual void renderBg(float a) override;
};
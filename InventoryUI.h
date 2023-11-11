/*
* Created another screen for the inventory
* By: Prasanna Joshi
*/
#pragma once

#include "UIScreen.h"

class Game;
class FPSActor;

class InventoryUI : public UIScreen
{
public:
    InventoryUI(class Game* game);
    ~InventoryUI();

    Game* GetGame() const;

    void HandleKeyPress(int key) override;
    void Update(float deltaTime) override;

    void OnClickShoe();
    void OnClickGlove();

private:
    Game* mGame;
    FPSActor* mFPSActor;

};
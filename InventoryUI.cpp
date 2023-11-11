/*
* Created another screen for the inventory
* By: Prasanna Joshi
*/

#include "InventoryUI.h"
#include "Game.h"
#include "FPSActor.h"
#include <iostream>
#include <SDL/SDL_keycode.h>

InventoryUI::InventoryUI(Game* game)
    : UIScreen(game),
    mGame(game),
    mFPSActor(nullptr)
{
    mGame->SetState(Game::EPaused);
    SetRelativeMouseMode(false);
    SetTitle("InventoryTitle");
    AddButton("ShoeButton", [this]() { OnClickShoe(); Close(); });
    AddButton("GloveButton", [this]() { OnClickGlove(); Close(); });
}

InventoryUI::~InventoryUI()
{
    SetRelativeMouseMode(true);
    mGame->SetState(Game::EGameplay);
}

Game* InventoryUI::GetGame() const
{
    return mGame;
}

void InventoryUI::HandleKeyPress(int key)
{
    UIScreen::HandleKeyPress(key);

    if (key == SDLK_i)
    {
        Close();
    }
}

void InventoryUI::Update(float deltaTime)
{
    if (!mFPSActor)
    {
        mFPSActor = GetGame()->GetPlayer();
    }
}

void InventoryUI::OnClickShoe()
{
    if (mFPSActor)
    {
        mFPSActor->ClickShoeButton();
    }
}

void InventoryUI::OnClickGlove()
{
    if (mFPSActor)
    {
        mFPSActor->ClickGloveButton();
    }
}
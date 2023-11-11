// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

//Created by Rebecca Morris, edited Pause Menu
//Will show a screen when the player dies

#include "DeathScreen.h"
#include "Game.h"
#include "DialogBox.h"
#include <SDL/SDL.h>

DeathScreen::DeathScreen(Game* game)
	:UIScreen(game)
{
	mGame->SetState(Game::EPaused);
	SetRelativeMouseMode(false);
	SetTitle("DeathTitle");
	/*AddButton("ResumeButton", [this]() {
		Close();
		});*/
	AddButton("QuitButton", [this]() {
		new DialogBox(mGame, "QuitText",
		[this]() {
				mGame->SetState(Game::EQuit);
			});
		});
}

DeathScreen::~DeathScreen()
{
	SetRelativeMouseMode(true);
	mGame->SetState(Game::EGameplay);
}

void DeathScreen::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);

	if (key == SDLK_ESCAPE)
	{
		Close();
	}
}

// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

//Created by Rebecca Morris, edited Pause Menu
//Will show a screen when the player dies

#include "WinScreen.h"
#include "Game.h"
#include "DialogBox.h"
#include <SDL/SDL.h>

WinScreen::WinScreen(Game* game)
	:UIScreen(game)
{
	mGame->SetState(Game::EPaused);
	SetRelativeMouseMode(false);
	SetTitle("WinTitle");
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

WinScreen::~WinScreen()
{
	SetRelativeMouseMode(true);
	mGame->SetState(Game::EGameplay);
}

void WinScreen::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);

	if (key == SDLK_ESCAPE)
	{
		Close();
	}
}

// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

//Created by Rebecca Morris, edited Pause Menu
//Will show a screen when the player dies

#pragma once
#include "UIScreen.h"

class WinScreen : public UIScreen
{
public:
	WinScreen(class Game* game);
	~WinScreen();

	void HandleKeyPress(int key) override;
};

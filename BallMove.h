// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "MoveComponent.h"
#include "SoundEvent.h"

class BallMove : public MoveComponent
{
public:
	BallMove(class Actor* owner);

	void SetPlayer(Actor* player) { mPlayer = player; }
	void Update(float deltaTime) override;
	bool GetKillCount() { return mKillCount; }
	void SetKillCount(float RecordKill) { mKillCount = RecordKill; }
protected:
	class Actor* mPlayer;
	float mKillCount;
	float mLastMonsterDeath; //Rebecca Morris
};
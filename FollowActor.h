// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"

class FollowActor : public Actor
{
public:
	FollowActor(class Game* game);
	void UpdateActor(float deltaTime) override;

	void ActorInput(const uint8_t* keys) override;

	void SetVisible(bool visible);
	void FixCollisions();
private:
	class BoxComponent* mBoxComp;
	class MoveComponent* mMoveComp;
	class FollowCamera* mCameraComp;
	class SkeletalMeshComponent* mMeshComp;
	bool mMoving;
};

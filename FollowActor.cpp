// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "FollowActor.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "FollowCamera.h"
#include "MoveComponent.h"
#include "BallActor.h" //Rebecca Morris
#include "BallMove.h" //Rebecca Morris
#include "BoxComponent.h"
#include "PlaneActor.h"

FollowActor::FollowActor(Game* game)
	:Actor(game)
	, mMoving(false)
{
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/CatWarrior.gpmesh"));
	mMeshComp->SetSkeleton(game->GetSkeleton("Assets/CatWarrior.gpskel"));
	mMeshComp->PlayAnimation(game->GetAnimation("Assets/CatActionIdle.gpanim"));
	SetPosition(Vector3(0.0f, 0.0f, -100.0f));

	// Add a box component
	mBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(-25.0f, -25.0f, -87.5f),
		Vector3(25.0f, 25.0f, 87.5f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);
	mMoveComp = new MoveComponent(this);
	mCameraComp = new FollowCamera(this);
	mCameraComp->SnapToIdeal();
}

void FollowActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	FixCollisions();
	BallCoolDown -= deltaTime; //Rebecca Morris
}

void FollowActor::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;
	// wasd movement
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 400.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		angularSpeed -= Math::Pi;
	}
	if (keys[SDL_SCANCODE_D])
	{
		angularSpeed += Math::Pi;
	}
	if (keys[SDL_SCANCODE_SPACE])
	{
		Vector3 pos = GetPosition();
		pos.y += 2.0f;
		SetPosition(pos);
		if (pos.y >= 2.0f)
		{
			pos.y = 0.0f;
			SetPosition(pos);
		}
	}

	// Did we just start moving?
	if (!mMoving && !Math::NearZero(forwardSpeed))
	{
		mMoving = true;
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatRunSprint.gpanim"), 1.25f);
	}
	// Or did we just stop moving?
	else if (mMoving && Math::NearZero(forwardSpeed))
	{
		mMoving = false;
		mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatActionIdle.gpanim"));
	}
	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetAngularSpeed(angularSpeed);
}

void FollowActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}

void FollowActor::FixCollisions()
{
	// Need to recompute my world transform to update world box
	ComputeWorldTransform();

	const AABB& playerBox = mBoxComp->GetWorldBox();
	Vector3 pos = GetPosition();

	auto& planes = GetGame()->GetPlanes();
	for (auto pa : planes)
	{
		// Do we collide with this PlaneActor?
		const AABB& planeBox = pa->GetBox()->GetWorldBox();
		if (Intersect(playerBox, planeBox))
		{
			// Calculate all our differences
			float dx1 = planeBox.mMax.x - playerBox.mMin.x;
			float dx2 = planeBox.mMin.x - playerBox.mMax.x;
			float dy1 = planeBox.mMax.y - playerBox.mMin.y;
			float dy2 = planeBox.mMin.y - playerBox.mMax.y;
			float dz1 = planeBox.mMax.z - playerBox.mMin.z;
			float dz2 = planeBox.mMin.z - playerBox.mMax.z;

			// Set dx to whichever of dx1/dx2 have a lower abs
			float dx = Math::Abs(dx1) < Math::Abs(dx2) ?
				dx1 : dx2;
			// Ditto for dy
			float dy = Math::Abs(dy1) < Math::Abs(dy2) ?
				dy1 : dy2;
			// Ditto for dz
			float dz = Math::Abs(dz1) < Math::Abs(dz2) ?
				dz1 : dz2;

			// Whichever is closest, adjust x/y position
			if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
			{
				pos.x += dx;
			}
			else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			// Need to set position and update box component
			SetPosition(pos);
			mBoxComp->OnUpdateWorldTransform();
		}
	}
}
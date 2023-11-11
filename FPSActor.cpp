// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "FPSActor.h"
#include "MoveComponent.h"
#include "SDL/SDL_scancode.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "Game.h"
#include "AudioComponent.h"
#include "FPSCamera.h"
#include "MeshComponent.h"
#include "BallActor.h"
#include "BoxComponent.h"
#include "PlaneActor.h"
#include "PhysWorld.h"
#include "Actor.h"
#include "MonsterActor.h"
#include "DeathScreen.h"
#include "BallMove.h"
#include <iostream>
//#include "WinScreen.h"

FPSActor::FPSActor(Game* game)
	:Actor(game)
{
	mMoveComp = new MoveComponent(this);
	mAudioComp = new AudioComponent(this);
	mLastFootstep = 0.0f;
	mFootstep = mAudioComp->PlayEvent("event:/Walking"); //Rebecca Morris
	mFootstep.SetPaused(true);
	mLastMonsterDeath = 0.0f; //Rebecca Morris
	mMonsterDeath = mAudioComp->PlayEvent("event:/MonsterDeath"); //Rebecca Morris
	mMonsterDeath.SetPaused(true); //Rebecca Morris
	mPlayerDirection = Vector3(0.0f, 0.0f, 0.0f);
	mBasicEvolution = false;
	mKillCount = 0.0;
	mMovementSpeed = 400.0;

	mCameraComp = new FPSCamera(this);

	mFPSModel = new Actor(game);
	mFPSModel->SetScale(0.75f);
	mMeshComp = new MeshComponent(mFPSModel);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/Rifle.gpmesh"));

	// Add a box component
	mBoxComp = new BoxComponent(this);
	AABB myBox(Vector3(-25.0f, -25.0f, -87.5f),
		Vector3(25.0f, 25.0f, 87.5f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);
	// Prasanna
	shoeButtonClicked = false;
}

void FPSActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	FixCollisions();

	BallCoolDown -= deltaTime;

	// Update position of FPS model relative to actor position
	const Vector3 modelOffset(Vector3(10.0f, 10.0f, -10.0f));
	Vector3 modelPos = GetPosition();
	modelPos += GetForward() * modelOffset.x;
	modelPos += GetRight() * modelOffset.y;
	modelPos.z += modelOffset.z;
	mFPSModel->SetPosition(modelPos);
	// Initialize rotation to actor rotation
	Quaternion q = GetRotation();
	// Rotate by pitch from camera
	q = Quaternion::Concatenate(q, Quaternion(GetRight(), mCameraComp->GetPitch()));
	mFPSModel->SetRotation(q);

	GetInjured(); //Rebecca Morris
}

void FPSActor::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;

	// Prasanna
	/*if (gloveButtonClicked)
	{
		BallCoolDown = 0.15f;
	}*/

	// wasd movement
	if (!(keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_D])) //Rebecca Morris
	{
		mFootstep.SetPaused(true);
		//mLastFootstep = 0.5f;
	}
	else //Rebecca Morris
	{
		mFootstep.SetPaused(false);
		//mLastFootstep = 0.0f;
	}

	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += mMovementSpeed;
		// Prasanna
		if (shoeButtonClicked)
		{
			forwardSpeed += 1200.0f;
		}
		/*if (mKillCount >= 1)
		{
			forwardSpeed += 100.0f;
			
		}
		if (mKillCount >= 5)
		{
			forwardSpeed += 200.0f;
		}*/
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= mMovementSpeed;
		// Prasanna
		if (shoeButtonClicked)
		{
			forwardSpeed -= 1200.0f;
		}
		/*if (mKillCount >= 1)
		{
			forwardSpeed -= 100.0f;
			
		}
		if (mKillCount >= 5)
		{
			forwardSpeed -= 200.0f;
		}*/
	}
	if (keys[SDL_SCANCODE_A])
	{
		strafeSpeed -= mMovementSpeed;
		// Prasanna
		if (shoeButtonClicked)
		{
			strafeSpeed -= 1200.0f;
		}
		/*if (mKillCount >= 1)
		{
			strafeSpeed -= 100.0f;
			
		}
		if (mKillCount >= 5)
		{
			strafeSpeed -= 200.0f;
		}*/
	}
	if (keys[SDL_SCANCODE_D])
	{
		strafeSpeed += mMovementSpeed;
		// Prasanna
		if (shoeButtonClicked)
		{
			strafeSpeed += 1200.0f;
		}
		/*if (mKillCount >= 1)
		{
			strafeSpeed += 100.0f;
			
		}
		if (mKillCount >= 5)
		{
			strafeSpeed += 200.0f;
		}*/
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetStrafeSpeed(strafeSpeed);

	// Mouse movement
	// Get relative movement from SDL
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	// Assume mouse movement is usually between -500 and +500
	const int maxMouseSpeed = 500;
	// Rotation/sec at maximum speed
	const float maxAngularSpeed = Math::Pi * 8;
	float angularSpeed = 0.0f;
	if (x != 0)
	{
		// Convert to ~[-1.0, 1.0]
		angularSpeed = static_cast<float>(x) / maxMouseSpeed;
		// Multiply by rotation/sec
		angularSpeed *= maxAngularSpeed;
	}
	mMoveComp->SetAngularSpeed(angularSpeed);

	// Compute pitch
	const float maxPitchSpeed = Math::Pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0)
	{
		// Convert to ~[-1.0, 1.0]
		pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	mCameraComp->SetPitchSpeed(pitchSpeed);
}

void FPSActor::Shoot()
{
	// Get start point (in center of screen on near plane)
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	Vector3 start = GetGame()->GetRenderer()->Unproject(screenPoint);
	// Get end point (in center of screen, between near and far)
	screenPoint.z = 0.9f;
	Vector3 end = GetGame()->GetRenderer()->Unproject(screenPoint);
	// Get direction vector
	Vector3 dir = end - start;
	dir.Normalize();
	SetPlayerDirection(dir);
	// Spawn a ball
	BallActor* ball = new BallActor(GetGame());
	ball->SetPlayer(this);
	ball->SetPosition(start + dir * 20.0f);
	// Rotate the ball to face new direction
	ball->RotateToNewForward(dir);
	// Play shooting sound
	mAudioComp->PlayEvent("event:/Gunshot"); //Rebecca Morris
}

void FPSActor::SetFootstepSurface(float value)
{
	// Pause here because the way I setup the parameter in FMOD
	// changing it will play a footstep
	mFootstep.SetPaused(true);
	mFootstep.SetParameter("Surface", value);
}

void FPSActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}

void FPSActor::SetPlayerDirection(Vector3 currentDirection)
{
	mPlayerDirection = currentDirection;
}

void FPSActor::FixCollisions() // Simply added by Adam C from Madhav code
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

void FPSActor::GetInjured() //Rebecca Morris
//Will allow for the player to take damage when the enemy runs into them 
{
	//auto& monsters = GetGame()->GetMonsters();

	//sPhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld* phys = mBoxComp->GetOwner()->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;

	const float segmentLength = 30.0f;
	//Vector3 start = mBoxComp->GetOwner()->GetPosition();
	Vector3 start = this->GetPosition();
	//Vector3 dir = mBoxComp->GetOwner()->GetForward();
	Vector3 dir = this->GetForward();
	Vector3 end = start + dir * segmentLength;

	// Create line segment
	LineSegment l(start, end);

	// (Don't collide vs our player)
	if (phys->SegmentCast(l, info) && info.mActor != mFPSModel)
	{
		// If we collided, reflect the ball about the normal
		//dir = Vector3::Reflect(dir, info.mNormal);
		//mBoxComp->GetOwner()->RotateToNewForward(dir);
		// Did we get hit?
		MonsterActor* monster = dynamic_cast<MonsterActor*>(info.mActor);
		if (monster)
		{
			this->DamageActor(this->GetDamageAmt()); //Rebecca Morris

			//monster->SetHealthState(); //Rebecca Morris

			// SetSate of self to dead if health is out
			if (this->GetHealth() <= 0)
			{
				new DeathScreen(this->GetGame()); //Rebecca Morris
			}
			if (GetKillCount() == 2)
			{
				//new WinScreen(this->GetGame());//Ashley 
			}
		}
	}

}

void FPSActor::PlayMonsterDeath(float LastDeath) //Rebecca Morris
{
	if (LastDeath >= 2.0f) //Has a cooldown so we are not overwhelmed by monster sounds
	{
		mMonsterDeath.SetPaused(false);
		mMonsterDeath = mAudioComp->PlayEvent("event:/MonsterDeath");
	}
}

void FPSActor::PauseMonsterDeath() //Rebecca Morris
{
	mMonsterDeath.SetPaused(true);
	mMonsterDeath.Restart();
}

// Prasanna
void FPSActor::ClickShoeButton()
{
	shoeButtonClicked = true;
}

// Prasanna
void FPSActor::ClickGloveButton()
{
	gloveButtonClicked = true;
}


#pragma once
// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include "MoveComponent.h" //Rebecca Morris
#include "SoundEvent.h"

class FPSActor : public Actor
{
public:
	FPSActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keys) override;

	void Shoot();

	void SetFootstepSurface(float value);

	void SetVisible(bool visible);

	Vector3 GetPlayerDirection() { return mPlayerDirection; };
	void SetPlayerDirection(Vector3 currentDirection);

	void FixCollisions();

	void GetInjured();

	bool GetBasicEvolvedStatus() { return mBasicEvolution; }
	void SetBasicEvolvedStatus(bool EvoStatus) { mBasicEvolution = EvoStatus; }

	int GetKillCount() { return mKillCount; }
	void SetKillCount(int RecordKill) { mKillCount = RecordKill; }

	float GetMovementSpeed() { return mMovementSpeed; }
	void SetMovementSpeed(float newSpeed) { mMovementSpeed = newSpeed; }

	void PlayMonsterDeath(float LastDeath); //Rebecca Morris
	void PauseMonsterDeath(); //Rebecca Morris

	// Prasanna
	void ClickShoeButton();
	void ClickGloveButton();

	// Prasanna
	bool shoeButtonClicked;
	bool gloveButtonClicked;

private:
	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	class MeshComponent* mMeshComp;
	class FPSCamera* mCameraComp;
	class BoxComponent* mBoxComp;
	class Actor* mFPSModel;
	SoundEvent mFootstep;
	SoundEvent mMonsterDeath; //Rebecca Morris
	float mLastFootstep;
	float mLastMonsterDeath; //Rebecca Morris
	Vector3 mPlayerDirection;
	bool mBasicEvolution;
	int mKillCount;
	float mMovementSpeed;
};


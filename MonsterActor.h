// ----------------------------------------------------------------
//Creation of Monster Class that is from Madhav's Game Programming Code :: Worked on by Ashley and Prassana 
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include <string>
#include <string>
#include "SoundEvent.h" //Rebecca Morris
using namespace std;

class MonsterActor : public Actor
{
public:
	MonsterActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keys) override;
	void FixCollisions();
	void SetVisible(bool visible);
	void SetAngularSpeed(float newSpeed);
	bool GetAggroState() { return mIsAggro; };
	void SetAggroState(bool AggroState);
	void SetHealthState(); //Rebecca Morris
	string GetHealthState() { return mHealthState; }; //Rebecca Morris
	void MonsterShoot();
	void MonsterAttackMode();
	float GetMonsterSpeed() { return mMonsterMovementSpeed; }
	void SetMonsterSpeed(float newSpeed) { mMonsterMovementSpeed = newSpeed; }
private:
	class AudioComponent* mAudioComp;
	SoundEvent mMonsterHostile; //Rebecca Morris
	class MoveComponent* mMoveComp;
	class SkeletalMeshComponent* mMeshComp;
	class BoxComponent* mBoxComp;
	float angularMovement;
	bool mMoving;
	bool mIsAggro;
	float mAggroTimer;
	float mMonsterMovementSpeed;
	string mHealthState;
};


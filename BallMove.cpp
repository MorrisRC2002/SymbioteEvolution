// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "BallMove.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"
#include "TargetActor.h"
#include "BallActor.h"
#include "MonsterActor.h" //Rebecca Morris
#include "FPSActor.h"
#include "SimpleManActor.h"
#include "AudioSystem.h"
#include "AudioComponent.h"


BallMove::BallMove(Actor* owner)
	:MoveComponent(owner)
{
	mOwner->SetDamageAmt(25);//Ashley Odden
	// I changed the damage amount to 25 for some tests // Adam
	mKillCount = 0.0;
	mLastMonsterDeath = 2.0f; //Rebecca Morris
}

void BallMove::Update(float deltaTime)
{
	mLastMonsterDeath += deltaTime; //Rebecca Morris

	if (mLastMonsterDeath >= 1.0f) //Rebecca Morris
	{
		mOwner->GetGame()->GetPlayer()->PauseMonsterDeath();
	}

	// Construct segment in direction of travel
	const float segmentLength = 30.0f;
	Vector3 start = mOwner->GetPosition();
	Vector3 dir = mOwner->GetForward();
	Vector3 end = start + dir * segmentLength;

	// Create line segment
	LineSegment l(start, end);

	// Test segment vs world
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;

	// (Don't collide vs our player)
	if (phys->SegmentCast(l, info) && info.mActor != mPlayer)
	{
		// If we collided, reflect the ball about the normal
		dir = Vector3::Reflect(dir, info.mNormal);
		mOwner->RotateToNewForward(dir);
		FPSActor* currActor = mOwner->GetGame()->GetPlayer();

		// Did we hit a target?
		MonsterActor* monster = dynamic_cast<MonsterActor*>(info.mActor);
		WolfMonster* wolfMonster = dynamic_cast<WolfMonster*>(info.mActor);
		TigerMonster* tigerMonster = dynamic_cast<TigerMonster*>(info.mActor);
		Monster_Shark* sharkMonster = dynamic_cast<Monster_Shark*>(info.mActor);
		EagleMonster* eagleMonster = dynamic_cast<EagleMonster*>(info.mActor);
		MonkeyMonster* monkeyMonster = dynamic_cast<MonkeyMonster*>(info.mActor);

		if (monkeyMonster) // Added collision and the ability to damage each different monster
							// they are all very similar they just handle each different monster	
							// Adam C and John Akpinar
		{
			static_cast<BallActor*>(mOwner)->HitTarget();
			monkeyMonster->DamageActor(mOwner->GetDamageAmt());
			mOwner->SetState(Actor::EDead);
			if (monkeyMonster->GetHealth() <= 0)
			{
				currActor->PlayMonsterDeath(mLastMonsterDeath); //Rebecca Morris
				mLastMonsterDeath = 0.0f;

				monkeyMonster->SetState(Actor::EDead);
			}
		}

		if (eagleMonster)
		{
			static_cast<BallActor*>(mOwner)->HitTarget();
			eagleMonster->DamageActor(mOwner->GetDamageAmt());
			mOwner->SetState(Actor::EDead);
			if (eagleMonster->GetHealth() <= 0)
			{
				currActor->PlayMonsterDeath(mLastMonsterDeath); //Rebecca Morris
				mLastMonsterDeath = 0.0f;

				eagleMonster->SetState(Actor::EDead);
			}
		}

		if (sharkMonster)
		{
			static_cast<BallActor*>(mOwner)->HitTarget();
			sharkMonster->DamageActor(mOwner->GetDamageAmt());
			mOwner->SetState(Actor::EDead);
			if (sharkMonster->GetHealth() <= 0)
			{
				currActor->PlayMonsterDeath(mLastMonsterDeath); //Rebecca Morris
				mLastMonsterDeath = 0.0f;

				sharkMonster->SetState(Actor::EDead);
			}
		}

		if (tigerMonster)
		{
			static_cast<BallActor*>(mOwner)->HitTarget();
			tigerMonster->DamageActor(mOwner->GetDamageAmt());
			mOwner->SetState(Actor::EDead);
			if (tigerMonster->GetHealth() <= 0)
			{
				currActor->PlayMonsterDeath(mLastMonsterDeath); //Rebecca Morris
				mLastMonsterDeath = 0.0f;

				tigerMonster->SetState(Actor::EDead);
			}
		}

		if (wolfMonster)
		{
			static_cast<BallActor*>(mOwner)->HitTarget();
			wolfMonster->DamageActor(mOwner->GetDamageAmt());
			mOwner->SetState(Actor::EDead);
			if (wolfMonster->GetHealth() <= 0)
			{
				currActor->PlayMonsterDeath(mLastMonsterDeath); //Rebecca Morris
				mLastMonsterDeath = 0.0f;

				wolfMonster->SetState(Actor::EDead); 
			}
		}

		if (monster)
		{
			static_cast<BallActor*>(mOwner)->HitTarget();
			monster->DamageActor(mOwner->GetDamageAmt()); //Ashley

			monster->SetHealthState(); //Rebecca Morris

			if (monster->GetAggroState() == false)
			{
				monster->SetAggroState(true); // This makes the monster aggro state true // Adam C
			}
			mOwner->SetState(Actor::EDead); //Ashley
			// SetSate of both monster and ball actor to dead upon kill
			if(monster->GetHealth() <= 0)
			{
				currActor->PlayMonsterDeath(mLastMonsterDeath); //Rebecca Morris
				mLastMonsterDeath = 0.0f;

				monster->SetState(Actor::EDead);//Ashley
				mOwner->SetState(Actor::EDead); //Ashley
				//currActor->SetBasicEvolvedStatus(true);
				int currentKillCount = currActor->GetKillCount(); // updating the kill count // Adam C
				currentKillCount++;
				currActor->SetKillCount(currentKillCount);// Setting killcount to the updated one // Adam C
				float currentMovementSpeed = currActor->GetMovementSpeed();
				if (currentMovementSpeed < 1000.0) // We increase the movement speed each kill by 50, until we hit 1000 // Adam C
				{
					currentMovementSpeed = currentMovementSpeed + 50.0;
					currActor->SetMovementSpeed(currentMovementSpeed);
				}
				//int x = 2; x = sqrt(2); just a testing line for debugging // Adam C
			}
		}
	}

	// Base class update moves based on forward speed
	MoveComponent::Update(deltaTime);
}

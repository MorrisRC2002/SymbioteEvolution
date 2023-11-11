// Monster Class Modified from Madhav's FollowActor class
#include "SharkMonster.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "FollowCamera.h"
#include "MoveComponent.h"
#include "BallActor.h"
#include "BallMove.h"
#include "TargetComponent.h"
#include "BoxComponent.h"
#include "Mesh.h"
#include "PlaneActor.h"
#include "FPSActor.h"
#include "AudioComponent.h"
#include "Random.h"
#include "Scenery.h"

SharkMonster::SharkMonster(Game* game) // set this to scenery instead of actor to see if we can get them to move
	:Actor(game)
	, mMoving(false)
	, angularMovement(1.0)
	, mIsAggro(false)
	, mAggroTimer(1.0)
	, mMonsterMovementSpeed(300)
{

	//John Akpinar
	int max = 2;
	int min = -2;
	float angularSpeed = min + rand() % (max - min + 1);
	Vector2 randomLoc = Random::GetVector(Vector2::Zero, Vector2(-1000, 1000));
	Vector3 monsterPos = Vector3(randomLoc.x, randomLoc.y, -100);
	SetPosition(monsterPos);
	SetAngularSpeed(angularSpeed);
	//Adam Caligiuri, trying to get the new meshes and stuff into its own monster classes
	//new Scenery(this, "Assets/Shark.fbx", "Assets/Shark.png");


	mHealthState = "Green"; //Rebecca Morris

	mAudioComp = new AudioComponent(this);
	//SetPosition(Vector3(100.0f, 100.0f, -100.0f));

	mMonsterHostile = mAudioComp->PlayEvent("event:/Hostile");
	mMonsterHostile.SetPaused(true);

	mMoveComp = new MoveComponent(this);
	new TargetComponent(this); //Ashley

	// Add collision box
	mBoxComp = new BoxComponent(this);
	mBoxComp->SetObjectBox(mMeshComp->GetMeshComp()->GetBox()); //Ashley

	//John Akpinar
	//game->AddMonster(this);

}

void SharkMonster::UpdateActor(float deltaTime)
{
	FixCollisions();
	mAggroTimer -= deltaTime;
	/*if (mAggroTimer <= 0)
	{
		mIsAggro = false;
	}*/
}

void SharkMonster::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = mMonsterMovementSpeed;
	float angularSpeed = angularMovement;

	// Setup basic movement in a circle 
	// Did we just start moving?
	
	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetAngularSpeed(angularSpeed);

	if (mIsAggro == true) // This handles the enemies becoming aggresive to the player // Adam C
	{
		// run at the player, and attack in that direction
		FPSActor* currPlayer = GetGame()->GetPlayer();
		Vector3 playerDirection = currPlayer->GetPlayerDirection();
		Vector3 oppositePlayerDir; // we need to get the opposite players direction in order to get the enemy to run at the player
		{ oppositePlayerDir.x = -playerDirection.x; oppositePlayerDir.y = -playerDirection.y; oppositePlayerDir.z = 0; } // z must remain 0 or the monster runs up or down depending on how you shoot them Adam C
		oppositePlayerDir.Normalize();//normalization
		RotateToNewForward(oppositePlayerDir);
		angularMovement = 0; // this prevents the enemy from going back to turning
		MonsterAttackMode();
		//MonsterShoot(); // Trigger the monster to attack back, doesnt currently work
	}
}

void SharkMonster::MonsterAttackMode() // Function made by Adam C
{
	if (mIsAggro == true) // Once the enemy becomes aggro, we continuously follow the player
	{
		// run at the player, and attack in that direction

		SetMonsterSpeed(500);
		FPSActor* currPlayer = GetGame()->GetPlayer();
		Vector3 playerPosition = currPlayer->GetPosition();
		Vector3 monsterPosition = this->GetPosition();
		Vector3 monsterFacingPlayer;
		{ monsterFacingPlayer.x = playerPosition.x - monsterPosition.x; };
		{ monsterFacingPlayer.y = playerPosition.y - monsterPosition.y; };
		{ monsterFacingPlayer.z = 0; };
		monsterFacingPlayer.Normalize();
		// What we do above is get the two points, one being the player position, the other being the monsters position.
		// We then take those two points, and subtract the player from the monster, then that result is the vector we need 
		// to get the monster to face the player.  We continuously do this to get the monster to follow you.
		RotateToNewForward(monsterFacingPlayer);
		angularMovement = 0; // this prevents the enemy from going back to turning

		//MonsterShoot(); // Trigger the monster to attack back, doesnt currently work
	}
}



// Attempting to get the monster to shoot in the direction it is looking // Adam C, doesnt currently work
void SharkMonster::MonsterShoot()
{
	// Get start point (in center of screen on near plane)
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	Vector3 start = GetGame()->GetRenderer()->Unproject(this->GetPosition());
	// Get end point (in center of screen, between near and far)
	screenPoint.z = 0.9f;
	Vector3 end = GetGame()->GetRenderer()->Unproject(GetGame()->GetPlayer()->GetPosition());
	// Get direction vector
	Vector3 dir = end - start;
	//dir.Normalize();
	// Spawn a ball
	BallActor* ball = new BallActor(GetGame());
	ball->SetPlayer(this);
	ball->SetPosition(dir);
	// Rotate the ball to face new direction
	ball->RotateToNewForward(dir);
	// Play shooting sound
	mAudioComp->PlayEvent("event:/Shot");
}


void SharkMonster::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}

void SharkMonster::SetAngularSpeed(float newSpeed)
{
	angularMovement = newSpeed;
}

void SharkMonster::SetAggroState(bool AggroState)
{
	mIsAggro = AggroState;
	mMonsterHostile.SetPaused(false);
	mMonsterHostile.Restart();
}

void SharkMonster::SetHealthState() //Rebecca Morris
{
	int health = this->GetHealth();

	if (health < 100 && health > 50)
		mHealthState = "Yellow";
	else if (health <= 50)
		mHealthState = "Red";
	else
		mHealthState = "Green";

}

void SharkMonster::FixCollisions() // simply added in by Adam C from madhav code
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
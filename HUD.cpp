// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "HUD.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "PhysWorld.h"
#include "FollowActor.h"
#include "FPSActor.h"
#include <algorithm>
#include "TargetComponent.h"
#include "MonsterActor.h"


HUD::HUD(Game* game)
	:UIScreen(game)
	, mRadarRange(2000.0f)
	, mRadarRadius(92.0f)
	, mTargetEnemy(false)
{
	Renderer* r = mGame->GetRenderer();
	mHealthBar = r->GetTexture("Assets/HealthBar.png");
	mHealthBar75 = r->GetTexture("Assets/HealthBar75.png");
	mHealthBar50 = r->GetTexture("Assets/HealthBar50.png");
	mHealthBar25 = r->GetTexture("Assets/HealthBar25.png");
	mRadar = r->GetTexture("Assets/Radar.png");
	mCrosshair = r->GetTexture("Assets/Crosshair.png");
	mCrosshairEnemy = r->GetTexture("Assets/CrosshairRed.png");
	mBlipTex = r->GetTexture("Assets/Blip.png"); 
	mFullHealthBlipTex = r->GetTexture("Assets/GreenBlip.png"); //Rebecca Morris
	mInjuredBlipTex = r->GetTexture("Assets/YellowBlip.png");
	mLowHealthBlipTex = r->GetTexture("Assets/RedBlip.png");
	mRadarArrow = r->GetTexture("Assets/RadarArrow.png");
}

HUD::~HUD()
{
}

void HUD::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);

	UpdateCrosshair(deltaTime);
	UpdateRadar(deltaTime);
	UpdateHealthBar(deltaTime); 
}

void HUD::Draw(Shader* shader)
{
	// Crosshair
	//Texture* cross = mTargetEnemy ? mCrosshairEnemy : mCrosshair;
	//DrawTexture(shader, cross, Vector2::Zero, 2.0f);

	// Radar
	const Vector2 cRadarPos(-390.0f, 275.0f);
	DrawTexture(shader, mRadar, cRadarPos, 1.0f);
	// Blips
	/*for (Vector2& blip : mBlips)
	{
		DrawTexture(shader, mBlipTex, cRadarPos + blip, 1.0f);
	}*/
	
	for (Vector2& redBlip : mRedBlips) //Rebecca Morris
	{
		DrawTexture(shader, mLowHealthBlipTex, cRadarPos + redBlip, 1.0f);
	}
	for (Vector2& yellowBlip : mYellowBlips) //Rebecca Morris
	{
		DrawTexture(shader, mInjuredBlipTex, cRadarPos + yellowBlip, 1.0f);
	}
	for (Vector2& greenBlip : mGreenBlips) //Rebecca Morris
	{
		DrawTexture(shader, mFullHealthBlipTex, cRadarPos + greenBlip, 1.0f);
	}


	for (Vector2& fullHealth : mHealthBarFull)
	{
		DrawTexture(shader, mHealthBar, Vector2(-350.0f, -350.0f));
	}
	for (Vector2& health75 : mHealthBar75p)
	{
		DrawTexture(shader, mHealthBar75, Vector2(-350.0f, -350.0f));
	}
	for (Vector2& health50 : mHealthBar50p)
	{
		DrawTexture(shader, mHealthBar50, Vector2(-350.0f, -350.0f));
	}
	for (Vector2& health25 : mHealthBar25p)
	{
		DrawTexture(shader, mHealthBar25, Vector2(-350.0f, -350.0f));
	}
	// Radar arrow
	DrawTexture(shader, mRadarArrow, cRadarPos);

	// Health bar
	//DrawTexture(shader, mHealthBar, Vector2(-350.0f, -350.0f));

}

void HUD::AddTargetComponent(TargetComponent* tc)
{
	mTargetComps.emplace_back(tc);
}

void HUD::RemoveTargetComponent(TargetComponent* tc)
{
	auto iter = std::find(mTargetComps.begin(), mTargetComps.end(),
		tc);
	mTargetComps.erase(iter);
}

void HUD::UpdateCrosshair(float deltaTime)
{
	// Reset to regular cursor
	mTargetEnemy = false;
	// Make a line segment
	const float cAimDist = 5000.0f;
	Vector3 start, dir;
	mGame->GetRenderer()->GetScreenDirection(start, dir);
	LineSegment l(start, start + dir * cAimDist);
	// Segment cast
	PhysWorld::CollisionInfo info;
	if (mGame->GetPhysWorld()->SegmentCast(l, info))
	{
		// Is this a target?
		for (auto tc : mTargetComps)
		{
			if (tc->GetOwner() == info.mActor)
			{
				mTargetEnemy = true;
				break;
			}
		}
	}
}

void HUD::UpdateRadar(float deltaTime)
{
	// Clear blip positions from last frame
	//mBlips.clear();
	mRedBlips.clear();
	mYellowBlips.clear();
	mGreenBlips.clear();

	// Convert player position to radar coordinates (x forward, z up)
	Vector3 playerPos = mGame->GetPlayer()->GetPosition();
	Vector2 playerPos2D(playerPos.y, playerPos.x);
	// Ditto for player forward
	Vector3 playerForward = mGame->GetPlayer()->GetForward();
	Vector2 playerForward2D(playerForward.x, playerForward.y);

	// Use atan2 to get rotation of radar
	float angle = Math::Atan2(playerForward2D.y, playerForward2D.x);
	// Make a 2D rotation matrix
	Matrix3 rotMat = Matrix3::CreateRotation(angle);

	// Get positions of blips
	for (auto tc : mTargetComps)
	{
		Vector3 targetPos = tc->GetOwner()->GetPosition();
		Vector2 actorPos2D(targetPos.y, targetPos.x);
		int targetHealth = tc->GetOwner()->GetHealth(); //Rebecca Morris

		// Calculate vector between player and target
		Vector2 playerToTarget = actorPos2D - playerPos2D;

		// See if within range
		if (playerToTarget.LengthSq() <= (mRadarRange * mRadarRange))
		{
			// Convert playerToTarget into an offset from
			// the center of the on-screen radar
			Vector2 blipPos = playerToTarget;
			blipPos *= mRadarRadius / mRadarRange;

			// Rotate blipPos
			blipPos = Vector2::Transform(blipPos, rotMat);

			if (targetHealth == 100) //Rebecca Morris
				mGreenBlips.emplace_back(blipPos);
			else if (targetHealth < 100 && targetHealth >= 50)
				mYellowBlips.emplace_back(blipPos);
			else
				mRedBlips.emplace_back(blipPos);

			//mBlips.emplace_back(blipPos);
		}
	}
}

void HUD::UpdateHealthBar(float deltaTime) // Function created by Adam C, with help from Rebecca
{
	mHealthBarFull.clear(); // these need tobe here or we just get a ton of health bars
	mHealthBar75p.clear();
	mHealthBar50p.clear();
	mHealthBar25p.clear();

	FPSActor* player = mGame->GetPlayer();
	if (player->GetHealth() < 75 && player->GetHealth() > 50) // Set the health bar relative to the players health
	{
		mHealthBar75p.emplace_back(Vector2(-350.0f, -350.0f));
	}
	else if (player->GetHealth() <= 50 && player->GetHealth() > 25)
	{
		mHealthBar50p.emplace_back(Vector2(-350.0f, -350.0f));
	}
	else if (player->GetHealth() <= 25)
	{
		mHealthBar25p.emplace_back(Vector2(-350.0f, -350.0f));
	}
	else
	{
		//place full health bar
		mHealthBarFull.emplace_back(Vector2(-350.0f, -350.0f));
	}
}

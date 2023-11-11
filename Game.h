// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "SoundEvent.h"
#include <SDL/SDL_types.h>
#include "MonsterActor.h"
#include "EagleMonster.h"
#include "Monster_Shark.h"
#include "TigerMonster.h"
#include "WolfMonster.h"
#include "MonkeyMonster.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	class Renderer* GetRenderer() { return mRenderer; }
	class AudioSystem* GetAudioSystem() { return mAudioSystem; }
	class PhysWorld* GetPhysWorld() { return mPhysWorld; }
	class HUD* GetHUD() { return mHUD; }

	// Manage UI stack
	const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
	void PushUI(class UIScreen* screen);
	
	//class FollowActor* GetPlayer() { return mFollowActor; }
	class FPSActor* GetPlayer() { return mFPSActor; }

	enum GameState
	{
		EGameplay,
		EPaused,
		EQuit
	};

	GameState GetState() const { return mGameState; }
	void SetState(GameState state) { mGameState = state; }

	class Font* GetFont(const std::string& fileName);

	void LoadText(const std::string& fileName);
	const std::string& GetText(const std::string& key);

	class Skeleton* GetSkeleton(const std::string& fileName);

	class Animation* GetAnimation(const std::string& fileName);
	// Game-specific
	void AddPlane(class PlaneActor* plane);
	void RemovePlane(class PlaneActor* plane);
	void AddMonster(MonsterActor* monst);
	std::vector<class PlaneActor*>& GetPlanes() { return mPlanes; }
	std::vector<class MonsterActor*>& GetMonsters() { return mMonsters; } //Rebecca Morris
private:
	void ProcessInput();
	void HandleKeyPress(int key);
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// All the actors in the game
	std::vector<class Actor*> mActors;
	//All the monsters in the game
	std::vector<class MonsterActor*> mMonsters; //Rebecca Morris
	std::vector<class UIScreen*> mUIStack;
	// Map for fonts
	std::unordered_map<std::string, class Font*> mFonts;
	// Map of loaded skeletons
	std::unordered_map<std::string, class Skeleton*> mSkeletons;
	// Map of loaded animations
	std::unordered_map<std::string, class Animation*> mAnims;

	// Map for text localization
	std::unordered_map<std::string, std::string> mText;
	// Any pending actors
	std::vector<class Actor*> mPendingActors;

	class Renderer* mRenderer;
	class AudioSystem* mAudioSystem;
	class PhysWorld* mPhysWorld;
	class HUD* mHUD;

	Uint32 mTicksCount;
	GameState mGameState;
	// Track if we're updating actors right now
	bool mUpdatingActors;
	float spawnTimer;
	// Game-specific code
	class FPSActor* mFPSActor;
	class FollowActor* mFollowActor;
	class MonsterActor* mEnemyActor;
	class EagleMonster* mEagleActor;
	class Monster_Shark* mSharkActor;
	class TigerMonster* mTigerActor;
	class WolfMonster* mWolfActor;
	class MonkeyMonster* mMonkeyActor;
	std::vector<class PlaneActor*> mPlanes;
	class SpriteComponent* mCrosshair;
	SoundEvent mMusicEvent;
	class MeshComponent* mc;
	class SkyBox* mSkybox;
};

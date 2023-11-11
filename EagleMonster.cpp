// ----------------------------------------------------------------
// Written by Will Briggs
// 2023-3-7
// This is a 3d model of a man. He's supposed to walk, but this code doesn't support that yet.
// ----------------------------------------------------------------
//Added by Ashley Added collisions and radar
#include "EagleMonster.h"
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <SDL/SDL_log.h> //for SDL_Log, which reports errors
#include "Game.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Texture.h"
#include "VertexArray.h"
#include "MoveComponent.h"
#include "AssimpLoad.h"
#include "BoxComponent.h" //Ashley Collisions
#include "TargetComponent.h" //Ashley radar
#include "PlaneActor.h"

EagleMonster::EagleMonster(::Game* game)
	: Actor(game)
	, rotationSpeed_(1.0f)
	, rotationAngle_(0.0f)
{
	SetScale(0.55f);

	Texture* texture = new Texture; texture->Load("Assets/Eagle.png");
	std::vector<Mesh*> meshes; LoadAssimpMeshes(meshes, game, "Assets/Eagle.fbx", texture);
	setSpecular(meshes, 30000.0); // we don't want a shiny man :)
	Vector3 startPos = Vector3(1500.00, 500.00, 1000.00);
	SetPosition(startPos);
	
	// apparently high numbers -> less shiny
	for (auto&& mesh : meshes)
	{
		MeshComponent* mc = new MeshComponent(this); mc->SetMesh(mesh);
		meshComponents_.push_back(mc);
		mBoxComp = new BoxComponent(this); //Ashley
		mBoxComp->SetObjectBox(mc->GetMeshComp()->GetBox()); //Ashley
	}

	mMoveComp = new MoveComponent(this);

	//new TargetComponent(this); //Ashley Add Target Component for Radar
}

void EagleMonster::UpdateActor(float deltaTime)
{
	mMoveComp->SetAngularSpeed(rotationSpeed_);
	mMoveComp->SetForwardSpeed(500);
}

bool EagleMonster::Update(float dt)
{
	rotationAngle_ += rotationSpeed_ * dt;
	while (rotationAngle_ > 2.f * Math::Pi)
		rotationAngle_ -= 2.f * Math::Pi;

	SetRotation(::Quaternion(Vector3::UnitZ, rotationAngle_));

	return true;
}

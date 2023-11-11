// ----------------------------------------------------------------
// Written by Will Briggs
// 2023-3-7
// This is a 3d model of a man. He's supposed to walk, but this code doesn't support that yet.
// ----------------------------------------------------------------
//Added by Ashley Added collisions and radar
#include "WolfMonster.h"
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

WolfMonster::WolfMonster(::Game* game)
	: Actor(game)
	, rotationSpeed_(2.0f)
	, rotationAngle_(2.0f)
{
	SetScale(2.0f);

	Texture* texture = new Texture; texture->Load("Assets/Wolf.png");
	std::vector<Mesh*> meshes; LoadAssimpMeshes(meshes, game, "Assets/Wolf.fbx", texture);
	setSpecular(meshes, 30000.0); // we don't want a shiny man :)
	Vector3 startPos = Vector3(1500.00, 1500.00, -100.00);
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
	mMoveComp->SetAngularSpeed(rotationSpeed_);
	mMoveComp->SetForwardSpeed(200);

	//new TargetComponent(this); //Ashley Add Target Component for Radar
}

void WolfMonster::UpdateActor(float deltaTime)
{
	FixCollisions();
}

bool WolfMonster::Update(float dt)
{
	rotationAngle_ += rotationSpeed_ * dt;
	while (rotationAngle_ > 2.f * Math::Pi)
		rotationAngle_ -= 2.f * Math::Pi;

	SetRotation(::Quaternion(Vector3::UnitZ, rotationAngle_));

	return true;
}

void WolfMonster ::FixCollisions() // simply added in by Adam C from madhav code
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
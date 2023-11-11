#pragma once

#include <vector>
#include "Actor.h"

class Game;
class MoveComponent;
class MeshComponent;

class Monster_Shark : public Actor
{
public:
	Monster_Shark(::Game* game);
	Monster_Shark(const Monster_Shark&) = delete;
	~Monster_Shark() = default;
	void UpdateActor(float deltaTime) override;
	void FixCollisions();

	bool Update(float dt);
protected:
	float rotationSpeed_;
	float rotationAngle_;
	MoveComponent* mMoveComp;
	class BoxComponent* mBoxComp;
	std::vector<MeshComponent*> meshComponents_;
};


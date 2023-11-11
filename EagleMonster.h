#pragma once
//Added by Ashley Added collisions and radar
#include <vector>
#include "Actor.h"

class Game;
class MoveComponent;
class MeshComponent;

class EagleMonster : public Actor
{
public:
	EagleMonster(::Game* game);
	EagleMonster(const EagleMonster&) = delete;
	~EagleMonster() = default;
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

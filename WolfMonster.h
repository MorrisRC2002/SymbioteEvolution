#pragma once
//Added by Ashley Added collisions and radar
#include <vector>
#include "Actor.h"

class Game;
class MoveComponent;
class MeshComponent;

class WolfMonster : public Actor
{
public:
	WolfMonster(::Game* game);
	WolfMonster(const WolfMonster&) = delete;
	~WolfMonster() = default;
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
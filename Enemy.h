#pragma once
#include "BaseCharactor.h"

class Enemy : public BaseCharacter
{
private:
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	uint32_t kModelBody = 0;
	uint32_t kModelL_arm = 1;
	uint32_t kModelR_arm = 2;

public:
	Enemy();
	~Enemy();
	void Initialize(const std::vector<Model*>& models) override;
	void Update() override;
	void Draw(const ViewProjection& viewProjection) override;
};


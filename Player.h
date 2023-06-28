#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player {
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

public:
	Player();
	~Player();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);
};
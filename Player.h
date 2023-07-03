#pragma once
#include "Audio.h"
#include "AxisIndicator.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player {
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	//カメラのビュー往路じぇくション
	const ViewProjection* viewProjection_ = nullptr;

public:
	Player();
	~Player();
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection& viewProjection);
	void DrawA(ViewProjection& viewProjection);

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	void SetViewPRojection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }
};
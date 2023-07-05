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
	std::unique_ptr<Model>  model_ = nullptr;
	std::unique_ptr<Model> modelFace_;
	std::unique_ptr<Model> modelBody_;
	std::unique_ptr<Model> modelL_arm_;
	std::unique_ptr<Model> modelR_arm_;

	uint32_t textureHandle_ = 0u;

	WorldTransform worldTransform_;

	WorldTransform worldTransformFace_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

	uint16_t frame_ = 0;

public:
	Player();
	~Player();
	void Initialize(Model* modelFace, Model* modelBody, Model* modelL_arm, Model* modelR_arm, uint32_t textureHandle);
	void Update();
	void Draw(ViewProjection& viewProjection);
	void DrawA(ViewProjection& viewProjection);

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	void SetViewPRojection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	//浮遊ギミック初期化
	void InitializeFloatingGimmick();

	//浮遊ギミック更新
	void UpdateFloatingGimmick();
};
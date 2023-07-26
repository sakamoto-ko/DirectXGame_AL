#pragma once
#include "BaseCharactor.h"

class Player : public BaseCharacter{
private:
	WorldTransform worldTransformFace_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

	uint16_t frame_ = 0;
	uint16_t period = 0;
	float amplitude = 0;

	uint32_t kModelFace = 0;
	uint32_t kModelBody = 1;
	uint32_t kModelL_arm = 2;
	uint32_t kModelR_arm = 3;

public:
	Player();
	~Player();
	void Initialize(const std::vector<Model*>& models) override;
	void Update() override;
	void Draw(const ViewProjection& viewProjection) override;

	const WorldTransform& GetWorldTransform() { return worldTransformBody_; }

	void SetViewPRojection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	//浮遊ギミック初期化
	void InitializeFloatingGimmick();

	//浮遊ギミック更新
	void UpdateFloatingGimmick();
};
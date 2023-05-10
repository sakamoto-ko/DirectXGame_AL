#include "PlayerBullet.h"

PlayerBullet::PlayerBullet() {}
PlayerBullet::~PlayerBullet() {}
// 初期化
void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
}
// 更新
void PlayerBullet::Update() {
	//WorldTransformの更新
	worldTransform_.UpdateMatrix();
}
// 描画
void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

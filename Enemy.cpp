#include "Enemy.h"

Enemy::Enemy() {}
Enemy::~Enemy() {}

void Enemy::Initialize(const std::vector<Model*>& models) {
	//既定クラスの初期化
	BaseCharacter::Initialize(models);

	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	worldTransformBody_.translation_.x = 1.0f;
	worldTransformBody_.translation_.z = 2.0f;
	//worldTransformBody_.rotation_.y = -0.3f;
	worldTransformL_arm_.translation_.x = 0.5f;
	worldTransformL_arm_.translation_.z = 2.0f;
	//worldTransformL_arm_.rotation_.y = -0.3f;
	//worldTransformL_arm_.rotation_.z = 0.5f;
	worldTransformR_arm_.translation_.x = 1.5f;
	worldTransformR_arm_.translation_.z = 2.0f;
	//worldTransformR_arm_.rotation_.y = -0.3f;
	//worldTransformR_arm_.rotation_.z = 0.5f;
}
void Enemy::Update() {
	//既定クラスの更新
	BaseCharacter::Update();

	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}
void Enemy::Draw(const ViewProjection& viewProjection) {
	//既定クラスの描画
	BaseCharacter::Draw(viewProjection);

	models_[kModelBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelR_arm]->Draw(worldTransformR_arm_, viewProjection);
}
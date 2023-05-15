#include "Enemy.h"

Enemy::Enemy() {

}
Enemy::~Enemy() {

}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_.z = 100.0f;
}

void Enemy::ApproachUpdate() {
	//移動(ベクトルを加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, { 0.0f,0.0f,-0.3f });
	//基底の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate() {
	//移動(ベクトルを加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, { -0.3f,0.3f,-0.3f });
}

void Enemy::Update() {
	//WorldTransformの更新
	worldTransform_.UpdateMatrix();

	switch (phase_) {
	case Phase::Approach:
	default:
		ApproachUpdate();
		break;

	case Phase::Leave:
		LeaveUpdate();
		break;
	}
	//キャラクターの座標を画面表示する処理
	/*ImGui::Begin("");
	ImGui::Text("Enemy%f, %f", worldTransform_.translation_.x, worldTransform_.translation_.y);
	ImGui::End();*/
}

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
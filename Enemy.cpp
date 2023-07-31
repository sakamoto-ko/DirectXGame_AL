#include "Enemy.h"
#include "MyMath.h"
#include <cassert>
#include <iostream>
#include "ImGuiManager.h"

//移動初期化
void Enemy::InitializeMoveGimmick() {
	//中心座標
	center = {0.0f,1.5f,12.0f};
	//角度
	angle = { 0.5f,0.5f,0.5f };
	//半径の長さ
	length = 8.0f;
}

//移動更新
void Enemy::UpdateMoveGimmick() {
	// 中心座標に角度と長さを使用した円の位置を加算する
	// 度数法の角度を弧度法に変換
	float radius = angle.x * 3.14f / 180.0f;
	// 三角関数を使用し、円の位置を割り出す。
	float add_x = cos(radius) * length;
	float add_z = sin(radius) * length;

	// 結果ででた位置を中心位置に加算し、それを描画位置とする
	worldTransformBody_.translation_.x = center.x + add_x;
	worldTransformBody_.translation_.z = center.z + add_z;

	worldTransformL_arm_.translation_.x = center.x + add_x - 2.5f;
	worldTransformL_arm_.translation_.z = center.z + add_z;

	worldTransformR_arm_.translation_.x = center.x + add_x + 2.5f;
	worldTransformR_arm_.translation_.z = center.z + add_z;

	// 角度更新
	angle.x += 3.0f;

	//移動方向に向きを合わせる
	//Y軸周り角度(θy)
	worldTransformBody_.rotation_.y = std::atan2(angle.x, angle.z);
	worldTransformL_arm_.rotation_.y = std::atan2(angle.x, angle.z);
	worldTransformR_arm_.rotation_.y = std::atan2(angle.x, angle.z);
	//Y軸周りに-θy回す回転行列を計算

	//velocity_に回転行列を掛け算してvelocityZを求める

	//X軸周り角度(θy)
	worldTransformBody_.rotation_.x = std::atan2(angle.y, angle.z);
	worldTransformL_arm_.rotation_.x = std::atan2(angle.y, angle.z);
	worldTransformR_arm_.rotation_.x = std::atan2(angle.y, angle.z);
}

Enemy::Enemy() {}
Enemy::~Enemy() {}

void Enemy::Initialize(const std::vector<Model*>& models) {
	//既定クラスの初期化
	BaseCharacter::Initialize(models);

	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	worldTransformBody_.translation_.y = 1.5f;
	//worldTransformBody_.translation_.z = 8.0f;
	//worldTransformBody_.rotation_.y = -1.5f;

	worldTransformL_arm_.translation_.y = 1.5f;
	//worldTransformL_arm_.translation_.z = 8.0f;
	//worldTransformL_arm_.rotation_.x = 1.5f;
	//worldTransformL_arm_.rotation_.y = -0.0f;

	worldTransformR_arm_.translation_.y = 1.5f;
	//worldTransformR_arm_.translation_.z = 8.0f;
	//worldTransformR_arm_.rotation_.x = 1.5f;
	//worldTransformR_arm_.rotation_.y = -0.0f;

	InitializeMoveGimmick();
}
void Enemy::Update() {
	//既定クラスの更新
	BaseCharacter::Update();

	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	UpdateMoveGimmick();
}
void Enemy::Draw(const ViewProjection& viewProjection) {
	//既定クラスの描画
	//BaseCharacter::Draw(viewProjection);

	models_[kModelBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelR_arm]->Draw(worldTransformR_arm_, viewProjection);
}
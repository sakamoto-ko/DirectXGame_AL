﻿#include "EnemyBullet.h"

EnemyBullet::EnemyBullet() {}
EnemyBullet::~EnemyBullet() {}

// 初期化
void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3 velocity) {
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("red.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

// 更新
void EnemyBullet::Update() {
	//WorldTransformの更新
	worldTransform_.UpdateMatrix();

	//座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	//時間経過でデス
	if (--deathTimer_ < ~0) {
		isDead_ = true;
	}
}

// 描画
void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
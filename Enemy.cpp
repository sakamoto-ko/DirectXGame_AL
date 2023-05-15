#include "Enemy.h"
#include "WorldTransform.h"
#include <list>

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

void Enemy::Fire() {
	//弾の速度
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//速度ベクトルを自機の向きに合わせて回転させり￥る
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	//弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//弾を登録する
	enemyBullet_.push_back(newBullet);
}

Enemy::Enemy() {

}
Enemy::~Enemy() {

}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_.x = 30.0f;
	worldTransform_.translation_.z = 100.0f;
	Fire();
}

void Enemy::Update() {
	//デスフラグの立った弾を削除
	enemyBullet_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		}
	);

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

	//弾更新
	for (EnemyBullet* bullet : enemyBullet_) {
		bullet->Update();
	}

	//キャラクターの座標を画面表示する処理
	/*ImGui::Begin("");
	ImGui::Text("Enemy%f, %f", worldTransform_.translation_.x, worldTransform_.translation_.y);
	ImGui::End();*/
}

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//弾描画
	for (EnemyBullet* bullet : enemyBullet_) {
		bullet->Draw(viewProjection);
	}
}
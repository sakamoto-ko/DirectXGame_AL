#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"

#include "MyMath.h"

#include <cassert>
#include <list>

void Enemy::ApproachInit() {
	//発射タイマーを初期化
	shotTimer = kFireInterval;
}

void Enemy::ApproachUpdate() {
	//発射タイマーをデクリメント
	shotTimer--;
	//指定時間に達した
	if (shotTimer <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		shotTimer = kFireInterval;
	}
	//移動(ベクトルを加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, { 0.0f,0.0f,move });
	//基底の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate() {
	//移動(ベクトルを加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, { move,-move,move });
}

void Enemy::Fire() {
	assert(player_);
	assert(gameScene_);

	//弾の速度
	const float kBulletSpeed = 1.0f;

	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 worldPos = Subtract(playerPos, worldTransform_.translation_);
	Vector3 velocity = Normalize(worldPos);
	velocity.z *= kBulletSpeed;

	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	//弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	//弾をゲームシーンに登録する
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::OnCollision() {
	//デスフラグ
	isDead_ = true;
}

Enemy::Enemy() {

}
Enemy::~Enemy() {

}

void Enemy::Initialize(Model* model, uint32_t textureHandle, Vector3 pos) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	//Fire();

	move = -0.3f;

	//接近フェーズ初期化
	ApproachInit();
}

void Enemy::Update() {
	//デスフラグの立った弾を削除
	/*bullets_.remove_if([](EnemyBullet* bullet)
		{
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		}
	);*/

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

	if (input_->TriggerKey(DIK_M)) {
		if (move < 0.0f) {
			move = 0.0f;
		}
		else {
			move = -0.3f;
		}
	}

	//弾更新
	/*for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}*/

	//キャラクターの座標を画面表示する処理
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Enemy.translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("Enemy.rotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//弾描画
	/*for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}*/
}
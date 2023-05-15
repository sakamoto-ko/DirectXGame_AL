#pragma once
#include "EnemyBullet.h"

class Enemy {
private:
	//行動フェーズ
	enum class Phase {
		Approach,//接近
		Leave,//離脱
	};

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;

	//フェーズ
	Phase phase_ = Phase::Approach;

	//弾
	std::list<EnemyBullet*> enemyBullet_;

public:
	Enemy();
	~Enemy();
	//初期化
	void Initialize(Model* model, uint32_t textureHandle);
	//更新
	void Update();
	//描画
	void Draw(ViewProjection viewProjection);

	void ApproachUpdate();
	void LeaveUpdate();

	//弾発射
	void Fire();
};
#pragma once
#include "PlayerBullet.h"
#include "WorldTransform.h"
#include <list>

class Player {
private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;

	//弾
	std::list<PlayerBullet*> bullets_;
	
public:
	Player();
	~Player();
	//初期化
	void Initialize(Model* model,uint32_t textureHandle);
	//更新
	void Update();
	//描画
	void Draw(ViewProjection viewProjection);

	//旋回
	void Rotate();

	//攻撃
	void Attack();
};

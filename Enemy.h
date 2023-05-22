#pragma once
#include "EnemyBullet.h"
#include "Player.h"

//自機クラスの前方宣言
class Player;

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
	std::list<EnemyBullet*> bullets_;

	//自キャラ
	Player* player_ = nullptr;

	//発射タイマー
	int32_t shotTimer = 0;

public:
	Enemy();
	~Enemy();
	//初期化
	void Initialize(Model* model, uint32_t textureHandle);
	//更新
	void Update();
	//描画
	void Draw(ViewProjection viewProjection);

	//接近フェーズ初期化
	void ApproachInit();
	//接近フェーズ更新
	void ApproachUpdate();

	//離脱フェーズ初期化
	//void LeaveInit();
	//離脱フェーズ更新
	void LeaveUpdate();

	//弾発射
	void Fire();

	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//発射間隔
	static const int kFireInterval = 60;

	//衝突を検出したら呼び出されるコールバック関数	
	void OnCollision();

	//弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

};
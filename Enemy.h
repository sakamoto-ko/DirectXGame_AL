#pragma once
#include "EnemyBullet.h"

//自機クラスの前方宣言
class Player;

//GameSceneクラスの前方宣言
class GameScene;

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

	//自キャラ
	Player* player_ = nullptr;

	//弾
	//std::list<EnemyBullet*> bullets_;

	//弾リストを取得
	//const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

	//発射タイマー
	int32_t shotTimer = 0;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	//デスフラグ
	bool isDead_ = false;

	float move;

	//メンバ関数ポインタ
	void (Enemy::* pFunc)();

	//メンバ関数ポインタのテーブル
	static void (Enemy::* spFuncTable[])();

public:
	Enemy();
	~Enemy();
	//初期化
	void Initialize(Model* model, uint32_t textureHandle, Vector3 pos);
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

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//ワールド座標を取得
	const Vector3& GetWorldPosition() { return worldTransform_.translation_; }

	//発射間隔
	static const int kFireInterval = 60;

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リストを取得
	//const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

	//
	bool IsDead() { return isDead_; }
};
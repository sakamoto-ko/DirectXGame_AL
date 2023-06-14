#pragma once
#include "PlayerBullet.h"

//GameSceneクラスの前方宣言
class GameScene;

class Player {
private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	uint32_t textureHandleReticle_ = 0u;

	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	//キーボード入力
	Input* input_ = nullptr;

	//弾
	//std::list<PlayerBullet*> bullets_;
	
	int count_ = 0;

	//デスフラグ
	bool isDead_ = false;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

public:
	Player();
	~Player();
	//初期化
	void Initialize(Model* model,uint32_t textureHandle, uint32_t textureHandleReticle);
	//更新
	void Update(const ViewProjection viewProjection);
	//描画
	void Draw(ViewProjection viewProjection);

	//旋回
	void Rotate();

	//攻撃
	void Attack();

	void Shot();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	//衝突を検出したら呼び出されるコールバック関数	
	void OnCollision();

	//弾リストを取得
	//const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	//親となるトランスフォームをセット
	void SetParent(const WorldTransform* parent);

	//UI描画
	void DrawUI();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	void Get3DReticleWorldPosition();

	//3Dレティクルもワールド座標から2Dレティクルのスクリーン座標を計算
	//void Convert3Dto2DCoord(const ViewProjection viewProjection);

	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	void GetMouseWorldPosition(const ViewProjection viewProjection);

	//
	bool IsDead() { return isDead_; }
};

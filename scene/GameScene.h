#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//衝突判定と応答
	void CheckAllCollisions();

	//自弾を追加する
	void AddPlayerBullet(PlayerBullet* playerBullet);

	//敵弾を追加する
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	//敵発生データの読み込み
	void LoadEnemyPopData();

	//敵発生コマンドの更新
	void UpdateEnemyPopCommands();

	//敵発生関数
	void EnemyPop(Vector3 pos);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//3Dモデル
	Model* model_ = nullptr;

	Model* modelSkydome_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t textureHandleReticle_ = 0;

	//自キャラ
	Player* player_ = nullptr;

	//自弾
	std::list<PlayerBullet*> playerBullets_;

	//弾リストを取得
	const std::list<PlayerBullet*>& GetPlayerBullets() { return playerBullets_; }

	//敵キャラ
	std::list<Enemy*> enemies_;

	//敵リストを取得
	const std::list<Enemy*>& GetEnemies() { return enemies_; }

	//敵弾
	std::list<EnemyBullet*> enemyBullets_;

	//弾リストを取得
	const std::list<EnemyBullet*>& GetEnemyBullets() { return enemyBullets_; }

	//スカイドーム
	Skydome* skydome_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	//待機中フラグ
	bool isWait = false;
	
	//待機タイマー
	uint32_t waitTimer = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};

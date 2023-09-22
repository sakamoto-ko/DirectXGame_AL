#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
//#include "RailCamera.h"
#include <sstream>
#include "Ground.h"
#include "FollowCamera.h"
#include <memory>

#include "TransitionEffect.h"

#define ENEMY_MAX 25

typedef enum {
	TITLE,
	GAME,
	SCORE,
}Scene;

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
	void TitleInitialize();
	void GameInitialize();
	void ScoreInitialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	void TitleUpdate();
	void GameUpdate();
	void ScoreUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	void TitleDraw();
	void GameDraw();
	void GameDrawUI();
	void ScoreDraw();

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
	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelBullet_ = nullptr;

	Model* modelSkydome_ = nullptr;
	std::unique_ptr <Model> modelGround_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//テクスチャハンドル
	uint32_t textureHandlePlayer_ = 0;
	uint32_t textureHandleEnemy_ = 0;
	uint32_t textureHandleReticle_ = 0;
	uint32_t textureHandleBullet_ = 0;

	//自キャラ
	Player* player_ = nullptr;

	//自弾
	std::list<PlayerBullet*> playerBullets_;

	int32_t playerBulletMax_ = 1;
	int32_t playerBulletRest_ = 1;
	int32_t playerBulletTimeMax_ = 20;
	int32_t playerBulletTime_ = 20;

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
	//RailCamera* railCamera_ = nullptr;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	//待機中フラグ
	bool isWait = false;

	//待機タイマー
	uint32_t waitTimer = 0;

	std::unique_ptr<Ground> ground_;

	std::unique_ptr<FollowCamera> followCamera_;

	bool isTitle_ = true;
	bool isGame_ = false;
	bool isGameOver_ = false;
	bool isClear_ = false;

	// 画面遷移アニメーション
	TransitionEffect* transition_ = nullptr;

	int scene = TITLE;
	int nowScene = TITLE;

	int32_t killEnemyCount_ = 0;
	int32_t allEnemy_ = 0;
	bool isAllKillEnemy_ = false;

	// スプライト
	std::unique_ptr<Sprite> titleSprite_[3];
	std::unique_ptr<Sprite> scoreSprite_[7];
	std::unique_ptr<Sprite> aSprite_[2];
	std::unique_ptr<Sprite> timerSprite_;
	std::unique_ptr<Sprite> killEnemySprite_;
	std::unique_ptr<Sprite> optionSprite_;
	std::unique_ptr<Sprite> playerHpSprite_[10];

	// 画像
	uint32_t titleTexture_[3];
	uint32_t scoreTexture_[7];
	uint32_t aTexture_[2];
	uint32_t timerTexture_;
	uint32_t killEnemyTexture_;
	uint32_t optionTexture_;
	uint32_t playerHpTexture_;

	bool isScoreDraw_ = false;
	bool isPressButton_ = false;

	bool isOption_ = false;

	int count = 0; 
	int timeCount = 3600;

	Vector2 titlePos[2];
	Vector2 scorePos[5];

	Vector2 titleVeclocity[2];
	Vector2 scoreVelocity[5];

	bool isCanPressButton_ = false;
};

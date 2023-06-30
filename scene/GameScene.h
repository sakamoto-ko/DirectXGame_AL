#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "SafeDelete.h"
#include "DebugCamera.h"
#include <memory>

#include "Player.h"
#include "Skydome.h"
#include "Ground.h"
#include "FollowCamera.h"

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//スプライト
	Sprite* sprite_ = nullptr;

	//3Dモデル
	std::unique_ptr <Model> model_ = nullptr;

	Model* modelSkydome_ = nullptr;
	Model* modelGround_ = nullptr;
	Model* modelPlayer_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	std::unique_ptr<Player> player_;

	std::unique_ptr<Skydome> skydome_;

	std::unique_ptr<Ground> ground_;

	std::unique_ptr<FollowCamera> followCamera_;
};

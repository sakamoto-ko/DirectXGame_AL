#include "GameScene.h"

void GameScene::CheckAllCollisions() {
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		//球と球の交差判定
		if (posA.z + playerRadius >= posB.z && posA.z <= posB.z + enemyBulletRadius) {
			if (posA.y + playerRadius >= posB.y && posA.y <= posB.y + enemyBulletRadius) {
				if (posA.x + playerRadius >= posB.x && posA.x <= posB.x + enemyBulletRadius) {
					//自キャラの衝突時コールバックを呼び出す
					player_->OnCollision();
					//敵弾の衝突時コールバックを呼び出す
					bullet->OnCollision();
				}
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	//自弾の座標
	for (PlayerBullet* bullet : playerBullets) {
		posA = bullet->GetWorldPosition();

		//自弾と敵キャラ全ての当たり判定
		//敵キャラの座標
		posB = enemy_->GetWorldPosition();

		//座標AとBの距離を求める
		//球と球の交差判定
		if (posA.z + playerRadius >= posB.z && posA.z <= posB.z + enemyBulletRadius) {
			if (posA.y + playerRadius >= posB.y && posA.y <= posB.y + enemyBulletRadius) {
				if (posA.x + playerRadius >= posB.x && posA.x <= posB.x + enemyBulletRadius) {
					//自弾の衝突時コールバックを呼び出す
					bullet->OnCollision();
					//敵キャラの衝突時コールバックを呼び出す
					enemy_->OnCollision();
				}
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	//自弾の座標
	for (PlayerBullet* playerBullet : playerBullets) {
		posA = playerBullet->GetWorldPosition();

		//自弾と敵弾全ての当たり判定
		//敵弾の座標
		for (EnemyBullet* enemyBullet : enemyBullets) {
			posB = enemyBullet->GetWorldPosition();

			//座標AとBの距離を求める
			//球と球の交差判定
			if (posA.z + playerRadius >= posB.z && posA.z <= posB.z + enemyBulletRadius) {
				if (posA.y + playerRadius >= posB.y && posA.y <= posB.y + enemyBulletRadius) {
					if (posA.x + playerRadius >= posB.x && posA.x <= posB.x + enemyBulletRadius) {
						//自弾の衝突時コールバックを呼び出す
						playerBullet->OnCollision();
						//敵弾の衝突時コールバックを呼び出す
						enemyBullet->OnCollision();
					}
				}
			}
		}
	}
#pragma endregion
}

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete model_;
	delete player_;
	delete enemy_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//スカイドームの初期化
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 10000.0f;
	viewProjection_.Initialize();

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//敵キャラの生成
	enemy_ = new Enemy();
	//敵キャラの初期化
	enemy_->Initialize(model_, textureHandle_);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowHeight, WinApp::kWindowWidth);

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向が参照するビュープロジェクションを指定する(アドレスなし)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN)) {
		isDebugCameraActive_ = true;
	}
#endif

	//スカイドームの更新
	skydome_->Update();

	//自キャラの更新
	player_->Update();

	//敵キャラの更新
	enemy_->Update();

	//カメラの処理
	if (isDebugCameraActive_) {
		//デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクソン行列の転送
		viewProjection_.TransferMatrix();
	}
	else {
		//ビュープロジェク諸ン行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

	CheckAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	//スカイドームの描画
	skydome_->Draw(viewProjection_);

	//自キャラの描画
	player_->Draw(viewProjection_);

	//敵キャラの描画
	enemy_->Draw(viewProjection_);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

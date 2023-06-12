#include "GameScene.h"
#include <fstream>

#include "MyMath.h"

#include <cassert>
#include <list>

void GameScene::CheckAllCollisions() {
	//判定対象AとBの座標
	Vector3 posA, posB;

	//敵リストの取得
	//const std::list<Enemy*>& enemies = gameScene_->GetEnemy();

	//自弾リストの取得
	//const std::list<PlayerBullet*>& playerBullets = player_->GetEnemyBullets();
	//敵弾リストの取得
	//const std::list<EnemyBullet*>& enemyBullets = gameScene_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets_) {
		//敵弾の座標
		posB = bullet->GetWorldTransform();

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
	for (PlayerBullet* bullet : playerBullets_) {
		posA = bullet->GetWorldPosition();

		//自弾と敵キャラ全ての当たり判定
		//敵キャラの座標
		for (Enemy* enemy : enemies_) {
			posB = enemy->GetWorldPosition();

			//座標AとBの距離を求める
			//球と球の交差判定
			if (posA.z + playerRadius >= posB.z && posA.z <= posB.z + enemyBulletRadius) {
				if (posA.y + playerRadius >= posB.y && posA.y <= posB.y + enemyBulletRadius) {
					if (posA.x + playerRadius >= posB.x && posA.x <= posB.x + enemyBulletRadius) {
						//自弾の衝突時コールバックを呼び出す
						bullet->OnCollision();
						//敵キャラの衝突時コールバックを呼び出す
						enemy->OnCollision();
					}
				}
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	//自弾の座標
	for (PlayerBullet* playerBullet : playerBullets_) {
		posA = playerBullet->GetWorldPosition();

		//自弾と敵弾全ての当たり判定
		//敵弾の座標
		for (EnemyBullet* enemyBullet : enemyBullets_) {
			posB = enemyBullet->GetWorldTransform();

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

//敵発生関数
void GameScene::EnemyPop(Vector3 pos) {
	//敵を生成し、初期化
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(model_, textureHandle_, pos);
	//敵を登録する
	enemies_.push_back(newEnemy);
	for (Enemy* enemy : enemies_) {
		//敵キャラにゲームシーンのアドレスを渡す
		enemy->SetGameScene(this);
		//敵キャラに自キャラのアドレスを渡す
		enemy->SetPlayer(player_);
		enemy->Update();
	}
}

//敵弾を追加する
void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) {
	//リストに登録する
	playerBullets_.push_back(playerBullet);
}

//敵弾を追加する
void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	//リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

//敵発生データの読み込み
void GameScene::LoadEnemyPopData() {
	//ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

//敵発生コマンドの更新
void GameScene::UpdateEnemyPopCommands() {
	//待機処理
	if (isWait) {
		waitTimer--;
		if (waitTimer <= 0) {
			//待機完了
			isWait = false;
		}
		return;
	}

	//1行文の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		//1行文分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');
		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		//POPコマンド
		if (word.find("POP") == 0) {
			//X座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//Y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//Z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生させる
			EnemyPop(Vector3(x, y, z));
		}
		//WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			isWait = true;
			waitTimer = waitTime;

			//コマンドループを抜ける
			break;//重要
		}
	}
}

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete railCamera_;
	delete model_;
	delete modelSkydome_;
	delete player_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	//bullet_の開放
	for (PlayerBullet* bullet : playerBullets_) {
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	textureHandleReticle_ = TextureManager::Load("reticle.png");

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
	//敵キャラにゲームシーンのアドレスを渡す
	player_->SetGameScene(this);
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_, textureHandleReticle_);

	//敵発生
	LoadEnemyPopData();
	UpdateEnemyPopCommands();

	//レールカメラの生成
	railCamera_ = new RailCamera();
	railCamera_->Initialize(worldTransform_, { 0.00f,0.00f,0.00f });

	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

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
		if (isDebugCameraActive_ != 1) {
			isDebugCameraActive_ = true;
		}
		else {
			isDebugCameraActive_ = false;
		}
	}
#endif

	//スカイドームの更新
	skydome_->Update();

	//カメラの処理
	if (isDebugCameraActive_) {
		//デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
	else {
		//ビュープロジェクション行列の更新と転送
		//レールカメラの更新
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();

	}

	//自キャラの更新
	player_->Update(viewProjection_);

	//弾更新
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Update();
	}
	//デスフラグの立った弾を削除
	playerBullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		}
	);

	//敵発生
	UpdateEnemyPopCommands();

	//敵キャラの更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	//デスフラグの立った敵を削除
	enemies_.remove_if([](Enemy* enemy)
		{
			if (enemy->IsDead()) {
				delete enemy;
				return true;
			}
			else if (enemy->GetWorldPosition().z <= -80) {
				delete enemy;
				return true;
			}
			return false;
		}
	);

	//敵弾の更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}
	//デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet)
		{
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		}
	);

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

	//弾描画
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Draw(viewProjection_);
	}

	//敵キャラの描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}

	//敵弾描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

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

	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

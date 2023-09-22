#include "GameScene.h"
#include <fstream>

#include "MyMath.h"

#include <cassert>
#include <list>

void GameScene::CheckAllCollisions() {
	//判定対象AとBの座標
	Vector3 posA, posB;
#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		//敵弾の座標
		posB = enemyBullet->GetWorldTransform();

		//座標AとBの距離を求める
		//球と球の交差判定
		if (posA.z + playerRadius >= posB.z && posA.z <= posB.z + enemyBulletRadius) {
			if (posA.y + playerRadius >= posB.y && posA.y <= posB.y + enemyBulletRadius) {
				if (posA.x + playerRadius >= posB.x && posA.x <= posB.x + enemyBulletRadius) {
					//自キャラの衝突時コールバックを呼び出す
					player_->OnCollision();
					//敵弾の衝突時コールバックを呼び出す
					enemyBullet->OnCollision();
				}
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	//自弾の座標
	for (PlayerBullet* playerBullet : playerBullets_) {
		posA = playerBullet->GetWorldPosition();

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
						playerBullet->OnCollision();
						//敵キャラの衝突時コールバックを呼び出す
						enemy->OnCollision();
						if (enemy->IsDead()) {
							killEnemyCount_++;
						}
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
	//敵キャラにゲームシーンのアドレスを渡す
	newEnemy->SetGameScene(this);
	//敵キャラに自キャラのアドレスを渡す
	newEnemy->SetPlayer(player_);
	newEnemy->Initialize(modelEnemy_, modelBullet_, textureHandleEnemy_, pos);
	//敵を登録する
	enemies_.push_back(newEnemy);
	for (Enemy* enemy : enemies_) {
		//カメラのビュープロジェクションを敵キャラにコピー
		newEnemy->SetViewPRojection(&followCamera_->GetViewProjection());

		enemy->Update();
	}
	allEnemy_++;
}

//敵弾を追加する
void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) {
	//リストに登録する
	if (playerBulletRest_ > 0) {
		playerBullets_.push_back(playerBullet);
		playerBulletRest_--;
	}
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
	//delete railCamera_;
	delete modelPlayer_;
	delete modelEnemy_;
	delete modelSkydome_;
	delete player_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}
	//bullet_の開放
	for (PlayerBullet* playerBullet : playerBullets_) {
		delete playerBullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandlePlayer_ = TextureManager::Load("player/player.png");
	textureHandleEnemy_ = TextureManager::Load("enemy_body/enemy_body.png");
	/*textureHandlePlayer_ = TextureManager::Load("mario.jpg");
	textureHandleEnemy_ = TextureManager::Load("mario.jpg");*/
	textureHandleBullet_ = TextureManager::Load("hammer/hammer.png");
	textureHandleReticle_ = TextureManager::Load("reticle.png");

	//3Dモデルの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	modelEnemy_ = Model::CreateFromOBJ("player", true);
	//modelEnemy_ = Model::Create();
	modelBullet_ = Model::CreateFromOBJ("hammer", true);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	modelGround_.reset(Model::CreateFromOBJ("ground", true));

	// 画像
	titleTexture_[0] = TextureManager::Load("hud/titleBack.png");
	titleTexture_[1] = TextureManager::Load("hud/titleLeft.png");
	titleTexture_[2] = TextureManager::Load("hud/titleRight.png");
	scoreTexture_[0] = TextureManager::Load("hud/scoreBack.png");
	scoreTexture_[1] = TextureManager::Load("hud/scoreWord.png");
	scoreTexture_[2] = TextureManager::Load("hud/scoreWord.png");
	scoreTexture_[3] = TextureManager::Load("hud/scoreWord.png");
	scoreTexture_[4] = TextureManager::Load("hud/scoreWord.png");
	scoreTexture_[5] = TextureManager::Load("hud/scoreWord.png");
	scoreTexture_[6] = TextureManager::Load("hud/score.png");
	aTexture_[0] = TextureManager::Load("hud/a.png");
	aTexture_[1] = TextureManager::Load("hud/a.png");
	timerTexture_ = TextureManager::Load("hud/num.png");
	killEnemyTexture_ = TextureManager::Load("hud/num.png");
	optionTexture_ = TextureManager::Load("hud/option.png");
	playerHpTexture_ = TextureManager::Load("hud/hp.png");

	// スプライト
	for (int i = 0; i < 3; i++) {
		titleSprite_[i].reset(Sprite::Create(titleTexture_[i], { 0.0f, 0.0f }));
		titleSprite_[i]->SetSize({ 64.0f, 64.0f });
		titleSprite_[i]->SetTextureRect(
			{
				0.0f,
				0.0f,
			},
			{ 32.0f, 32.0f });
		titleSprite_[i]->SetPosition({ 0.0f, 0.0f });
	}
	for (int i = 0; i < 7; i++) {
		scoreSprite_[i].reset(Sprite::Create(scoreTexture_[i], { 0.0f, 0.0f }));
		scoreSprite_[i]->SetSize({ 64.0f, 64.0f });
		scoreSprite_[i]->SetTextureRect(
			{
				0.0f,
				0.0f,
			},
			{ 32.0f, 32.0f });
		scoreSprite_[i]->SetPosition({ 0.0f, 0.0f });
	}
	for (int i = 0; i < 2; i++) {
		aSprite_[i].reset(Sprite::Create(aTexture_[i], { 0.0f, 0.0f }));
		aSprite_[i]->SetSize({ 64.0f, 64.0f });
		aSprite_[i]->SetTextureRect(
			{
				0.0f,
				0.0f,
			},
			{ 32.0f, 32.0f });
		aSprite_[i]->SetPosition({ 0.0f, 0.0f });
	}
	//timerSprite_.reset(Sprite::Create(timerTexture_, { 0.0f, 0.0f }));
	//killEnemySprite_.reset(Sprite::Create(killEnemyTexture_, { 0.0f, 0.0f }));
	optionSprite_.reset(Sprite::Create(optionTexture_, { 0.0f, 0.0f }));
	optionSprite_->SetSize({ 64.0f, 64.0f });
	optionSprite_->SetTextureRect(
		{
			0.0f,
			0.0f,
		},
		{ 32.0f, 32.0f });
	optionSprite_->SetPosition({ 0.0f, 0.0f });
	for (int i = 0; i < 10; i++) {
		playerHpSprite_[i].reset(Sprite::Create(playerHpTexture_, { 0.0f, 0.0f }));
		playerHpSprite_[i]->SetSize({ 64.0f, 64.0f });
		playerHpSprite_[i]->SetTextureRect(
			{
				0.0f,
				0.0f,
			},
			{ 32.0f, 32.0f });
		playerHpSprite_[i]->SetPosition({ 0.0f, 0.0f });
	}

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	transition_ = TransitionEffect::GetInstance();
	transition_->SetIsChangeScene(false);
	transition_->Init();

	//レールカメラの生成
	/*railCamera_ = new RailCamera();
	railCamera_->Initialize(worldTransform_, { 0.00f,0.00f,0.00f });*/

	//自キャラとレールカメラの親子関係を結ぶ
	//player_->SetParent(&railCamera_->GetWorldTransform());

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowHeight, WinApp::kWindowWidth);

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向が参照するビュープロジェクションを指定する(アドレスなし)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	TitleInitialize();
}

void GameScene::Update() {

	//XINPUT_STATE joyState;

	if (transition_->GetIsChangeScene()) {
		if (transition_->GetFadeOut()) {
			scene = nowScene + 1;
			//scene = SCORE;
			if (scene > SCORE) {
				scene = TITLE;
			}
			switch (scene) {

			case TITLE:
				TitleInitialize();
				break;
			case GAME:
				GameInitialize();
				break;
			case SCORE:
				ScoreInitialize();
				break;
			}
		}
	}
	else {
		switch (scene) {

		case TITLE:
			TitleUpdate();
			break;
		case GAME:
			GameUpdate();
			break;
		case SCORE:
			ScoreUpdate();
			break;
		}
	}

	transition_->Update();

#ifdef _DEBUG

	ImGui::Begin("b");
	ImGui::Text("kill %d %d %d", killEnemyCount_, allEnemy_, ENEMY_MAX);
	ImGui::End();

#endif // _DEBUG

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

	switch (scene) {

	case TITLE:
		break;
	case GAME:
		GameDraw();
		break;
	case SCORE:
		break;
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

	switch (scene) {

	case TITLE:
		TitleDraw();
		break;
	case GAME:
		GameDrawUI();
		break;
	case SCORE:
		ScoreDraw();
		break;
	}

	// 画面遷移の描画
	transition_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::TitleInitialize() {
	isPressButton_ = false;

	titleSprite_[0]->SetSize({ 1280.0f, 720.0f });
	titleSprite_[0]->SetTextureRect(
		{
			0.0f,
			0.0f,
		},
		{ 1280.0f, 720.0f });
	titleSprite_[0]->SetPosition({ 0.0f, 0.0f });

	titleSprite_[1]->SetSize({ 1280.0f, 720.0f });
	titleSprite_[1]->SetTextureRect(
		{
			0.0f,
			0.0f,
		},
		{ 1280.0f, 720.0f });
	titleSprite_[1]->SetPosition({ -1280.0f, 0.0f });

	titleSprite_[2]->SetSize({ 1280.0f, 720.0f });
	titleSprite_[2]->SetTextureRect(
		{
			0.0f,
			0.0f,
		},
		{ 1280.0f, 720.0f });
	titleSprite_[2]->SetPosition({ 1280.0f, 0.0f });

	titlePos[0] = titleSprite_[1]->GetPosition();
	titlePos[1] = titleSprite_[2]->GetPosition();

	titleVeclocity[0] = { 1.0f,0.0f };
	titleVeclocity[1] = { 1.0f,0.0f };

	aSprite_[0]->SetSize({ 750.0f, 360.0f });
	aSprite_[0]->SetTextureRect(
		{
			0.0f,
			0.0f,
		},
		{ 1280.0f, 720.0f });
	aSprite_[0]->SetPosition({ 300.0f, 350.0f });

	aSprite_[1]->SetSize({ 1280.0f, 360.0f });
	aSprite_[1]->SetTextureRect(
		{
			1280.0f,
			0.0f,
		},
		{ 2560.0f, 720.0f });
	aSprite_[1]->SetPosition({ 300.0f, 350.0f });
	isCanPressButton_ = false;
	count = 60;
}
void GameScene::GameInitialize() {
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 10000.0f;
	viewProjection_.Initialize();

	//スカイドームの初期化
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_.get());

	//自キャラの生成
	player_ = new Player();
	//敵キャラにゲームシーンのアドレスを渡す
	player_->SetGameScene(this);
	//自キャラの初期化
	player_->Initialize(modelPlayer_, modelBullet_, textureHandlePlayer_, textureHandleBullet_, textureHandleReticle_);

	//敵発生
	LoadEnemyPopData();
	//UpdateEnemyPopCommands();

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	//カメラのビュープロジェクションを自キャラにコピー
	player_->SetViewPRojection(&followCamera_->GetViewProjection());

	//自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	//timerSprite_.reset(Sprite::Create(timerTexture_, { 0.0f, 0.0f }));

	//killEnemySprite_.reset(Sprite::Create(killEnemyTexture_, { 0.0f, 0.0f }));

	optionSprite_->SetSize({ 1280.0f, 720.0f });
	optionSprite_->SetTextureRect(
		{
			0.0f,
			0.0f,
		},
		{ 1280.0f, 720.0f });
	optionSprite_->SetPosition({ 0.0f, 0.0f });

	for (int i = 0; i < 10; i++) {
		playerHpSprite_[i]->SetSize({ 64.0f, 64.0f });
		playerHpSprite_[i]->SetTextureRect(
			{
				0.0f,
				0.0f,
			},
			{ 32.0f, 32.0f });
		playerHpSprite_[i]->SetPosition({ 0.0f, 0.0f });
	}

	killEnemyCount_ = 0;
	allEnemy_ = 0;
	isAllKillEnemy_ = false;
	isOption_ = true;
	isCanPressButton_ = false;
	isPressButton_ = true;
	count = 60;
	timeCount = 3600;

}
void GameScene::ScoreInitialize() {
	scoreSprite_[0]->SetSize({ 1280.0f, 720.0f });
	scoreSprite_[0]->SetTextureRect(
		{
			0.0f,
			0.0f,
		},
		{ 1280.0f, 720.0f });
	scoreSprite_[0]->SetPosition({ 0.0f, 0.0f });

	scoreSprite_[1]->SetSize({ 109.2f, 133.0f });
	scoreSprite_[1]->SetTextureRect(
		{
			0.0f,
			0.0f,
		},
		{ 85.2f, 133.0f });
	scoreSprite_[1]->SetPosition({ 320.0f, -150.0f });

	scoreSprite_[2]->SetSize({ 109.2f, 133.0f });
	scoreSprite_[2]->SetTextureRect(
		{
			90.2f,
			0.0f,
		},
		{ 100.2f, 133.0f });
	scoreSprite_[2]->SetPosition({ 448.0f, -150.0f });

	scoreSprite_[3]->SetSize({ 109.2f, 133.0f });
	scoreSprite_[3]->SetTextureRect(
		{
			198.4f,
			0.0f,
		},
		{ 129.2f, 133.0f });
	scoreSprite_[3]->SetPosition({ 576.0f, -150.0f });

	scoreSprite_[4]->SetSize({ 109.2f, 133.0f });
	scoreSprite_[4]->SetTextureRect(
		{
			327.6f,
			0.0f,
		},
		{ 118.2f, 133.0f });
	scoreSprite_[4]->SetPosition({ 704.0f, -150.0f });

	scoreSprite_[5]->SetSize({ 109.2f, 133.0f });
	scoreSprite_[5]->SetTextureRect(
		{
			447.8f,
			0.0f,
		},
		{ 97.2f, 133.0f });
	scoreSprite_[5]->SetPosition({ 832.0f, -150.0f });

	scoreSprite_[6]->SetSize({ 150.0f, 150.0f });
	scoreSprite_[6]->SetTextureRect(
		{
			0.0f,
			0.0f,
		},
		{ 149.8f, 179.0f });
	scoreSprite_[6]->SetPosition({ 560.0f, 360.0f });

	scorePos[0] = scoreSprite_[1]->GetPosition();
	scorePos[1] = scoreSprite_[2]->GetPosition();
	scorePos[2] = scoreSprite_[3]->GetPosition();
	scorePos[3] = scoreSprite_[4]->GetPosition();
	scorePos[4] = scoreSprite_[5]->GetPosition();

	scoreVelocity[0] = { 0.0f,6.0f };
	scoreVelocity[1] = { 0.0f,6.0f };
	scoreVelocity[2] = { 0.0f,6.0f };
	scoreVelocity[3] = { 0.0f,6.0f };
	scoreVelocity[4] = { 0.0f,6.0f };

	isScoreDraw_ = false;
	isPressButton_ = false;
	isCanPressButton_ = false;
	count = 60;
}

void GameScene::TitleUpdate() {

	nowScene = TITLE;
	XINPUT_STATE joyState;

	//文字登場
	if (isPressButton_ != 1) {
		if (titlePos[0].x >= 0.0f && titlePos[1].x <= 0.0f) {
			isPressButton_ = true;
		}
		else {
			EaseInRight(titlePos[0], titleVeclocity[0].x);
			EaseInLeft(titlePos[1], titleVeclocity[1].x);
		}
	}

	Clamp(titlePos[0].x, 0.0f, -1280.0f);
	Clamp(titlePos[1].x, 1280.0f, 0.0f);

	titleSprite_[1]->SetPosition({ titlePos[0].x, titlePos[0].y });
	titleSprite_[2]->SetPosition({ titlePos[1].x, titlePos[1].y });

	if (isPressButton_) {
		if (--count <= 0) {
			if (Input::GetInstance()->GetJoystickState(0, joyState)) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
					if (transition_->GetIsChangeScene() == false) {
						transition_->SetIsChangeScene(true);
						transition_->Reset();
						count = 60;
					}
				}

			}
		}
	}
}
void GameScene::GameUpdate() {

	nowScene = GAME;
	XINPUT_STATE joyState;

	//#ifdef _DEBUG
	//	if (input_->TriggerKey(DIK_RETURN)) {
	//		if (isDebugCameraActive_ != 1) {
	//			isDebugCameraActive_ = true;
	//		}
	//		else {
	//			isDebugCameraActive_ = false;
	//		}
	//	}
	//#endif

		//カメラの処理
	if (isDebugCameraActive_) {
		//デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
	}
	else {
		//ビュープロジェクション行列の更新と転送
		//レールカメラの更新
		//railCamera_->Update();
		//viewProjection_.matView = railCamera_->GetViewProjection().matView;
		//viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		followCamera_->Update();
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

	}
	viewProjection_.TransferMatrix();

	//スカイドームの更新
	skydome_->Update();
	ground_->Update();

	if (isOption_ && isPressButton_) {
		if (--count <= 0) {
			if (Input::GetInstance()->GetJoystickState(0, joyState)) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
					isOption_ = false;
					isPressButton_ = false;
					count = 60;
				}
			}
		}
	}

	else {

		//自キャラの更新
		player_->Update(viewProjection_);

		//弾更新
		for (PlayerBullet* playerBullet : playerBullets_) {
			playerBullet->Update();
		}
		//デスフラグの立った弾を削除
		playerBullets_.remove_if([](PlayerBullet* playerBullet) {
			if (playerBullet->IsDead()) {
				delete playerBullet;
				return true;
			}
			return false;
			}
		);
		if (playerBulletRest_ <= 0) {
			if (--playerBulletTime_ <= 0) {
				playerBulletRest_ = playerBulletMax_;
				playerBulletTime_ = playerBulletTimeMax_;
			}
		}

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
		for (EnemyBullet* enemyBullet : enemyBullets_) {
			enemyBullet->Update();
		}
		//デスフラグの立った弾を削除
		enemyBullets_.remove_if([](EnemyBullet* enemyBullet)
			{
				if (enemyBullet->IsDead()) {
					delete enemyBullet;
					return true;
				}
				return false;
			}
		);

	}

	CheckAllCollisions();

	if (allEnemy_ >= ENEMY_MAX) {
		if (allEnemy_ <= killEnemyCount_) {
			isAllKillEnemy_ = true;
		}
	}

	if (player_->GetPlayerHp() <= 0 ||
		--timeCount <= 0 ||
		isAllKillEnemy_) {

		if (transition_->GetIsChangeScene() == false) {
			transition_->SetIsChangeScene(true);
			transition_->Reset();
		}
	}
}
void GameScene::ScoreUpdate() {
	nowScene = SCORE;

	XINPUT_STATE joyState;

	//スコアの評価
	if (isAllKillEnemy_) {
		scoreSprite_[6]->SetTextureRect({ 0.0f,0.0f }, { 112.0f,179.0f });
	}
	else if (ENEMY_MAX / 0.25f <= killEnemyCount_) {
		scoreSprite_[6]->SetTextureRect({ 115.0f,0.0f }, { 170.0f,179.0f });
	}
	else if (ENEMY_MAX / 0.5f <= killEnemyCount_) {
		scoreSprite_[6]->SetTextureRect({ 287.0f,0.0f }, { 145.0f,179.0f });
	}
	else if (ENEMY_MAX / 0.75f <= killEnemyCount_) {
		scoreSprite_[6]->SetTextureRect({ 435.0f,0.0f }, { 145.0f,179.0f });
	}
	else {
		scoreSprite_[6]->SetTextureRect({ 590.0f,0.0f }, { 159.0f,179.0f });
	}

	//文字登場
	if (isScoreDraw_ != 1 && isPressButton_ != 1) {
		if (scorePos[0].y >= 140.0f) {
			if (scorePos[1].y >= 140.0f) {
				if (scorePos[2].y >= 140.0f) {
					if (scorePos[3].y >= 140.0f) {
						if (scorePos[4].y >= 140.0f) {
							isPressButton_ = true;
							isScoreDraw_ = true;
						}
						else {
							EaseOutDown(scorePos[4], scoreVelocity[4].y);
						}
					}
					else {
						EaseOutDown(scorePos[3], scoreVelocity[3].y);
					}
				}
				else {
					EaseOutDown(scorePos[2], scoreVelocity[2].y);
				}
			}
			else {
				EaseOutDown(scorePos[1], scoreVelocity[1].y);
			}
		}
		else {
			EaseOutDown(scorePos[0], scoreVelocity[0].y);
		}
	}

	Clamp(scorePos[0].y, -1000.0f, 140.0f);
	Clamp(scorePos[1].y, -1000.0f, 140.0f);
	Clamp(scorePos[2].y, -1000.0f, 140.0f);
	Clamp(scorePos[3].y, -1000.0f, 140.0f);
	Clamp(scorePos[4].y, -1000.0f, 140.0f);

	scoreSprite_[1]->SetPosition({ scorePos[0].x, scorePos[0].y });
	scoreSprite_[2]->SetPosition({ scorePos[1].x, scorePos[1].y });
	scoreSprite_[3]->SetPosition({ scorePos[2].x, scorePos[2].y });
	scoreSprite_[4]->SetPosition({ scorePos[3].x, scorePos[3].y });
	scoreSprite_[5]->SetPosition({ scorePos[4].x, scorePos[4].y });

	if (isPressButton_) {
		if (--count <= 0) {
			if (Input::GetInstance()->GetJoystickState(0, joyState)) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
					if (transition_->GetIsChangeScene() == false) {
						transition_->SetIsChangeScene(true);
						transition_->Reset();
						count = 60;
					}
				}
			}
		}
	}

}

void GameScene::TitleDraw() {
	titleSprite_[0]->Draw();
	titleSprite_[1]->Draw();
	titleSprite_[2]->Draw();
	if (isPressButton_) {
		aSprite_[0]->Draw();
	}
}
void GameScene::GameDraw() {
	//スカイドームの描画
	skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);

	//自キャラの描画
	if (!player_->IsDead()) {
		player_->Draw(viewProjection_);
	}

	//弾描画
	for (PlayerBullet* playerBullet : playerBullets_) {
		playerBullet->Draw(viewProjection_);
	}

	//敵キャラの描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}

	//敵弾描画
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Draw(viewProjection_);
	}
}
void GameScene::GameDrawUI() {
	player_->DrawUI();

	for (int32_t i = 0; i < player_->GetPlayerHp() / 20; i++) {
		playerHpSprite_[i]->SetPosition(Vector2((float)i * 80 + 30, (float)20));
		playerHpSprite_[i]->Draw();
	}

	//timerSprite_->Draw();

	//killEnemySprite_->Draw();

	if (isOption_) {
		optionSprite_->Draw();
	}
	if (isPressButton_) {
		aSprite_[0]->Draw();
	}
}
void GameScene::ScoreDraw() {
	scoreSprite_[0]->Draw();
	scoreSprite_[1]->Draw();
	scoreSprite_[2]->Draw();
	scoreSprite_[3]->Draw();
	scoreSprite_[4]->Draw();
	scoreSprite_[5]->Draw();
	if (isScoreDraw_) {
		scoreSprite_[6]->Draw();
	}
	if (isPressButton_) {
		aSprite_[1]->Draw();
	}
}
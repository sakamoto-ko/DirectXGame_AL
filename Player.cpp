#include "Player.h"

#include "MyMath.h"

#include <cassert>
#include <list>

#include "GameScene.h"

Player::Player() {

}
Player::~Player() {
	delete sprite2DReticle_;
}

Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos = {};
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

//旋回
void Player::Rotate() {
	//回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Shot() {
	assert(gameScene_);

	//弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//速度ベクトルを自機の向きに合わせて回転させる
	//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	//自機から照準オブジェクトへのベクトル
	velocity = Subtract(worldTransform3DReticle_.translation_, worldTransform_.translation_);
	velocity = Multiply(kBulletSpeed, Normalize(velocity));

	//弾を生成し、初期化
	PlayerBullet* newBullet = new PlayerBullet();
	newBullet->Initialize(
		model_,
		{ worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1],worldTransform_.matWorld_.m[3][2] }
	, velocity);

	//弾をゲームシーンに登録する
	gameScene_->AddPlayerBullet(newBullet);
}

//攻撃
void Player::Attack() {
	assert(gameScene_);

	XINPUT_STATE joyState;

	//ゲームパッド未接続なら何もせずに抜ける
	/*if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}*/

	//Rトリガーを押していたら
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			Shot();
		}
	}
	else if (input_->PushKey(DIK_SPACE) ||
		input_->IsPressMouse(0)) {
		Shot();
	}
}

void Player::OnCollision() {
	//死ぬ
	isDead_ = true;
}

//親となるトランスフォームをセット
void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
	worldTransform3DReticle_.parent_ = parent;
}

//自機のワールド座標から3Dレティクルのワールド座標を計算
void Player::Get3DReticleWorldPosition() {
	//徐壱岐から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	//自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = { 0,0,1.0f };
	//自機のワールド行列の回転を反映
	Matrix4x4 rot = MakeRotateMatrix(worldTransform_.rotation_);
	offset = Multiply(offset, rot);
	//ベクトルの長さを整える
	offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
	//3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = Add(offset, worldTransform_.translation_);
	//WorldTransformの更新と転送
	worldTransform3DReticle_.UpdateMatrix();
}

//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
//void Player::Convert3Dto2DCoord(const ViewProjection viewProjection) {
//	Vector3 spritePosition = worldTransform3DReticle_.translation_;
//	//ビューポート行列
//	Matrix4x4 matViewport = MakeViewportMatrix(0, 0,
//		WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
//	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
//	Matrix4x4 matViewProjectionViewport =
//		Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
//	//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
//	spritePosition = TransformNormal(spritePosition, matViewProjectionViewport);
//	//スプライトのレティクルに座標設定
//	sprite2DReticle_->SetPosition(Vector2(spritePosition.x, spritePosition.y));
//}

//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
void Player::GetMouseWorldPosition(const ViewProjection viewProjection) {
	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//スプライトの現在座標を取得
	Vector2 spritePosition = sprite2DReticle_->GetPosition();
	XINPUT_STATE joyState;

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	//ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += ((float)joyState.Gamepad.sThumbRX / SHRT_MAX) * 10.0f;
		spritePosition.y -= ((float)joyState.Gamepad.sThumbRY / SHRT_MAX) * 10.0f;
		//スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePosition);
	}
	else {
		//マウス座標を2Dレティクルのスプライトに代入する
		spritePosition = { (float)mousePosition.x, (float)mousePosition.y };
	}

	//ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	//ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	//合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	//スクリーン座標
	Vector3 posNear = Vector3(spritePosition.x, spritePosition.y, 0);
	Vector3 posFar = Vector3(spritePosition.x, spritePosition.y, 1);

	//スクリーン座標系からサールド座標系へ
	posNear = TransformNormal(posNear, matInverseVPV);
	posFar = TransformNormal(posFar, matInverseVPV);

	//マウスレイの方向
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);

	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 50.0f;
	worldTransform3DReticle_.translation_ = Multiply(kDistanceTestObject, mouseDirection);
	//spritePosition.x = Multiply(kDistanceTestObject, mouseDirection).x;
	//spritePosition.y = Multiply(kDistanceTestObject, mouseDirection).y;

	//worldTransform3DReticle_のワールド行列更新と転送
	worldTransform3DReticle_.UpdateMatrix();
	sprite2DReticle_->SetPosition(Vector2(spritePosition.x, spritePosition.y));

	//キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");
	ImGui::DragFloat3("Player.translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("Player.rotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::Text("2DReticle:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
		worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();
}

// 初期化
void Player::Initialize(Model* model, uint32_t textureHandle, uint32_t textureHandleReticle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	textureHandleReticle_ = textureHandleReticle;

	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("re.png");

	//スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, { worldTransform_.translation_.x,worldTransform_.translation_.y }, Vector4{ 1,1,1,1 }, Vector2{ 0.5f,0.5f });

	worldTransform_.Initialize();

	worldTransform_.translation_.z += 15;

	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

// 更新
void Player::Update(const ViewProjection viewProjection) {
	// キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	//ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// キャラクターの移動速さ
	const float kChatacterSpeed = 0.2f;

	//ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kChatacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kChatacterSpeed;
	}
	else {
		// 押した方向で移動ベクトルを変更(左右)
		if (input_->PushKey(DIK_LEFT)) {
			move.x -= kChatacterSpeed;
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move.x += kChatacterSpeed;
		}

		// 押した方向で移動ベクトルを変更(上下)
		if (input_->PushKey(DIK_UP)) {
			move.y += kChatacterSpeed;
		}
		else if (input_->PushKey(DIK_DOWN)) {
			move.y -= kChatacterSpeed;
		}
	}

	// 旋回処理
	Rotate();

	//座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	//移動限界座標
	const float kMoveLimitX = 30;
	const float kMoveLimitY = 18;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//WorldTransformの更新
	worldTransform_.UpdateMatrix();

	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	GetMouseWorldPosition(viewProjection);

	//キャラクターの攻撃処理
	Attack();
}

// 描画
void Player::Draw(ViewProjection viewProjection) {
	//model_->Draw(worldTransform3DReticle_, viewProjection, textureHandleReticle_);
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//UI描画
void Player::DrawUI() {
	sprite2DReticle_->Draw();
}
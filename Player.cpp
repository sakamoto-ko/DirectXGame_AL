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

Vector3 Player::GetReticleWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos = {};
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

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

//攻撃
void Player::Attack() {
	assert(gameScene_);

	if (input_->PushKey(DIK_SPACE)) {
		//自キャラの座標をコピー
		//DirectX::XMFLOAT3 position = worldTransform_.translation_;

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//自機から照準オブジェクトへのベクトル
		velocity = Subtract(worldTransform3DReticle_.translation_ ,worldTransform_.translation_);
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
}

void Player::OnCollision() {
	//何もしない
}

//親となるトランスフォームをセット
void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
	worldTransform3DReticle_.parent_ = parent;
}

// 初期化
void Player::Initialize(Model* model, uint32_t textureHandle, uint32_t textureHandleReticle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	textureHandleReticle_ = textureHandleReticle;

	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("reticle.png");

	//スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, { worldTransform_.translation_.x,worldTransform_.translation_.y } , Vector4{ 1,1,1,1 }, Vector2{ 0.5f,0.5f });

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

	// キャラクターの移動速さ
	const float kChatacterSpeed = 0.2f;

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

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	{
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
	//3Dレティクルもワールド座標から2Dレティクルのスクリーン座標を計算
	{
		Vector3 positionReticle = worldTransform3DReticle_.translation_;
		//ビューポート行列
		Matrix4x4 matViewport = MakeViewportMatrix(0, 0,
			WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		//ビュー行列とプロジェクション行列、ビューポート行列を合成する
		Matrix4x4 matViewProjectionViewport =
			Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
		//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
		positionReticle = TransformNormal(positionReticle, matViewProjectionViewport);
		//スプライトのレティクルに座標設定
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

		ImGui::Begin("reticlePos");
		ImGui::DragFloat3("pos", &positionReticle.x, 0.01f);
		ImGui::End();
	}

	//キャラクターの攻撃処理
	Attack();

	//キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");
	ImGui::DragFloat3("Player.translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("Player.rotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();

	ImGui::Begin("Reticle");
	ImGui::DragFloat3("Reticle.translation", &worldTransform3DReticle_.translation_.x, 0.01f);
	ImGui::DragFloat3("Reticle.rotate", &worldTransform3DReticle_.rotation_.x, 0.01f);
	ImGui::End();
}

// 描画
void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform3DReticle_, viewProjection, textureHandleReticle_);
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//UI描画
void Player::DrawUI() {
	sprite2DReticle_->Draw();
}
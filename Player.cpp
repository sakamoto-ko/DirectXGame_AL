#include "Player.h"

Player::Player() {

}
Player::~Player() {}

//旋回
void Player::Rotate() {
	//回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

//攻撃
void Player::Attack() { 
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		//弾を登録する
		bullet_ = newBullet;
	}
}

    // 初期化
void Player::Initialize(Model* model, uint32_t textureHandle) {
    assert(model); 

    model_ = model;
	textureHandle_ = textureHandle;

    worldTransform_.Initialize();

    //シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}
// 更新
void Player::Update() {
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kChatacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kChatacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kChatacterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kChatacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kChatacterSpeed;
	}

	// 旋回処理
	Rotate();

	//座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	//WorldTransformの更新
	worldTransform_.UpdateMatrix();

	//キャラクターの攻撃処理
	Attack();

	//弾更新
	if (bullet_) {
		bullet_->Update();
	}

	//キャラクターの座標を画面表示する処理
	/*ImGui::Begin("");
	ImGui::InputFloat3("Player",)
	ImGui::End();*/
}
// 描画
void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//弾描画
	if (bullet_) {
		bullet_->Draw(viewProjection);
	}
}
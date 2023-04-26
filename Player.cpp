#include "Player.h"

Player::Player() {

}
Player::~Player() {}

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
		move.y -= kChatacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y += kChatacterSpeed;
	}

	//座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	//アフィン行列の呼び出し
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	//キャラクターの座標を画面表示する処理
	/*ImGui::Begin("");
	ImGui::InputFloat3("Player",)
	ImGui::End();*/
}
// 描画
void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
#include "Player.h"
#include "MyMath.h"
#include <cassert>
#include <iostream>
#include "ImGuiManager.h"

Player::Player() {

}

Player::~Player() {

}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
}

void Player::Update() {
	//ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	//ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//速さ
		const float speed = 0.3f;
		//移動量
		Vector3 move = {
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX,
			0.0f,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX
		};
		//移動量に速さを反映
		move = Multiply(speed, Normalize(move));

		//移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 rotate = MakeRotateMatrix(viewProjection_->rotation_);
		move = TransformNormal(move, rotate);

		//移動方向に向きを合わせる
		//Y軸周り角度(θy)
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);

		//移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	}

	worldTransform_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::DragFloat3("Player.translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("Player.rotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
void Player::DrawA(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
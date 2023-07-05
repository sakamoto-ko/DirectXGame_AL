#include "Player.h"
#include "MyMath.h"
#include <cassert>
#include <iostream>
#include "ImGuiManager.h"

//浮遊ギミック初期化
void Player::InitializeFloatingGimmick() {
	floatingParameter_ = 0.0f;
}

//浮遊ギミック更新
void Player::UpdateFloatingGimmick() {
	frame_ += 1;
	if (frame_ >= 60) {
		frame_ = 0;
	}

	//浮遊移動のサイクル<frame>
	const uint16_t period = frame_;
	//1フレームでのパラメータ加算値
	const float step = 2.0f * PAI / period;
	//パラメータを1ステップ分加算
	floatingParameter_ += step;
	//2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * PAI);
	//浮遊の横幅<m>
	const float floatingWidth = 1.0f;
	//浮遊を座標に反映
	worldTransformFace_.translation_.y = std::sin(floatingParameter_) * floatingWidth;
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingWidth;
	worldTransformL_arm_.translation_.y = std::sin(floatingParameter_) * floatingWidth;
	worldTransformR_arm_.translation_.y = std::sin(floatingParameter_) * floatingWidth;
}

Player::Player() {

}

Player::~Player() {

}

void Player::Initialize(Model* modelFace, Model* modelBody, Model* modelL_arm, Model* modelR_arm, uint32_t textureHandle) {
	assert(modelFace);
	assert(modelBody);
	assert(modelL_arm);
	assert(modelR_arm);

	modelFace_.reset(modelFace);
	modelBody_.reset(modelBody);
	modelL_arm_.reset(modelL_arm);
	modelR_arm_.reset(modelR_arm);

	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	//浮遊ギミック初期化
	InitializeFloatingGimmick();
}

void Player::Update() {
	//ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	//浮遊ギミック更新
	UpdateFloatingGimmick();

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

	worldTransformFace_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::DragFloat3("Player.translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("Player.rotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	modelFace_->Draw(worldTransformFace_, viewProjection);
	modelBody_->Draw(worldTransformBody_, viewProjection);
	modelL_arm_->Draw(worldTransformL_arm_, viewProjection);
	modelR_arm_->Draw(worldTransformR_arm_, viewProjection);
}
void Player::DrawA(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
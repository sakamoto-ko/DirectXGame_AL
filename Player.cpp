#include "Player.h"
#include "MyMath.h"
#include <cassert>
#include <iostream>
#include "ImGuiManager.h"

//浮遊ギミック初期化
void Player::InitializeFloatingGimmick() {
	//浮遊移動のサイクル<frame>
	period = 60;
	//浮遊の振幅<m>
	amplitude = 0.5f;

	floatingParameter_ = 0.0f;
}

//浮遊ギミック更新
void Player::UpdateFloatingGimmick() {
	//1フレームでのパラメータ加算値
	const float step = 2.0f * PAI / period;
	//パラメータを1ステップ分加算
	floatingParameter_ += step;
	//2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * PAI);
	//浮遊を座標に反映
	worldTransformBase_.translation_.y = std::sin(floatingParameter_) * amplitude + 5.0f;
	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * amplitude;
	worldTransformR_arm_.rotation_.x = -std::sin(floatingParameter_) * amplitude;

	ImGui::Begin("Player");
	ImGui::SliderFloat3("face.translation", &worldTransformFace_.translation_.x, -10, 10);
	ImGui::SliderFloat3("body.translation", &worldTransformBody_.translation_.x, -10, 10);
	ImGui::SliderFloat3("L_arm.translation", &worldTransformL_arm_.translation_.x, -10, 10);
	ImGui::SliderFloat3("R_arm.translation", &worldTransformR_arm_.translation_.x, -10, 10);
	ImGui::SliderInt("period", reinterpret_cast<int*>(&period), 1, 144);
	ImGui::SliderFloat("amplitude", &amplitude, 0.0f, PAI * 2.0f);
	ImGui::End();
}

//通常行動初期化
void Player::BehaviorRootInitialize() {

}

//通常行動更新
void Player::BehaviorRootUpdate() {
	//既定クラスの更新
	BaseCharacter::Update();

	//浮遊ギミック更新
	UpdateFloatingGimmick();

	//ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	//ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//速さ
		const float speed = 0.3f;
		//移動量
		Vector3 move = {
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX,
			0,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX
		};
		//移動量に速さを反映
		move = Multiply(speed, Normalize(move));

		//移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 rotate = MakeRotateMatrix(viewProjection_->rotation_);
		move = TransformNormal(move, rotate);

		//移動方向に向きを合わせる
		//Y軸周り角度(θy)
		worldTransformBase_.rotation_.y = std::atan2(move.x, move.z);

		//移動
		worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, move);

		//親子関係
		worldTransformBody_.translation_ = worldTransformBase_.translation_;
		worldTransformBody_.translation_.y = worldTransformBase_.translation_.y + 3.5f;
		worldTransformBody_.rotation_.y = -worldTransformBase_.rotation_.y;

		worldTransformFace_.translation_ = worldTransformBase_.translation_;
		worldTransformFace_.translation_.y = worldTransformBase_.translation_.y + 3.5f;
		worldTransformFace_.rotation_.y = worldTransformBase_.rotation_.y;

		worldTransformL_arm_.translation_.x = worldTransformBase_.translation_.x + 0.75f;
		worldTransformL_arm_.translation_.y = worldTransformBase_.translation_.y + 3.5f;
		worldTransformL_arm_.translation_.z = worldTransformBase_.translation_.z;
		worldTransformL_arm_.rotation_.y = worldTransformBase_.rotation_.y;

		worldTransformR_arm_.translation_.x = worldTransformBase_.translation_.x - 0.75f;
		worldTransformR_arm_.translation_.y = worldTransformBase_.translation_.y + 3.5f;
		worldTransformR_arm_.translation_.z = worldTransformBase_.translation_.z;
		worldTransformR_arm_.rotation_.y = worldTransformBase_.rotation_.y;
	}

	worldTransformBase_.UpdateMatrix();
	worldTransformFace_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();
}

//攻撃行動初期化
void Player::BehaviorAttackInitialize() {
	worldTransformWeapon_.rotation_.x = 0.0f;
}

//攻撃行動更新
void Player::BehaviorAttackUpdate() {
	//既定クラスの更新
	BaseCharacter::Update();

	//ゲームパッドの状態を得る変数(XINPUT)
	//XINPUT_STATE joyState;

	if (worldTransformWeapon_.rotation_.x >= -1.5f) {
		worldTransformWeapon_.rotation_.x -= 0.1f;
	}

	worldTransformBase_.UpdateMatrix();
	worldTransformFace_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::SliderFloat3("face.translation", &worldTransformFace_.translation_.x, -10, 10);
	ImGui::SliderFloat3("body.translation", &worldTransformBody_.translation_.x, -10, 10);
	ImGui::SliderFloat3("L_arm.translation", &worldTransformL_arm_.translation_.x, -10, 10);
	ImGui::SliderFloat3("R_arm.translation", &worldTransformR_arm_.translation_.x, -10, 10);
	ImGui::SliderFloat3("weapon.translation_", &worldTransformWeapon_.translation_.x, -10, 10);
	ImGui::SliderFloat3("weapon.rotation_", &worldTransformWeapon_.rotation_.x, -10, 10);
	ImGui::End();
}

Player::Player() {}

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {
	//既定クラスの初期化
	BaseCharacter::Initialize(models);

	worldTransformBase_.Initialize();
	worldTransformFace_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformWeapon_.Initialize();

	worldTransformBase_.translation_.y = 3.5f;
	worldTransformFace_.translation_.y = 3.5f;
	worldTransformBody_.translation_.y = 3.5f;
	worldTransformL_arm_.translation_.y = 3.5f;
	worldTransformR_arm_.translation_.y = 3.5f;
	worldTransformWeapon_.translation_.y = 3.5f;

	//浮遊ギミック初期化
	InitializeFloatingGimmick();
}

void Player::Update() {
	if (behaviorRequest_) {
		//振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		//各振る舞い事の初期化を実行
		switch (behavior_){
		case Player::Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Player::Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		}
		//振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_)
	{
	//通常行動
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
		//攻撃行動
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}
}

void Player::Draw(const ViewProjection& viewProjection) {
	//既定クラスの描画
	//BaseCharacter::Draw(viewProjection);

	models_[kModelFace]->Draw(worldTransformFace_, viewProjection);
	models_[kModelBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelR_arm]->Draw(worldTransformR_arm_, viewProjection);
	models_[kModelWeapon]->Draw(worldTransformWeapon_, viewProjection);
}
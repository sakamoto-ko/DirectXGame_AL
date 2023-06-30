#include "Player.h"
#include "MyMath.h"
#include <cassert>
#include "ImGuiManager.h"

Player::Player() {

}

Player::~Player() {

}

void Player::Initialize(Model* model) {
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
}

void Player::Update() {
	//�Q�[���p�b�h�̏�Ԃ𓾂�ϐ�(XINPUT)
	XINPUT_STATE joyState;

	//�W���C�X�e�B�b�N��Ԏ擾
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//����
		const float speed = 0.3f;
		//�ړ���
		Vector3 move = { (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f, (float)joyState.Gamepad.sThumbLY / SHRT_MAX };
		//�ړ��ʂɑ����𔽉f
		move = Multiply(speed, Normalize(move));
		//�ړ�
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
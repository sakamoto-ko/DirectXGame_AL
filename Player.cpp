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
	worldTransform_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::DragFloat3("Player.translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("Player.rotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
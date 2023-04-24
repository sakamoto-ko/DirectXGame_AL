#include "Player.h"
#include <cassert>

Player::Player() {

}
Player::~Player() {}

    // ������
void Player::Initialize(Model* model, uint32_t textureHandle) {
    assert(model); 

    model_ = model;
	textureHandle_ = textureHandle;

    worldTransform_.Initialize();
}
// �X�V
void Player::Update() {
    //�s���萔�o�b�t�@�ɓ]��
	worldTransform_.TransferMatrix();
}
// �`��
void Player::Draw() {}
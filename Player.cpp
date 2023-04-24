#include "Player.h"
#include <cassert>

Player::Player() {

}
Player::~Player() {}

    // 初期化
void Player::Initialize(Model* model, uint32_t textureHandle) {
    assert(model); 

    model_ = model;
	textureHandle_ = textureHandle;

    worldTransform_.Initialize();
}
// 更新
void Player::Update() {
    //行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}
// 描画
void Player::Draw() {}
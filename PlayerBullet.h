#pragma once
#include "TextureManager.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "MyMath.h"
#include <cassert>
#include "ImGuiManager.h"

class PlayerBullet {
private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

public:
	PlayerBullet();
	~PlayerBullet();
	// 初期化
	void Initialize(Model* model, const Vector3& position);
	// 更新
	void Update();
	// 描画
	void Draw(const ViewProjection& viewProjection);
};

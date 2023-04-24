#pragma once
#include "TextureManager.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <cassert>

class Player {
private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

public:
	Player();
	~Player();
	//初期化
	void Initialize(Model* model,uint32_t textureHandle);
	//更新
	void Update();
	//描画
	void Draw(ViewProjection viewProjection);
};

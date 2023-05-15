﻿#pragma once
#include "WorldTransform.h"
#include "TextureManager.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "MyMath.h"
#include <cassert>
#include "ImGuiManager.h"

class EnemyBullet
{
private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	//速度
	Vector3 velocity_ = {};

	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

public:
	EnemyBullet();
	~EnemyBullet();
	// 初期化
	void Initialize(Model* model, const Vector3& position, const Vector3 velocity);
	// 更新
	void Update();
	// 描画
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }
};


#include "FollowCamera.h"

FollowCamera::FollowCamera() {}
FollowCamera::~FollowCamera() {}
//初期化
void FollowCamera::Initialize(WorldTransform worldTransform, Vector3 radian) {
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}
//更新
void FollowCamera::Update() {
	//追従対象がいれば
	if (target_) {
		//追従対象からカメラまでのオフセット
		Vector3 offset = { 0.0f,2.0f,-10.0f };

		//座標をコピーしてp布施ッと分ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);
	}
}
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

}
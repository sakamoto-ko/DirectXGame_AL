#include "FollowCamera.h"
#include "MyMath.h"
#include <cassert>
#include "ImGuiManager.h"

FollowCamera::FollowCamera() {}
FollowCamera::~FollowCamera() {}
//初期化
void FollowCamera::Initialize() {
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}
//更新
void FollowCamera::Update() {
	//ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;
	//ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float rotate = 0.0f;
		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotate;
	}
	//追従対象がいれば
	if (target_) {
		//追従対象からカメラまでのオフセット
		Vector3 offset = { 0.0f,2.0f,-10.0f };
		
		viewProjection_.rotation_ = MakeRotateMatrix(viewProjection_.rotation_);

		//オフセットをカメラの回転に合わせて回転させる
		offset=TransformNormal()

		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);
	}
}
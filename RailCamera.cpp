#include "RailCamera.h"

#include "MyMath.h"

#include <cassert>
#include <list>

RailCamera::RailCamera(){}
RailCamera::~RailCamera() {}

//初期化
void RailCamera::Initialize(WorldTransform worldTransform, Vector3 radian) {
	//引数でワールド座標を受け取ってワールドトランスフォームに設定
	worldTransform_ = worldTransform;
	//引数で回転角[ラジアン]を受け取ってワールドトランスフォームに設定
	worldTransform_.rotation_ = radian;
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	viewProjection_.farZ = 10000.0f;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	kMoveSpeed.z = 0.0f;
	//kRotSpeed.y = (1.0f/360.0f);
}
//更新
void RailCamera::Update() {
	//ワールドトランスフォームの座標の数値を加算したりする(移動)
	//X
	if (input_->PushKey(DIK_T)) {
		kMoveSpeed.x += 0.01f;
	}
	else if (input_->PushKey(DIK_Y)) {
		kMoveSpeed.x += -0.01f;
	}
	else {
		kMoveSpeed.x += 0.0f;
	}
	//Y
	if (input_->PushKey(DIK_U)) {
		kMoveSpeed.y += 0.01f;
	}
	else if (input_->PushKey(DIK_I)) {
		kMoveSpeed.y += -0.01f;
	}
	else {
		kMoveSpeed.y += 0.0f;
	}
	//Z
	if (input_->PushKey(DIK_O)) {
		kMoveSpeed.z += 0.01f;
	}
	else if (input_->PushKey(DIK_P)) {
		kMoveSpeed.z += -0.01f;
	}
	else {
		kMoveSpeed.z += 0.0f;
	}
	worldTransform_.translation_ = Add(worldTransform_.translation_, kMoveSpeed);

	//ワールドトランスフォームの座標の数値を加算したりする(回転)
	//X
	if (input_->PushKey(DIK_F)) {
		kRotSpeed.x += 0.01f;
	}
	else if (input_->PushKey(DIK_G)) {
		kRotSpeed.x += -0.01f;
	}
	else {
		kRotSpeed.x += 0.0f;
	}
	//Y
	if (input_->PushKey(DIK_H)) {
		kRotSpeed.y += 0.01f;
	}
	else if (input_->PushKey(DIK_J)) {
		kRotSpeed.y += -0.01f;
	}
	else {
		kRotSpeed.y += 0.0f;
	}
	//Z
	if (input_->PushKey(DIK_K)) {
		kRotSpeed.z += 0.01f;
	}
	else if (input_->PushKey(DIK_L)) {
		kRotSpeed.z += -0.01f;
	}
	else {
		kRotSpeed.z += 0.0f;
	}
	worldTransform_.rotation_ = Add(worldTransform_.rotation_, kRotSpeed);

	//ワールドトランスフォームのワールド座標再計算
	worldTransform_.UpdateMatrix();

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//カメラの座標を画面表示させる処理
	ImGui::Begin("Camera");
	ImGui::DragFloat3("Camera.translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("Camera.rotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}
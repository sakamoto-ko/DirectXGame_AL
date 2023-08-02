#pragma once
#include "Audio.h"
#include "AxisIndicator.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <variant>
#include <map>
#include <json.hpp>

//using
using json = nlohmann::json;

class GlobalVariables
{
public:
	static GlobalVariables* GetInstance();

	//項目
	struct Item {
		//項目の値
		std::variant<int32_t, float, Vector3> value;
	};
	//グループ
	struct Group {
		std::map<std::string, Item> items;
	};
	//全データ
	std::map<std::string, Group> dates_;

	//グループの作成
	void CreateGroup(const std::string& groupName);

	//値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	//値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	//値のセット(Vector3)
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	//毎フレーム処理
	void Update();

	//ファイルに書き出し
	void SaveFile(const std::string& groupName);

	//グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/GlobalVariables/";

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	//コピーインストラクタ = delete;
	//コピー代入演算子 = delete; 
};


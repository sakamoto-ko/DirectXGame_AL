#include "GlobalVariables.h"

GlobalVariables* GlobalVariables::GetInstance() {
	static GlobalVariables instance;
	return &instance;
}
//グループの作成
void GlobalVariables::CreateGroup(const std::string& groupName) {
	//指定名のオブジェクトがなければ追加する
	dates_[groupName];
}

//値のセット(int)
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value) {
	//グループの参照値を取得
	Group& group = dates_[groupName];
	//新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	//設定した項目をstd::mapに追加
	group.items[key] = newItem;
}
//値のセット(float)
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {
	//グループの参照値を取得
	Group& group = dates_[groupName];
	//新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	//設定した項目をstd::mapに追加
	group.items[key] = newItem;
}
//値のセット(Vector3)
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3& value) {
	//グループの参照値を取得
	Group& group = dates_[groupName];
	//新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	//設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

//毎フレーム処理
void GlobalVariables::Update() {
	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar()) return;

	//各グループについて
	for (std::map<std::string, Group>::iterator itGroup = dates_.begin();
		itGroup != dates_.end(); ++itGroup) {
		//グループ名を取得
		const std::string& groupName = itGroup->first;
		//グループの参照を取得
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str()))
			continue;

		//各項目について
		for (std::map<std::string, Item>::iterator itItem = group.items.begin();
			itItem != group.items.end(); ++itItem) {
			//項目名を取得
			const std::string& itemName = itItem->first;
			//項目の参照を取得
			Item& item = itItem->second;

			//int32_t型の値を保持していれば
			if (std::holds_alternative<int32_t>(item.value)) {
				int32_t* ptr = std::get_if<int32_t>(&item.value);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
			}

			//float型の値を保持していれば
			else if (std::holds_alternative<float>(item.value)) {
				float* ptr = std::get_if<float>(&item.value);
				ImGui::SliderFloat(itemName.c_str(), ptr, 0, 100);
			}

			//1Vector3型の値を保持していれば
			else if (std::holds_alternative<Vector3>(item.value)) {
				Vector3* ptr = std::get_if<Vector3>(&item.value);
				ImGui::SliderFloat3(itemName.c_str(),reinterpret_cast<float*>(ptr),-10.0f,10.0f);
			}
		}

		//改行
		ImGui::Text("\n");

		if (ImGui::Button("Save")) {
			SaveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr,message.c_str(),"GlobalVariables",0);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
	ImGui::End();
}

//ファイルに書き出し
void GlobalVariables::SaveFile(const std::string& groupName) {
	//グループを検索
	std::map<std::string, Group>::iterator itGroup = dates_.find(groupName);
	//未登録チェック
	assert(itGroup != dates_.end());

	json root;
	root = json::object();

	//jsonオブジェクト登録
	root[groupName] = json::object();

	//各項目について
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin();
		itItem != itGroup->second.items.end(); ++itItem) {
		//項目名を取得
		const std::string& itemName = itItem->first;
		//項目の参照を取得
		Item& item = itItem->second;

		//int32_t型の値を保持していれば
		if (std::holds_alternative<int32_t>(item.value)) {
			//int32_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item.value);
		}

		//float型の値を保持していれば
		else if (std::holds_alternative<float>(item.value)) {
			//float型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);
		}

		//Vector3型の値を保持していれば
		else if (std::holds_alternative<Vector3>(item.value)) {
			//Vector3型のjson配列登録
			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = json::array({value.x,value.y,value.z});
		}
	}

	//ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists("Resources/GlobalVariables/")) {
		std::filesystem::create_directory("Resources/GlobalVariables/");
	}

	//各項目のJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	//書き込み用ファイルストリーム
	std::ofstream ofs;
	//ファイルを書き込みように開く
	ofs.open(filePath);
	//ファイルオープン失敗？
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	//ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	//ファイルを閉じる
	ofs.close();
}
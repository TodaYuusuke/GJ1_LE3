#include "EnemyManager.h"

using namespace LWP;
using namespace LWP::Math;

void EnemyManager::Initialize(Player* ptr) {
	player_ = ptr;
}

void EnemyManager::Update() {
#if DEMO
	// ** ImGui用変数 ** //
	static int selectedClass = 0;	// 生成クラス
	static int currentItem = 0;	// 選ばれている番号
	// デバッグ用の召喚関数をセット
	static std::vector<std::function<void()>> summonFunction {
		[this]() { enemies_.push_back(new Spider(player_)); },
		[this]() { enemies_.push_back(new Slime(player_)); }
	};
	// 名前もセット
	static std::vector<const char*> classText {
		"Spider", "Slime"
	};

	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("Enemy")) {
			// 実験用召喚ボタン
			ImGui::Text("--- Summon Enemy ---");
			ImGui::Combo("Select Type", &selectedClass, classText.data(), static_cast<int>(EnemyType::Count));
			if (ImGui::Button("Summon")) { summonFunction[selectedClass](); }
			ImGui::Text("");
			// 一覧
			ImGui::Text("--- Enemy List ---");
			if (!enemies_.empty()) {
				// サイズオーバーケア
				if (currentItem >= enemies_.size()) {
					currentItem = enemies_.size() - 1;
				}

				std::vector<const char*> itemText;
				for (IEnemy* e : enemies_) {
					itemText.push_back(classText[static_cast<int>(e->GetType())]);
				}
				ImGui::ListBox("List", &currentItem, itemText.data(), static_cast<int>(itemText.size()), 4);
				enemies_[currentItem]->DebugGUI();
				ImGui::Text("");
				if (ImGui::Button("Delete This Enemy")) {
					delete enemies_[currentItem];
					enemies_.erase(enemies_.begin() + currentItem);
				}
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
#endif

	for (auto& enemy : enemies_) {
		enemy->Update();
	}
}

IEnemy* EnemyManager::GetNearDeadBody(LWP::Math::Vector3 pos) {
	int index = -1; // 一番近いインデックス
	int i = 0;
	float distance = 99999999.0f;	// 距離
	for (auto& enemy : enemies_) {
		float d = Vector3::Distance(enemy->GetWorldPosition(), pos);
		// 死体かつ最も近かった場合 -> 返す対象を変更
		if (enemy->behavior_ == IEnemy::Behavior::DeadBody && d < distance) {
			index = i;	// インデックスを保持
			distance = d;
		}
		i++;
	}

	// 一番近かった死体のインスタンスを渡す
	IEnemy* result = nullptr;
	if (index != -1) {
		result = enemies_[index];
		enemies_.erase(enemies_.begin() + index);
	}

	return result;
}

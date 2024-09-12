#include "EnemyManager.h"

#include <fstream>
#include <sstream>

using namespace LWP;
using namespace LWP::Math;

EnemyManager::~EnemyManager() {
	for (IEnemy* i : enemies_) {
		delete i;
	}
	enemies_.clear();
}

void EnemyManager::Initialize(Player* ptr) {
	player_ = ptr;

	enemyProperty_[int(EnemyType::Spider)].kMaxSpawn = 5;
	enemyProperty_[int(EnemyType::Spider)].summonFunction_ = [this](Vector3 pos) { enemies_.push_back(new Spider(player_, pos)); };
	enemyProperty_[int(EnemyType::Slime)].kMaxSpawn = 2;
	enemyProperty_[int(EnemyType::Slime)].summonFunction_ = [this](Vector3 pos) { enemies_.push_back(new Slime(player_, pos)); };
}

void EnemyManager::Update() {
#if DEMO
	// ** ImGui用変数 ** //
	static int selectedClass = 0;	// 生成クラス
	static int currentItem = 0;	// 選ばれている番号
	// 名前もセット
	static std::vector<const char*> classText {
		"Spider", "Slime"
	};

	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("Enemy")) {
			ImGui::Text("--- Debug Menu ---");
			if(ImGui::Button("Kill All Enemy")) {
				for (auto& e : enemies_) {
					delete e;
				}
				enemies_.clear();
			}
			ImGui::Checkbox("isSummon", &isSummon);

			// パラメータ
			ImGui::Text("--- Parameter ---");
			ImGui::DragFloat("screenOutRange", &screenOutDistance_, 0.01f);
			if (ImGui::TreeNode("EnemyProperty")) {
				for (int i = 0; i < static_cast<int>(EnemyType::Count); i++) {
					if (ImGui::TreeNode(classText[i])) {
						ImGui::InputInt("spawn", &enemyProperty_[i].spawn);
						ImGui::InputInt("kMaxSpawn", &enemyProperty_[i].kMaxSpawn, 1);
						ImGui::Text("summonInterval : %f", enemyProperty_[i].summonInterval_);
						ImGui::DragFloat("kSummonInterval", &enemyProperty_[i].kSummonInterval_, 0.01f);
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
			// 実験用召喚ボタン
			ImGui::Text("--- Summon Enemy ---");
			ImGui::Combo("Select Type", &selectedClass, classText.data(), static_cast<int>(EnemyType::Count));
			//if (ImGui::Button("Summon")) { summonFunction[selectedClass](Vector3{ 0.0f,0.0f,0.0f }); }
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

	// 現在召喚されている敵の数をカウント
	int currentSpawn[static_cast<int>(EnemyType::Count)] = {0};
	for (auto& enemy : enemies_) {
		enemy->Update();
		currentSpawn[static_cast<int>(enemy->GetType())] += 1;
	}

	for (auto& enemy : enemies_) {
		enemy->SetVolume(currentSpawn[static_cast<int>(enemy->GetType())]);
	}

	// 敵生成処理
	if (isSummon) {
		float delta = LWP::Info::GetDeltaTimeF();
		// 敵別処理
		for (int i = 0; i < static_cast<int>(EnemyType::Count); i++) {
			EnemyProperty& prop = enemyProperty_[i];

			// 一定時間ごとに敵を生成
			if (prop.summonInterval_ < 0.0f) {

				// 最大数未満 かつ 召喚数が残っている場合
				if (currentSpawn[i] < prop.kMaxSpawn && 0 < prop.spawn) {
					prop.spawn--;	// 残りスポーン数を減らす

					// 敵召喚
					Vector3 summonPos = { player_->GetWorldPosition().x,0.0f,0.0f };
					switch (Utility::GenerateRandamNum<int>(0, 1)) {
						case 0:
							// 場外じゃないかチェック
							if (summonPos.x + screenOutDistance_ > outArea_) {
								summonPos.x -= screenOutDistance_;
							}
							else {
								summonPos.x += screenOutDistance_;
							}
							break;
						case 1:
							// 場外じゃないかチェック
							if (summonPos.x - screenOutDistance_ < outArea_) {
								summonPos.x += screenOutDistance_;
							}
							else {
								summonPos.x -= screenOutDistance_;
							}
							break;
					}

					// 召喚
					prop.summonFunction_(summonPos);
					// 召喚したのでクールタイムリセット
					prop.summonInterval_ = prop.kSummonInterval_;
				}
			}
			else {
				prop.summonInterval_ -= delta;
			}
		}
	}
}

void EnemyManager::StartWave(int waveNum) {
	// レベルのステージデータを読み込む
	std::ifstream ifs("resources/csv/wave" + std::to_string(waveNum) + ".csv");
	std::string line;	// 1行分のデータ
	std::string name;	// 敵の名前
	std::string temp;	// 一時データ

	// 全行読むまでループ
	while (std::getline(ifs, line)) {
		std::istringstream stream(line);
		std::getline(stream, name, ',');	// 敵の種類
		std::getline(stream, temp, ',');	// 数

		if (name == "Spider") {
			enemyProperty_[0].spawn = std::stoi(temp);
		}
		else if (name == "Slime") {
			enemyProperty_[1].spawn = std::stoi(temp);
		}
	}
}
bool EnemyManager::GetEndWave() {
	int result = 0;
	for (int i = 0; i < static_cast<int>(EnemyType::Count); i++) {
		result += enemyProperty_[i].spawn;
	}
	result += enemies_.size();

	// 残敵0未満ならば終了
	return result <= 0;
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

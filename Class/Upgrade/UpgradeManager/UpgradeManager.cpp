#include "UpgradeManager.h"

UpgradeManager::UpgradeManager(Player* player)
{
	player_ = player;
}

UpgradeManager::~UpgradeManager()
{
	//データ削除
	upgradeDatas_.clear();
}

void UpgradeManager::Initialize()
{
	//取得フラグをすべてfalseに
	for (auto& data : upgradeDatas_) {
		data.second->isUsed_ = false;
	}
}

void UpgradeManager::DebugGUI()
{
#ifdef DEMO


	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("UpgradeManager")) {



			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
#endif // DEMO

}

void UpgradeManager::AddUpgrade(const std::string& upgradeName, IUpgrade* upgrade)
{
	upgradeDatas_[upgradeName] = upgrade;
}

void UpgradeManager::ApplyUpgrade(const std::string& upgradeName)
{
	upgradeDatas_[upgradeName]->Apply(player_,drone_);
}

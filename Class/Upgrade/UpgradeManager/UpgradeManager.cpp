#include "UpgradeManager.h"

void UpgradeManager::Initialize(Player* player, Drone* drone)
{
	// ポインタを取得する
	player_ = player; // プレイヤー
	drone_	= drone;  // ドローン
}

void UpgradeManager::Update()
{

}

void UpgradeManager::DebugGUI()
{
#ifdef DEMO

	// ImGui開始
	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("UpgradeManager")) {



			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	// ImGui終了
	ImGui::End();

#endif // DEMO

}

void UpgradeManager::AddUpgrades()
{
	/// プレイヤー系のアップグレード追加
	// ジャンプ有効化アップグレード
	UpgradeData data0;
	data0.upgrade = new ActiveCutBack(); // 新規生成
	data0.upgrade->Init();				 // 初期化
	data0.ui.isUI = 

}
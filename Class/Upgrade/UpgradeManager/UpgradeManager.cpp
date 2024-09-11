#include "UpgradeManager.h"
#include "../../UI/GameUIManager.h"

// LWPを短縮
using namespace LWP;

void UpgradeManager::Initialize(Player* player, Drone* drone, GameUIManager* ui)
{
	// ポインタを取得する
	player_		= player; // プレイヤー
	drone_		= drone;  // ドローン
	uiManager_	= ui;	  // UIマネージャー

	// 背景用スプライトのリセット
	SpriteReset(backGround_, "UI/Upgrade/BackGround.png");
	backGround_.anchorPoint = { 0.5f, 0.5f }; // アンカーポイントを設定
	backGround_.worldTF.translation = {
		static_cast<float>(Config::Window::kResolutionWidth) / 2.0f,
		static_cast<float>(Config::Window::kResolutionHeight) / 2.0f,
	}; // 座標を設定
	backGround_.worldTF.scale = {
		7.25f,
		4.0f,
		1.0f,
	}; // スケールを設定
	backGround_.material.color = { 0.0f, 0.0f, 0.0f, 0.85f }; // 色を設定
	backGround_.isActive = false;

	/// 大枠カテゴリのリセット
	// 身体
	SpriteReset(bodyParent_, "UI/Upgrade/Icon/BodyIcon.png");
	bodyParent_.anchorPoint = { 0.5f, 0.5f }; // アンカーポイントを設定
	bodyParent_.worldTF.translation = {
		960.0f,
		300.0f
	}; // 座標を設定
	bodyParent_.worldTF.scale = { 0.35f, 0.35f }; // スケール調整

	bodyParent_.isActive = false;
	// 銃
	SpriteReset(gunParent_, "UI/Upgrade/Icon/GunIcon.png");
	gunParent_.anchorPoint = { 0.5f, 0.5f }; // アンカーポイントを設定
	gunParent_.worldTF.translation = {
		420.0f,
		300.0f
	}; // 座標を設定
	gunParent_.worldTF.scale = { 0.35f, 0.35f }; // スケール調整
	gunParent_.isActive = false;
	// ドローン
	SpriteReset(droneParent_, "UI/Upgrade/Icon/DroneIcon.png");
	droneParent_.anchorPoint = { 0.5f, 0.5f }; // アンカーポイントを設定
	droneParent_.worldTF.translation = {
		1500.0f,
		300.0f
	}; // 座標を設定
	droneParent_.worldTF.scale = { 0.35f, 0.35f }; // スケール調整
	droneParent_.isActive = false;

	// アップグレードの追加
	AddUpgrades();
}

void UpgradeManager::Update()
{
	// 表示状態が切り替えられているか確認する
	if (isOpenObserver_.GetChanged()) {
		// フラグが切り替えられている場合、表示状態を切り替える
		SwitchDisplayUI(isOpenObserver_.t);
	}

	// デバッグ表示
	DebugGUI();
}

void UpgradeManager::DebugGUI()
{
#if DEMO

	// ImGui開始
	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("UpgradeManager")) {
			// ウィンドウ表示の切り替えフラグ
			ImGui::Checkbox("isOpen Window", &isOpenObserver_.t);

			// 背景スプライトのデバッグ情報表示
			if (ImGui::TreeNode("BackGround")) {
				backGround_.DebugGUI("BackGround");
				ImGui::TreePop();
			}
			// 各大カテゴリのデバッグ情報表示
			if (ImGui::TreeNode("BodyCategroy")) {
				bodyParent_.DebugGUI("BodyCategroy");
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("GunCategroy")) {
				gunParent_.DebugGUI("GunCategroy");
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("DroneCategroy")) {
				droneParent_.DebugGUI("DroneCategroy");
				ImGui::TreePop();
			}

			// ここより下はアップグレードのデバッグ表示
			ImGui::Text("Upgrades");
			// 配列内の全要素の表示、非表示を切り替える
			for (auto& [key, data] : upgrades_) {
				// ツリーノード名称の生成
				std::string treeNodeName = key + " : UI";
				// 生成した名称でツリーノード開始
				if (ImGui::TreeNode(treeNodeName.c_str())) {
					// ツリーノード名称の生成
					std::string uiName = key + " : Sprite";
					// UIのデバッグ表示
					data.ui.DebugGUI(uiName);
					// アップグレード本体のデバッグ表示
					data.upgrade->DebugGUI(player_, drone_);
					// ツリーノード終了
					ImGui::TreePop();
				}
			}

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
	/// プレイヤー関係のアップグレードの追加
	// プレイヤーの切り返し有効化
	ActiveCutBack* data0 = new ActiveCutBack(); // インスタンス生成
	AddUI(data0, BODY, Math::Vector2(400.0f, 500.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png");
	// プレイヤーのジャンプ有効化
	ActiveJump* data1 = new ActiveJump(); // インスタンス生成
	AddUI(data1, BODY, Math::Vector2(400.0f, 850.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png");
	// プレイヤーの体力増加Lv1
	HealthUPLv1* data2 = new HealthUPLv1(); // インスタンス生成
	AddUI(data2, BODY, Math::Vector2(-0.0f, 500.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png");
	// プレイヤーの体力増加Lv2
	HealthUPLv2* data3 = new HealthUPLv2(); // インスタンス生成
	AddUI(data3, BODY, Math::Vector2(0.0f, 850.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png");
	// プレイヤーの体力増加Lv3
	HealthUPLv3* data4 = new HealthUPLv3(); // インスタンス生成
	AddUI(data4, BODY, Math::Vector2(0.0f, 1200.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png");
	// プレイヤーのスライディング距離増加Lv1
	SlindingUpGradeLv1* data5 = new SlindingUpGradeLv1(); // インスタンス生成
	AddUI(data5, BODY, Math::Vector2(-400.0f, 500.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png");
	// プレイヤーのスライディング距離増加Lv2
	SlindingUpGradeLv2* data6 = new SlindingUpGradeLv2(); // インスタンス生成
	AddUI(data6, BODY, Math::Vector2(-400.0f, 850.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png");
	// プレイヤーのスライディング距離増加Lv3
	SlindingUpGradeLv3* data7 = new SlindingUpGradeLv3(); // インスタンス生成
	AddUI(data7, BODY, Math::Vector2(-400.0f, 1200.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png");

}

void UpgradeManager::SpriteReset(LWP::Primitive::Sprite& s, const std::string& texName)
{
	// テクスチャを読み込み
	s.material.texture = Resource::LoadTexture(texName.c_str());
	// UI化
	s.isUI = true;
	// ライティング無効化
	s.material.enableLighting = false;
	// アンカーポイント設定
	s.anchorPoint = { 0.5f, 0.5f };
}

void UpgradeManager::AddUI(IUpgrade* upgrade, Category category, const LWP::Math::Vector2 position, float scale, const std::string& texName)
{
	/// アップグレードの初期化
	upgrade->Init();
	// スプライトの初期化
	SpriteReset(upgrades_[upgrade->name_].ui, texName);
	
	// 登録されるカテゴリ別に親を変更
	switch (category)
	{
	case UpgradeManager::BODY:
		upgrades_[upgrade->name_].ui.worldTF.Parent(&bodyParent_.worldTF);
		break;
	case UpgradeManager::GUN:
		upgrades_[upgrade->name_].ui.worldTF.Parent(&gunParent_.worldTF);
		break;
	case UpgradeManager::DRONE:
		upgrades_[upgrade->name_].ui.worldTF.Parent(&droneParent_.worldTF);
		break;
	}

	// スプライトの位置、サイズを初期化
	upgrades_[upgrade->name_].ui.worldTF.translation = Math::Vector3(position.x, position.y, 0.0f);
	upgrades_[upgrade->name_].ui.worldTF.scale		 = Math::Vector3(scale, scale, 0.0f);
	upgrades_[upgrade->name_].ui.anchorPoint		 = { 0.5f, 0.5f };
	upgrades_[upgrade->name_].ui.isActive			 = false;

	// 対象データを追加
	upgrades_[upgrade->name_].upgrade = std::move(upgrade);
}

void UpgradeManager::SwitchDisplayUI(bool isDisplay)
{
	// アップグレード時の背景の表示を切り替える
	backGround_.isActive = isDisplay;
	// 大カテゴリ用スプライトの表示を切り替える
	bodyParent_.isActive	= isDisplay;
	gunParent_.isActive		= isDisplay;
	droneParent_.isActive	= isDisplay;

	// 配列内の全要素の表示、非表示を切り替える
	for (auto& [key, data] : upgrades_) {
		// UIの表示、非表示を切り替える
		data.ui.isActive = isDisplay;
	}

	// アップグレードUIの表示状態が切り替えフラグの状態を変更
	isOpenUpgradeWindow_ = isDisplay;

	// プレイヤーのUIの表示非表示を切り替える
	uiManager_.
}

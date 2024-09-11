#include "UpgradeManager.h"
#include "../../UI/GameUIManager.h"

// LWPを短縮
using namespace LWP;

UpgradeManager::~UpgradeManager()
{
	delete cursorSprite_;
}

void UpgradeManager::Initialize(Player* player, Drone* drone)
{
	// ポインタを取得する
	player_ = player; // プレイヤー
	drone_ = drone;  // ドローン

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

	// カーソル
	cursorSprite_ = new LWP::Primitive::Sprite();
	SpriteReset(*cursorSprite_, "UI/Upgrade/Cursor.png");
	cursorSprite_->anchorPoint = { 0.5f, 0.5f }; // アンカーポイントを設定
	cursorSprite_->worldTF.translation = {
		960.0f,
		450.0f,
		0.0f,
	}; // 座標を設定
	cursorSprite_->worldTF.scale = { 0.25f, 0.25f }; // スケール調整
	cursorSprite_->isActive = false;
}

void UpgradeManager::Update()
{
	// デバッグ表示
	DebugGUI();

	// 表示状態が切り替えられているか確認する
	if (isOpenObserver_.GetChanged()) {
		// フラグが切り替えられている場合、表示状態を切り替える
		SwitchDisplayUI(isOpenObserver_.t);
	}

	// 表示中の場合
	if (isOpenObserver_.t) {
		// カーソル座標の更新
		CursorInput();
		// 全てのアップグレードとカーソルの当たり判定を検証する
		CheckCollisionUpgrades();
	}
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

			// スキルポイント
			ImGui::InputInt("SkilPoint", &skillPoint_);

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

			// カーソルのデバッグ情報表示
			if (ImGui::TreeNode("Cursor")) {
				droneParent_.DebugGUI("Cursor");
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
	// プレイヤーのジャンプ有効化
	ActiveJump* data0 = new ActiveJump(); // インスタンス生成
	AddUI(data0, BODY, Math::Vector2(400.0f, 500.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png");
	// プレイヤーの切り返し有効化
	ActiveCutBack* data1 = new ActiveCutBack(); // インスタンス生成
	AddUI(data1, BODY, Math::Vector2(400.0f, 850.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png", "ActiveJump");
	// プレイヤーの体力増加Lv1
	HealthUPLv1* data2 = new HealthUPLv1(); // インスタンス生成
	AddUI(data2, BODY, Math::Vector2(-0.0f, 500.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png");
	// プレイヤーの体力増加Lv2
	HealthUPLv2* data3 = new HealthUPLv2(); // インスタンス生成
	AddUI(data3, BODY, Math::Vector2(0.0f, 850.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png", "HealthUPLv1");
	// プレイヤーの体力増加Lv3
	HealthUPLv3* data4 = new HealthUPLv3(); // インスタンス生成
	AddUI(data4, BODY, Math::Vector2(0.0f, 1200.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png", "HealthUPLv2");
	// プレイヤーのスライディング距離増加Lv1
	SlindingUpGradeLv1* data5 = new SlindingUpGradeLv1(); // インスタンス生成
	AddUI(data5, BODY, Math::Vector2(-400.0f, 500.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png");
	// プレイヤーのスライディング距離増加Lv2
	SlindingUpGradeLv2* data6 = new SlindingUpGradeLv2(); // インスタンス生成
	AddUI(data6, BODY, Math::Vector2(-400.0f, 850.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png", "SlindingUpGradeLv1");
	// プレイヤーのスライディング距離増加Lv3
	SlindingUpGradeLv3* data7 = new SlindingUpGradeLv3(); // インスタンス生成
	AddUI(data7, BODY, Math::Vector2(-400.0f, 1200.0f), 0.5f, "UI/Upgrade/Icon/BodyIcon.png", "SlindingUpGradeLv2");

	/// 銃関係のアップグレードの追加
	// 銃のマガジンサイズ増加Lv1
	MagazineSizeUPLv1* data8 = new MagazineSizeUPLv1(); // インスタンス生成
	AddUI(data8, GUN, Math::Vector2(600.0f, 500.0f), 0.5f, "UI/Upgrade/Icon/GunIcon.png");
	// 銃のマガジンサイズ増加Lv2
	MagazineSizeUPLv2* data9 = new MagazineSizeUPLv2(); // インスタンス生成
	AddUI(data9, GUN, Math::Vector2(600.0f, 850.0f), 0.5f, "UI/Upgrade/Icon/GunIcon.png", "MagazineSizeUPLv1");
	// 銃のマガジンサイズ増加Lv3
	MagazineSizeUPLv3* data10 = new MagazineSizeUPLv3(); // インスタンス生成
	AddUI(data10, GUN, Math::Vector2(600.0f, 1200.0f), 0.5f, "UI/Upgrade/Icon/GunIcon.png", "MagazineSizeUPLv2");
	// リロード時の弾込め数増加Lv1
	ReloadCountUpLv1* data11 = new ReloadCountUpLv1(); // インスタンス生成
	AddUI(data11, GUN, Math::Vector2(200.0f, 500.0f), 0.5f, "UI/Upgrade/Icon/GunIcon.png");
	// リロード時の弾込め数増加Lv2
	ReloadCountUpLv2* data12 = new ReloadCountUpLv2(); // インスタンス生成
	AddUI(data12, GUN, Math::Vector2(200.0f, 850.0f), 0.5f, "UI/Upgrade/Icon/GunIcon.png", "ReloadCountUpLv1");
	// リロード時の弾込め数増加Lv3
	ReloadCountUpLv3* data13 = new ReloadCountUpLv3(); // インスタンス生成
	AddUI(data13, GUN, Math::Vector2(200.0f, 1200.0f), 0.5f, "UI/Upgrade/Icon/GunIcon.png", "ReloadCountUpLv2");
	// リロード時間短縮Lv1
	ReloadSpeedUPLv1* data14 = new ReloadSpeedUPLv1(); // インスタンス生成
	AddUI(data14, GUN, Math::Vector2(-200.0f, 500.0f), 0.5f, "UI/Upgrade/Icon/GunIcon.png");
	// リロード時間短縮Lv2
	ReloadSpeedUPLv2* data15 = new ReloadSpeedUPLv2(); // インスタンス生成
	AddUI(data15, GUN, Math::Vector2(-200.0f, 850.0f), 0.5f, "UI/Upgrade/Icon/GunIcon.png", "ReloadSpeedUPLv1");
	// リロード時間短縮Lv3
	ReloadSpeedUPLv3* data16 = new ReloadSpeedUPLv3(); // インスタンス生成
	AddUI(data16, GUN, Math::Vector2(-200.0f, 1200.0f), 0.5f, "UI/Upgrade/Icon/GunIcon.png", "ReloadSpeedUPLv2");
	// 拡散率増加Lv1
	SpreadUPLv1* data17 = new SpreadUPLv1(); // インスタンス生成
	AddUI(data17, GUN, Math::Vector2(-600.0f, 500.0f), 0.5f, "UI/Upgrade/Icon/GunIcon.png");
	//  拡散率増加Lv2
	SpreadUPLv2* data18 = new SpreadUPLv2(); // インスタンス生成
	AddUI(data18, GUN, Math::Vector2(-600.0f, 850.0f), 0.5f, "UI/Upgrade/Icon/GunIcon.png", "SpreadUPLv1");
	//  拡散率増加Lv3
	SpreadUPLv3* data19 = new SpreadUPLv3(); // インスタンス生成
	AddUI(data19, GUN, Math::Vector2(-600.0f, 1200.0f), 0.5f, "UI/Upgrade/Icon/GunIcon.png", "SpreadUPLv2");

	/// ドローン関係のアップグレード
	// ドローンの回収速度アップLv1
	CorrectSpeedUPLv1* data20 = new CorrectSpeedUPLv1(); // インスタンス生成
	AddUI(data20, DRONE, Math::Vector2(-400.0f, 500.0f), 0.5f, "UI/Upgrade/Icon/DroneIcon.png");
	// ドローンの回収速度アップLv2
	CorrectSpeedUPLv2* data21 = new CorrectSpeedUPLv2(); // インスタンス生成
	AddUI(data21, DRONE, Math::Vector2(-400.0f, 850.0f), 0.5f, "UI/Upgrade/Icon/DroneIcon.png", "CorrectSpeedUPLv1");
	// ドローンの回収速度アップLv3
	CorrectSpeedUPLv3* data22 = new CorrectSpeedUPLv3(); // インスタンス生成
	AddUI(data22, DRONE, Math::Vector2(-400.0f, 1200.0f), 0.5f, "UI/Upgrade/Icon/DroneIcon.png", "CorrectSpeedUPLv2");
	// ドローンのアイテム生成敵数ダウンLv1
	GenerateItemCountDownLv1* data23 = new GenerateItemCountDownLv1(); // インスタンス生成
	AddUI(data23, DRONE, Math::Vector2(400.0f, 500.0f), 0.5f, "UI/Upgrade/Icon/DroneIcon.png");
	// ドローンのアイテム生成敵数ダウンLv2
	GenerateItemCountDownLv2* data24 = new GenerateItemCountDownLv2(); // インスタンス生成
	AddUI(data24, DRONE, Math::Vector2(400.0f, 850.0f), 0.5f, "UI/Upgrade/Icon/DroneIcon.png", "GenerateItemCountDownLv1");
	// ドローンのアイテム生成敵数ダウンLv3
	GenerateItemCountDownLv3* data25 = new GenerateItemCountDownLv3(); // インスタンス生成
	AddUI(data25, DRONE, Math::Vector2(400.0f, 1200.0f), 0.5f, "UI/Upgrade/Icon/DroneIcon.png", "GenerateItemCountDownLv2");

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

void UpgradeManager::AddUI(IUpgrade* upgrade, Category category, const LWP::Math::Vector2 position, float scale, const std::string& texName, const std::string& prevUpgradeName)
{
	/// アップグレードの初期化
	upgrade->Init(prevUpgradeName);
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
	upgrades_[upgrade->name_].ui.worldTF.scale = Math::Vector3(scale, scale, 0.0f);
	upgrades_[upgrade->name_].ui.anchorPoint = { 0.5f, 0.5f };
	upgrades_[upgrade->name_].ui.isActive = false;

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

	// カーソル表示を切り替える
	cursorSprite_->isActive = isDisplay;

	// アップグレードUIの表示状態が切り替えフラグの状態を変更
	isOpenUpgradeWindow_ = isDisplay;
}

void UpgradeManager::CheckCollisionUpgrades()
{
	// カーソルのワールド座標を取得する
	Math::Vector2 cursorPos = {
		cursorSprite_->worldTF.GetWorldPosition().x,
		cursorSprite_->worldTF.GetWorldPosition().y
	};
	// カーソルの当たり判定半径
	float cursorRadius = 20.0f;
	// アップグレートの当たり判定半径
	float upgradeRadius = 40.0f;

	// 全アップグレートとカーソルとの当たり判定を取る
	for (auto it = upgrades_.begin(); it != upgrades_.end(); it++) {
		// アップグレートのアイコンのワールド座標を取得する
		Math::Vector2 upgradePos = {
			it->second.ui.worldTF.GetWorldPosition().x,
			it->second.ui.worldTF.GetWorldPosition().y
		};

		// 前提アップグレードが存在しない場合
		if (it->second.upgrade->prevUpgradeName_ == "" && !it->second.upgrade->isApplied_) {
			// 衝突判定を検証する
			if (CheckCollision2Upgrade(cursorPos, cursorRadius, upgradePos, upgradeRadius)) {
				// スキルポイントが1以上なら
				if (skillPoint_ >= 1) {
					// スケールを線形補間で大きくする
					it->second.ui.worldTF.scale =
						Utility::Interp::Lerp(
							it->second.ui.worldTF.scale, Math::Vector3(0.75f, 0.75f, 1.0f), 0.25f
						);

					// スペースキーが押されたら
					if (Input::Keyboard::GetTrigger(DIK_SPACE)) {
						// 適用関数を呼び出す
						it->second.upgrade->Apply(player_, drone_);
						// スプライトの色を少し薄くする
						it->second.ui.material.color = { 0.5f, 0.5f, 0.5f, 1.0f };

						// スキルポイントを - 1
						skillPoint_--;
					}
				}
				else { // それ以外であれば
					// スケールを線形補間で小さくする
					it->second.ui.worldTF.scale =
						Utility::Interp::Lerp(
							it->second.ui.worldTF.scale, Math::Vector3(0.5f, 0.5f, 1.0f), 0.35f
						);
				}
			}
			else { // 選択されていない場合
				// スケールを線形補間で小さくする
				it->second.ui.worldTF.scale =
					Utility::Interp::Lerp(
						it->second.ui.worldTF.scale, Math::Vector3(0.5f, 0.5f, 1.0f), 0.35f
					);
			}
		}
		else if(it->second.upgrade->prevUpgradeName_ != "") { // 前提アップグレードが存在する場合
			// 前提アップグレードが解放されている場合
			if (upgrades_[it->second.upgrade->prevUpgradeName_].upgrade->isApplied_ && !it->second.upgrade->isApplied_) {
				// スプライトの色を戻す
				it->second.ui.material.color = { 1.0f, 1.0f, 1.0f, 1.0f };

				// 衝突判定を検証する
				if (CheckCollision2Upgrade(cursorPos, cursorRadius, upgradePos, upgradeRadius)) {
					// スキルポイントが1以上かつ、適用していない状態であれば
					if (skillPoint_ >= 1) {
						// スケールを線形補間で大きくする
						it->second.ui.worldTF.scale =
							Utility::Interp::Lerp(
								it->second.ui.worldTF.scale, Math::Vector3(0.75f, 0.75f, 1.0f), 0.25f
							);

						// スペースキーが押されたら
						if (Input::Keyboard::GetTrigger(DIK_SPACE)) {
							// 適用関数を呼び出す
							it->second.upgrade->Apply(player_, drone_);
							// スプライトの色を少し薄くする
							it->second.ui.material.color = { 0.5f, 0.5f, 0.5f, 1.0f };

							// スキルポイントを - 1
							skillPoint_--;
						}
					}
					else { // それ以外であれば
						// スケールを線形補間で小さくする
						it->second.ui.worldTF.scale =
							Utility::Interp::Lerp(
								it->second.ui.worldTF.scale, Math::Vector3(0.5f, 0.5f, 1.0f), 0.35f
							);
					}
				}
				else { // 選択されていない場合
					// スケールを線形補間で小さくする
					it->second.ui.worldTF.scale =
						Utility::Interp::Lerp(
							it->second.ui.worldTF.scale, Math::Vector3(0.5f, 0.5f, 1.0f), 0.35f
						);
				}
			}
			else { // 解放されていない場合
				// スプライトの色を少し薄くする
				it->second.ui.material.color = { 0.5f, 0.5f, 0.5f, 1.0f };

				// スケールを線形補間で小さくする
				it->second.ui.worldTF.scale =
					Utility::Interp::Lerp(
						it->second.ui.worldTF.scale, Math::Vector3(0.5f, 0.5f, 1.0f), 0.35f
				);
			}
		}
		else { // 選択されていない場合
			// スケールを線形補間で小さくする
			it->second.ui.worldTF.scale =
				Utility::Interp::Lerp(
					it->second.ui.worldTF.scale, Math::Vector3(0.5f, 0.5f, 1.0f), 0.35f
				);
		}
	}
}

bool UpgradeManager::CheckCollision2Upgrade(const LWP::Math::Vector2& p1, const float r1, const LWP::Math::Vector2& p2, const float r2)
{
	// それぞれの中心点との距離を計算する
	float d = std::sqrtf(std::powf(p2.x - p1.x, 2) + std::powf(p2.y - p1.y, 2));

	// 半径より小さければ衝突している
	return d <= (r1 + r2);
}

void UpgradeManager::CursorInput()
{
	Math::Vector2 input = {
		0.0f, 0.0f
	};

	// キーボード入力によってカーソルの移動ベクトルを設定する
	if (Input::Keyboard::GetPress(DIK_UPARROW)) {
		input.y = -1.0f;
	}
	if (Input::Keyboard::GetPress(DIK_DOWNARROW)) {
		input.y = 1.0f;
	}
	if (Input::Keyboard::GetPress(DIK_LEFTARROW)) {
		input.x = -1.0f;
	}
	if (Input::Keyboard::GetPress(DIK_RIGHTARROW)) {
		input.x = 1.0f;
	}

	// 入力を正規化する
	input = input.Normalize();

	// カーソルの座標を移動させる
	cursorSprite_->worldTF.translation += Math::Vector3(input.x, input.y, 0.0f) * 10.0f;

	/// カーソルの移動範囲を画面内に制限する
	// X軸
	if (cursorSprite_->worldTF.translation.x > (static_cast<float>(LWP::Config::Window::kResolutionWidth) - (cursorSprite_->size.t.x * cursorSprite_->worldTF.scale.x) / 2.0f)) {
		cursorSprite_->worldTF.translation.x = (static_cast<float>(LWP::Config::Window::kResolutionWidth) - (cursorSprite_->size.t.x * cursorSprite_->worldTF.scale.x) / 2.0f);
	}
	else if (cursorSprite_->worldTF.translation.x < ((cursorSprite_->size.t.x * cursorSprite_->worldTF.scale.x) / 2.0f)){
		cursorSprite_->worldTF.translation.x = ((cursorSprite_->size.t.x * cursorSprite_->worldTF.scale.x) / 2.0f);
	}
	// Y軸
	if (cursorSprite_->worldTF.translation.y > (static_cast<float>(LWP::Config::Window::kResolutionHeight) - (cursorSprite_->size.t.y * cursorSprite_->worldTF.scale.y) / 2.0f)) {
		cursorSprite_->worldTF.translation.y = (static_cast<float>(LWP::Config::Window::kResolutionHeight) - (cursorSprite_->size.t.y * cursorSprite_->worldTF.scale.y) / 2.0f);
	}
	else if (cursorSprite_->worldTF.translation.y < ((cursorSprite_->size.t.y * cursorSprite_->worldTF.scale.y) / 2.0f)){
		cursorSprite_->worldTF.translation.y = ((cursorSprite_->size.t.y * cursorSprite_->worldTF.scale.y) / 2.0f);
	}

}

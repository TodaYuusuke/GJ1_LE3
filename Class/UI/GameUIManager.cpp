#include "GameUIManager.h"
#include "../Player/Player.h"

using namespace LWP;

void GameUIManager::Initialize(Player* player)
{
	// メンバ変数にプレイヤーを格納
	player_ = player;

	// スプライトを最大HP分生成
	for (int i = 0; i < player->maxHp_; i++) {
		// 新規背景スプライト生成
		Primitive::Sprite& b = hpGaugeBG_.emplace_back();
		b.material.texture = Resource::LoadTexture("UI/HPGauge/HPGauge_BG.png");
		b.isUI = true;
		b.material.enableLighting = false;
		// 新規本体スプライト生成
		Primitive::Sprite& s = hpGauge_.emplace_back();
		s.material.texture = Resource::LoadTexture("UI/HPGauge/HPGauge.png");
		s.isUI = true;
		s.material.enableLighting = false;

		// 最初のスプライトのみ個別の設定
		if (i == 0) {
			b.worldTF.translation = { 50.0f, 30.0f };
			b.worldTF.scale = { 0.4f, 0.4f };
		}
	}
}

void GameUIManager::Update()
{
	// UI関係のセットアップが終わっていない場合セットアップを行う
	if (!isSetUpUI_) {
		SetUp();
		// フラグをtrue
		isSetUpUI_ = true;
	}

	// デバッグ時のみ実行
	#ifdef _DEBUG
	
	// デバッグ時のみHPゲージを最初の座標に合わせる
	for (int i = 1; i < player_->maxHp_; i++) {
		// 0のスプライトから位置を合わせる
		hpGaugeBG_[i].worldTF.translation = hpGaugeBG_[0].worldTF.translation;
		hpGaugeBG_[i].worldTF.translation.x += (hpGaugeBG_[0].worldTF.scale.x * hpGaugeBG_[0].size.t.x) * i - (hpGaugeOffsetX_ * i * hpGaugeBG_[0].worldTF.scale.x);
		hpGaugeBG_[i].worldTF.scale = hpGaugeBG_[0].worldTF.scale;
		hpGauge_[i].worldTF.translation = hpGaugeBG_[0].worldTF.translation;
		hpGauge_[i].worldTF.translation.x += (hpGaugeBG_[0].worldTF.scale.x * hpGaugeBG_[0].size.t.x) * i - (hpGaugeOffsetX_ * i * hpGaugeBG_[0].worldTF.scale.x);
		hpGauge_[i].worldTF.scale = hpGaugeBG_[0].worldTF.scale;

		// 最初のスプライトのみ個別に設定
		if (i == 1) {
			hpGauge_[0].worldTF.translation = hpGaugeBG_[0].worldTF.translation;
			hpGauge_[0].worldTF.scale = hpGaugeBG_[0].worldTF.scale;
		}
	}

	// デバッグGUIの表示
	DebugGUI();

	#endif // _DEBUG

}

void GameUIManager::DebugGUI()
{
	// ImGuiを開始
	ImGui::Begin("Game");
	// タブ開始
	if (ImGui::BeginTabBar("LWP")) {
		// 子アイテム生成
		if (ImGui::BeginTabItem("UI")) {
			// HPゲージ系スプライト
			if (ImGui::TreeNode("HPGauge")) {
				// 最初のスプライトのImGuiを表示
				hpGaugeBG_[0].DebugGUI();
				// オフセット指定
				ImGui::DragFloat("HPGauge_Offset", &hpGaugeOffsetX_);

				// 終了
				ImGui::TreePop();
			}

			// 終了
			ImGui::EndTabItem();
		}
		// 終了
		ImGui::EndTabBar();
	}

	// 終了
	ImGui::End();
}

void GameUIManager::SetUp()
{
	/// HPゲージの大きさを最初のスプライトに合わせる
	// デバッグ時のみHPゲージを最初の座標に合わせる
	for (int i = 1; i < player_->maxHp_; i++) {

		// 0のスプライトから位置を合わせる
		hpGaugeBG_[i].worldTF.translation = hpGaugeBG_[0].worldTF.translation;
		hpGaugeBG_[i].worldTF.translation.x += (hpGaugeBG_[0].worldTF.scale.x * hpGaugeBG_[0].size.t.x) * i - (hpGaugeOffsetX_ * i * hpGaugeBG_[0].worldTF.scale.x);
		hpGaugeBG_[i].worldTF.scale = hpGaugeBG_[0].worldTF.scale;
		hpGauge_[i].worldTF.translation = hpGaugeBG_[0].worldTF.translation;
		hpGauge_[i].worldTF.translation.x += (hpGaugeBG_[0].worldTF.scale.x * hpGaugeBG_[0].size.t.x) * i - (hpGaugeOffsetX_ * i * hpGaugeBG_[0].worldTF.scale.x);
		hpGauge_[i].worldTF.scale = hpGaugeBG_[0].worldTF.scale;

		// 最初のスプライトのみ個別に設定
		if (i == 1) {
			hpGauge_[0].worldTF.translation = hpGaugeBG_[0].worldTF.translation;
			hpGauge_[0].worldTF.scale = hpGaugeBG_[0].worldTF.scale;
		}
	}
}

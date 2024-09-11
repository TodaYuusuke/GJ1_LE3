#include "GameUIManager.h"
#include "../Player/Player.h"

using namespace LWP;

void GameUIManager::Initialize(Player* player)
{
	// メンバ変数にプレイヤーを格納
	player_ = player;

	// オブザーバーに各変数を渡す
	healthObserver_.Init(&player_->parameters_.hp); // HP
	bulletObserver_.Init(&player_->parameters_.bulletData.ammoRemaining_); // 残弾数

	// スプライトを最大HP分生成
	for (int i = 0; i < player_->parameters_.maxHp; i++) {
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

	// 弾用スプライトを最大数分生成
	for (int i = 0; i < player_->parameters_.bulletData.maxAmmoNum_; i++) {
		// 新規背景スプライト生成
		Primitive::Sprite& b = BulletsUI_.emplace_back();
		b.material.texture = Resource::LoadTexture("UI/Bullet/Bullet.png");
		b.isUI = true;
		b.material.enableLighting = false;

		// 最初のスプライトのみ個別の設定
		if (i == 0) {
			b.worldTF.translation = { 0.0f,0.0f };
			b.worldTF.scale = { 0.35f, 0.35f };
		}
	}
}

void GameUIManager::Update()
{
	// デバッグ
	Debug();

	// UI関係のセットアップが終わっていない場合セットアップを行う
	if (!isSetUpUI_) {
		SetUp();
		// フラグをtrue
		isSetUpUI_ = true;
	}

	// UIの表示状態が切り替わっている場合
	if (isDisplayObserver_.GetChanged()) {
		SwitchDisplayUI(isDisplayObserver_.t);
	}

	// 表示状態の場合は更新
	if (isDisplayObserver_.t) {
		// HPゲージ更新
		HPGaugeUpdate();
		// 弾ゲージ更新
		BulletGaugeUpdate();
	}
}

void GameUIManager::DebugGUI()
{
	// ImGuiを開始
	ImGui::Begin("Game");
	// タブ開始
	if (ImGui::BeginTabBar("LWP")) {
		// 子アイテム生成
		if (ImGui::BeginTabItem("UI")) {
			// 表示トリガー切り替え
			ImGui::Checkbox("IsDisplay", &isDisplayObserver_.t);
			
			// HPゲージ系スプライト
			if (ImGui::TreeNode("HPGauge")) {
				// 最初のスプライトのImGuiを表示
				hpGaugeBG_[0].DebugGUI();
				// オフセット指定
				ImGui::DragFloat("HPGauge_Offset", &hpGaugeOffsetX_);

				// 終了
				ImGui::TreePop();
			}

			// 弾数スプライト
			if (ImGui::TreeNode("Bullets")) {
				// 最初の弾のImGuiを表示
				BulletsUI_[0].DebugGUI();
				// オフセット指定
				ImGui::DragFloat("Bullets_Offset", &BulletGaugeOffsetX_);
				ImGui::DragFloat2("Bullets_Anhor", &bulletsAnchor_.x, 1.0f);

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
	for (int i = 1; i < player_->parameters_.maxHp; i++) {
		// 生成済みスプライトで足りない場合新しいスプライトを生成
		if (hpGaugeBG_.size() <= i) {
			// 新規背景スプライト生成
			Primitive::Sprite& b = hpGaugeBG_.emplace_back();
			b.material.texture = Resource::LoadTexture("UI/HPGauge/HPGauge_BG.png");
			b.isUI = true;
			b.material.enableLighting = false;
			// 新規本体スプライト生成
			Primitive::Sprite& s = hpGauge_.emplace_back();
			s.material.texture = Resource::LoadTexture("UI/HPGauge/HPGauge.png");
			s.isUI = true;
			s.isActive = false;
			s.material.enableLighting = false;
		}

		// 0のスプライトから位置を合わせる
		hpGaugeBG_[i].worldTF.translation = hpGaugeBG_[0].worldTF.translation;
		hpGaugeBG_[i].worldTF.translation.x += (hpGaugeBG_[0].worldTF.scale.x * hpGaugeBG_[0].size.t.x) * i - (hpGaugeOffsetX_ * i * hpGaugeBG_[0].worldTF.scale.x);
		hpGaugeBG_[i].worldTF.scale = hpGaugeBG_[0].worldTF.scale;
		hpGaugeBG_[i].isActive = true;
		hpGauge_[i].worldTF.translation = hpGaugeBG_[0].worldTF.translation;
		hpGauge_[i].worldTF.translation.x += (hpGaugeBG_[0].worldTF.scale.x * hpGaugeBG_[0].size.t.x) * i - (hpGaugeOffsetX_ * i * hpGaugeBG_[0].worldTF.scale.x);
		hpGauge_[i].worldTF.scale = hpGaugeBG_[0].worldTF.scale;

		// 最初のスプライトのみ個別に設定
		if (i == 1) {
			hpGauge_[0].worldTF.translation = hpGaugeBG_[0].worldTF.translation;
			hpGauge_[0].worldTF.scale = hpGaugeBG_[0].worldTF.scale;
		}
	}
	// スプライト数よりHP最大数の方が小さい場合
	if (hpGaugeBG_.size() >= player_->parameters_.maxHp) {
		// HP最大数以外
		for (int i = player_->parameters_.maxHp; i < hpGaugeBG_.size(); i++) {
			hpGaugeBG_[i].isActive = false;
			hpGauge_[i].isActive = false;
		}
	}

	/// 弾ゲージの大きさを最初のスプライトに合わせる
	for (int i = 1; i < player_->parameters_.bulletData.maxAmmoNum_; i++) {
		// 生成済みスプライトで足りない場合新しいスプライトを生成
		if (BulletsUI_.size() <= i) {
			// 新規背景スプライト生成
			Primitive::Sprite& b = BulletsUI_.emplace_back();
			b.material.texture = Resource::LoadTexture("UI/Bullet/Bullet.png");
			b.isUI = true;
			b.material.enableLighting = false;
		}

		// 0のスプライトから位置を合わせる
		BulletsUI_[i].worldTF.translation = { bulletsAnchor_.x, bulletsAnchor_.y, 0.0f };
		BulletsUI_[i].worldTF.translation.x += (BulletsUI_[0].worldTF.scale.x * BulletsUI_[0].size.t.x) * i - (BulletGaugeOffsetX_ * i * BulletsUI_[0].worldTF.scale.x);
		BulletsUI_[i].worldTF.scale = BulletsUI_[0].worldTF.scale;

		// 最初のスプライトのみ個別に設定
		if (i == 1) {
			BulletsUI_[0].worldTF.translation = { bulletsAnchor_.x, bulletsAnchor_.y, 0.0f };
			hpGauge_[0].worldTF.scale = hpGaugeBG_[0].worldTF.scale;
		}
	}
	// スプライト数より弾最大数の方が小さい場合
	if (BulletsUI_.size() >= player_->parameters_.bulletData.maxAmmoNum_) {
		// 弾最大数以外
		for (int i = player_->parameters_.bulletData.maxAmmoNum_; i < BulletsUI_.size(); i++) {
			BulletsUI_[i].isActive = false;
		}
	}
}

void GameUIManager::Debug()
{
	// デバッグ時のみ実行
#ifdef _DEBUG

	// セットアップを毎フレーム実行
	SetUp();

	// デバッグGUIの表示
	DebugGUI();

#endif // _DEBUG
}

void GameUIManager::HPGaugeUpdate()
{
	// 現在HPと前フレームHPが一致していない場合
	if (healthObserver_.GetChanged()) {
		// HP分ゲージの表示非表示を切り替える
		for (int i = 0; i < player_->parameters_.maxHp; i++) {
			// iがプレイヤーのHPを上回っていれば
			if (i + 1 > player_->parameters_.hp) {
				hpGauge_[i].isActive = false;
			}
			else {
				hpGauge_[i].isActive = true;
			}
		}
	}
}

void GameUIManager::BulletGaugeUpdate()
{
	// 現在HPと前フレームHPが一致していない場合
	if (bulletObserver_.GetChanged()) {
		// HP分ゲージの表示非表示を切り替える
		for (int i = 0; i < player_->parameters_.bulletData.maxAmmoNum_; i++) {
			// iがプレイヤーのHPを上回っていれば
			if (i + 1 > player_->parameters_.bulletData.ammoRemaining_) {
				BulletsUI_[i].isActive = false;
			}
			else {
				BulletsUI_[i].isActive = true;
			}
		}
	}
}

void GameUIManager::SwitchDisplayUI(bool isDisplay)
{
	//　HPゲージの表示を非表示
	for (int i = 0; i < player_->parameters_.maxHp; i++) {
		hpGauge_[i].isActive = isDisplay;
	}

	//　弾ゲージの表示を非表示
	for (int i = 0; i < player_->parameters_.bulletData.maxAmmoNum_; i++) {
		BulletsUI_[i].isActive = isDisplay;
	}
}


#include "GameUIManager.h"
#include "../Player/Player.h"

using namespace LWP;
using namespace LWP::Math;

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

	// ボタンUI初期化
	ButtonUIInit();
}

void GameUIManager::Update(int wave, int enemyCount) {
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
		// ボタンUIの更新
		ButtonUIUpdate();
		// ウェーブ更新
		waveSprite_.Update(wave);
		// エネミー更新
		enemySprite_.Update(enemyCount);
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
		// ボタンUIの更新
		ButtonUIUpdate();
	}

	waveSprite_.number.n[0].isActive = false;
	waveSprite_.text.isActive = false;

	enemySprite_.text.isActive = false;

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
			
			// 左スティック
			if (ImGui::TreeNode("LeftStick")) {
				leftStickUI_.DebugGUI();
				ImGui::TreePop();
			}
			// Bボタン
			if (ImGui::TreeNode("BButton")) {
				buttonBUI_.DebugGUI();
				ImGui::TreePop();
			}
			// 右トリガー
			if (ImGui::TreeNode("RTrigger")) {
				triggerRUI_.DebugGUI();
				ImGui::TreePop();
			}
			// Aボタン
			if (ImGui::TreeNode("Abutton")) {
				buttonAUI_.DebugGUI();
				ImGui::TreePop();
			}

			// 左スティック
			if (ImGui::TreeNode("move")) {
				moveText_.DebugGUI();
				ImGui::TreePop();
			}
			// Aボタン
			if (ImGui::TreeNode("sliding")) {
				slidingText_.DebugGUI();
				ImGui::TreePop();
			}
			// Bボタン
			if (ImGui::TreeNode("Shot")) {
				shotText_.DebugGUI();
				ImGui::TreePop();
			}
			// 右トリガー
			if (ImGui::TreeNode("jump")) {
				jumpText_.DebugGUI();
				ImGui::TreePop();
			}

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
	
	// 強制リロード
	player_->parameters_.bulletData.ammoRemaining_ = player_->parameters_.bulletData.maxAmmoNum_;

	// 初期化
	waveSprite_.Init({ 753.0f,24.0f,0.0f });
	enemySprite_.Init({ 834.0f,137.0f,0.0f });
}

void GameUIManager::Debug()
{
	// デバッグ時のみ実行
#ifdef _DEBUG

	// セットアップを毎フレーム実行
	//SetUp();

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

void GameUIManager::ButtonUIInit()
{
	/// ボタン系スプライト
	// 左スティック
	leftStickUI_.material.texture = Resource::LoadTexture("UI/Button/joystick1_left_N.png");
	leftStickUI_.worldTF.translation = {
		1500.0f, 10.0f
	};
	leftStickUI_.worldTF.scale = {
		0.4f, 0.4f, 1.0f
	};
	leftStickUI_.isUI = true;
	leftStickUI_.material.enableLighting = false;
	// Bボタン
	buttonBUI_.material.texture = Resource::LoadTexture("UI/Button/button_b_N.png");
	buttonBUI_.worldTF.translation = {
		1500.0f, 65.0f
	};
	buttonBUI_.worldTF.scale = {
		0.4f, 0.4f, 1.0f
	};
	buttonBUI_.isUI = true;
	buttonBUI_.material.enableLighting = false;
	// 右トリガー
	triggerRUI_.material.texture = Resource::LoadTexture("UI/Button/rt_N.png");
	triggerRUI_.worldTF.translation = {
		1500.0f, 120.0f
	};
	triggerRUI_.worldTF.scale = {
		0.4f, 0.4f, 1.0f
	};
	triggerRUI_.isUI = true;
	triggerRUI_.material.enableLighting = false;
	// Aボタン
	buttonAUI_.material.texture = Resource::LoadTexture("UI/Button/button_a_N.png");
	buttonAUI_.worldTF.translation = {
		1500.0f, 175.0f
	};
	buttonAUI_.worldTF.scale = {
		0.4f, 0.4f, 1.0f
	};
	buttonAUI_.isUI = true;
	buttonAUI_.material.enableLighting = false;

	// 行動名UI初期化
	// 移動
	moveText_.material.texture = Resource::LoadTexture("UI/Text/Move.png");
	moveText_.worldTF.Parent(&leftStickUI_.worldTF);
	moveText_.worldTF.translation = { 125.0f, 0.0f };
	moveText_.isUI = true;
	moveText_.material.enableLighting = false;
	// スライディング
	slidingText_.material.texture = Resource::LoadTexture("UI/Text/Sliding.png");
	slidingText_.worldTF.Parent(&buttonBUI_.worldTF);
	slidingText_.worldTF.translation = { 125.0f, 0.0f };
	slidingText_.isUI = true;
	slidingText_.material.enableLighting = false;
	// 射撃
	shotText_.material.texture = Resource::LoadTexture("UI/Text/Shot.png");
	shotText_.worldTF.Parent(&triggerRUI_.worldTF);
	shotText_.worldTF.translation = { 125.0f, 0.0f };
	shotText_.isUI = true;
	shotText_.material.enableLighting = false;
	// ジャンプ
	jumpText_.material.texture = Resource::LoadTexture("UI/Text/Jump.png");
	jumpText_.worldTF.Parent(&buttonAUI_.worldTF);
	jumpText_.worldTF.translation = { 125.0f, 0.0f };
	jumpText_.isUI = true;
	jumpText_.material.enableLighting = false;
	// ジャンプのみは個別で設定、有効化された段階で表示させる
	jumpText_.isActive = false;

	// キーUI初期化
	// 移動
	moveKeyUI_.material.texture = Resource::LoadTexture("UI/KeyBoard/MoveKey.png");
	moveKeyUI_.worldTF.Parent(&leftStickUI_.worldTF);
	moveKeyUI_.worldTF.translation = { -165.0f, 0.0f };
	moveKeyUI_.isUI = true;
	moveKeyUI_.material.enableLighting = false;
	// スライディング
	shiftKeyUI_.material.texture = Resource::LoadTexture("UI/KeyBoard/SHIFT.png");
	shiftKeyUI_.worldTF.Parent(&buttonBUI_.worldTF);
	shiftKeyUI_.worldTF.translation = { -165.0f, 0.0f };
	shiftKeyUI_.isUI = true;
	shiftKeyUI_.material.enableLighting = false;
	// 射撃
	spaceKeyUI＿.material.texture = Resource::LoadTexture("UI/KeyBoard/SPACE.png");
	spaceKeyUI＿.worldTF.Parent(&triggerRUI_.worldTF);
	spaceKeyUI＿.worldTF.translation = { -165.0f, 0.0f };
	spaceKeyUI＿.isUI = true;
	spaceKeyUI＿.material.enableLighting = false;
	// ジャンプ
	wKeyUI_.material.texture = Resource::LoadTexture("UI/KeyBoard/W.png");
	wKeyUI_.worldTF.Parent(&buttonAUI_.worldTF);
	wKeyUI_.worldTF.translation = { -165.0f, 0.0f };
	wKeyUI_.isUI = true;
	wKeyUI_.material.enableLighting = false;

}

void GameUIManager::ButtonUIUpdate()
{

	// ジャンプUIの表示を有効化、非有効化で切り替える
	jumpText_.isActive = player_->parameters_.activeFlag.jump;
}

void GameUIManager::SwitchDisplayUI(bool isDisplay)
{
	//　HPゲージの表示を非表示
	for (int i = 0; i < player_->parameters_.hp; i++) {
		hpGauge_[i].isActive = isDisplay;
	}

	//　HPゲージ背景の表示を非表示
	for (int i = 0; i < player_->parameters_.maxHp; i++) {
		hpGaugeBG_[i].isActive = isDisplay;
	}

	//　弾ゲージの表示を非表示
	for (int i = 0; i < player_->parameters_.bulletData.maxAmmoNum_; i++) {
		BulletsUI_[i].isActive = isDisplay;
	}

	// ウェーブとエネミーも非表示
	waveSprite_.text.isActive = isDisplay;
	enemySprite_.text.isActive = isDisplay;

	// 数字はどちらにしろfalseで
	for (int i = 0; i < 10; i++) {
		waveSprite_.number.n[i].isActive = false;
		enemySprite_.numFirst.n[i].isActive = false;
		enemySprite_.numSecond.n[i].isActive = false;
	}

	// ボタン系UIの表示表示
	leftStickUI_.isActive = isDisplay;
	buttonAUI_.isActive = isDisplay;
	buttonBUI_.isActive = isDisplay;
	triggerRUI_.isActive = isDisplay;

	// キー系UIの表示非表示
	moveKeyUI_.isActive = isDisplay;
	shiftKeyUI_.isActive = isDisplay;
	spaceKeyUI＿.isActive = isDisplay;
	wKeyUI_.isActive = isDisplay;

	// 行動名系UIの表示非表示
	moveText_.isActive = isDisplay;
	slidingText_.isActive = isDisplay;
	shotText_.isActive = isDisplay;
	jumpText_.isActive = isDisplay;

}

void GameUIManager::WaveSprite::Init(LWP::Math::Vector3 pos) {
	text.material.texture = LWP::Resource::LoadTexture("UI/Text/Wave.png");
	text.material.enableLighting = false;
	text.isUI = true;
	text.worldTF.translation = pos;
	text.worldTF.scale = { 0.3f,0.3f,0.3f };

	// 
	for (int i = 0; i < 10; i++) {
		number.n[i].worldTF.Parent(&text.worldTF);
		number.n[i].worldTF.translation = { 955.0f,-35.0f,0.0f };
		number.n[i].worldTF.scale = { 0.3f,0.3,0.3f };
		number.n[i].isActive = false;
		number.n[i].material.enableLighting = false;
		number.n[i].isUI = true;
	}
}
void GameUIManager::WaveSprite::Update(int wave) {
	// 初期化
	for (int i = 0; i < 10; i++) {
		number.n[i].isActive = false;
	}

	// 10は特殊処理
	if (wave >= 10) {
		number.n[1].isActive = true;
		number.n[0].isActive = true;
		number.n[0].worldTF.translation.x = 1165.0f;
	}
	// それ以外は通常処理
	else {
		number.n[wave].isActive = true;
	}
}

void GameUIManager::EnemySprite::Init(LWP::Math::Vector3 pos) {
	text.material.texture = LWP::Resource::LoadTexture("UI/Text/Enemy.png");
	text.material.enableLighting = false;
	text.isUI = true;
	text.worldTF.translation = pos;
	text.worldTF.scale = { 0.15f,0.15f,0.15f };

	// 
	for (int i = 0; i < 10; i++) {
		numFirst.n[i].worldTF.Parent(&text.worldTF);
		numFirst.n[i].worldTF.translation = { 1345.0f,25.0f,0.0f };
		numFirst.n[i].worldTF.scale = { 0.2f,0.2f,0.2f };
		numFirst.n[i].isActive = false;
		numFirst.n[i].material.enableLighting = false;
		numFirst.n[i].isUI = true;
		numSecond.n[i].worldTF.Parent(&text.worldTF);
		numSecond.n[i].worldTF.translation = { 1165.0f,25.0f,0.0f };
		numSecond.n[i].worldTF.scale = { 0.2f,0.2f,0.2f };
		numSecond.n[i].isActive = false;
		numSecond.n[i].material.enableLighting = false;
		numSecond.n[i].isUI = true;
	}
}
void GameUIManager::EnemySprite::Update(int enemy) {
	// 初期化
	for (int i = 0; i < 10; i++) {
		numFirst.n[i].isActive = false;
		numSecond.n[i].isActive = false;
	}

	int second = enemy / 10;
	int first = enemy % 10;

	numSecond.n[second].isActive = true;
	numFirst.n[first].isActive = true;
}
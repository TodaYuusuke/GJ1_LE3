#pragma once
#include <Adapter.h>

/// クラスの前方宣言
class Player;

/// <summary>
/// ゲームUIマネージャー
/// </summary>
class GameUIManager
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameUIManager() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameUIManager() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Initialize(Player* player);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DebugGUI();

	/// <summary>
	/// セットアップ関数
	/// </summary>
	void SetUp();

public: // アクセッサ等

	/// <summary>
	/// 表示トリガーゲッター
	/// </summary>
	/// <returns>表示トリガー</returns>
	bool GetIsDisplay() { return isDisplayObserver_.t; }
	/// <summary>
	/// 表示トリガーセッター
	/// </summary>
	/// <param name="isDisplay">表示するか</param>
	void SetIsDisplay(bool isDisplay) { isDisplayObserver_.t = isDisplay; }

private: // プライベートな関数

	/// <summary>
	/// デバッグ関数
	/// </summary>
	void Debug();

	/// <summary>
	/// HPゲージ更新関数
	/// </summary>
	void HPGaugeUpdate();

	/// <summary>
	/// 弾ゲージ更新関数
	/// </summary>
	void BulletGaugeUpdate();

	/// <summary>
	/// UIの表示、非表示の切り替え関数
	/// </summary>
	/// <param name="isDisplay">表示、非表示フラグ</param>
	void SwitchDisplayUI(bool isDisplay);

private: // メンバ変数

	// プレイヤー本体
	Player* player_ = nullptr;

	// 表示トリガー
	LWP::Utility::Observer<bool> isDisplayObserver_ = true;

	// UI系のセットアップフラグ
	bool isSetUpUI_ = false;

	/// スプライト群
	// 体力ゲージ
	std::vector<LWP::Primitive::Sprite> hpGaugeBG_; // 背景
	std::vector<LWP::Primitive::Sprite> hpGauge_;	// 本体
	// 弾用
	std::vector<LWP::Primitive::Sprite> BulletsUI_; // 弾ゲージ

	/// UI用変数群
	// 体力ゲージ変数群
	float hpGaugeOffsetX_ = 85.0f; // HPゲージのX軸オフセット
	LWP::Utility::Observer<int*> healthObserver_; // 体力ゲージ監視用
	// 弾ゲージ変数群
	LWP::Math::Vector2 bulletsAnchor_ = { 50.0f, 100.0f }; // 弾UIのアンカー
	float BulletGaugeOffsetX_ = 5.0f; // 弾ゲージのX軸オフセット
	LWP::Utility::Observer<int*> bulletObserver_; // 弾ゲージ監視用
};


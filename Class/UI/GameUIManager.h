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
	
private: // メンバ変数

	// プレイヤー本体
	Player* player_ = nullptr;

	// UI系のセットアップフラグ
	bool isSetUpUI_ = false;

	/// スプライト群
	// 体力ゲージ
	std::vector<LWP::Primitive::Sprite> hpGaugeBG_; // 背景
	std::vector<LWP::Primitive::Sprite> hpGauge_;	// 本体

	// 体力ゲージ変数群
	float hpGaugeOffsetX_ = 85.0f; // HPゲージのX軸オフセット
};


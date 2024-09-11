#pragma once
#include<Adapter.h>
#include"../IUpgrade/IUpgrade.h"
#include "UpGradeList.h"

/// <summary>
/// アップグレードマネージャー
/// </summary>
class UpgradeManager {
private: // サブクラス

	/// <summary>
	/// 単体のアップグレードデータ
	/// </summary>
	struct UpgradeData {
		LWP::Primitive::Sprite ui;		// UI用スプライト
		IUpgrade*			   upgrade; // アップグレード
	};

public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	UpgradeManager() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~UpgradeManager() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="drone">ドローン</param>
	void Initialize(Player* player, Drone* drone);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// デバッグ情報表示関数
	/// </summary>
	void DebugGUI();

private: // プライベートな関数

	/// <summary>
	/// 配列にアップグレードを追加する
	/// </summary>
	void AddUpgrades();

	/// <summary>
	/// 引数の対象スプライトをリセットする関数
	/// </summary>
	/// <param name="s">対象スプライト</param>
	void SpriteReset(LWP::Primitive::Sprite& s, const std::string FileName);

private: // メンバ変数

	// アップグレード格納配列
	std::map<std::string, UpgradeData> upgrades_;

	//プレイヤーポインタ
	Player* player_ = nullptr;

	//ドローンポインタ
	Drone* drone_ = nullptr;
};



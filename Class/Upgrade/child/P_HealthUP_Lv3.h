#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// 体力増加Lv3
/// </summary>
class HealthUPLv3 : public IUpgrade {
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init(const std::string& prevUpgradeName = "") override {
		// 名称設定
		name_ = "HealthUPLv3";
		// 前提アップグレード名称
		prevUpgradeName_ = prevUpgradeName;
	};

	/// <summary>
	/// 適用関数
	/// </summary>
	/// <param name="player_">プレイヤー本体</param>
	/// <param name="drone">ドローン本体</param>
	void Apply(Player* player_, Drone* drone)override {
		// プレイヤーの最大体力を増加
		player_->parameters_.maxHp += 1;
		// プレイヤーの現在HPも+1
		player_->parameters_.hp += 1;

		// 適用フラグをtrue
		isApplied_ = true;
	}

};
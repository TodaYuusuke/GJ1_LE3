#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// スライディング距離増加Lv3
/// </summary>
class SlindingUpGradeLv3 : public IUpgrade {
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init(const std::string& prevUpgradeName = "") override {
		// 名称設定
		name_ = "SlindingUpGradeLv3";
		// 前提アップグレード名称
		prevUpgradeName_ = prevUpgradeName;
	};

	/// <summary>
	/// 適用関数
	/// </summary>
	/// <param name="player_">プレイヤー本体</param>
	/// <param name="drone">ドローン本体</param>
	void Apply(Player* player_, Drone* drone)override {
		// 上昇率
		float percent = 20.0f;

		// スライディング距離加算
		player_->parameters_.slideData.length *= (100.0f + percent) / 100.0f;
		// 適用フラグをtrue
		isApplied_ = true;
	}

};
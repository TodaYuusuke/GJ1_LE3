#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// 弾の拡散増加Lv2
/// </summary>
class SpreadUPLv2 : public IUpgrade {
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init(const std::string& prevUpgradeName = "") override {
		// 名称設定
		name_ = "SpreadUPLv2";
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
		float percent = 5.0f;

		// 弾の拡散値を割合で増加
		player_->parameters_.bulletData.bulletDispersion_ *= (100.0f + percent) / 100.0f;
		// 適用フラグをtrue
		isApplied_ = true;
	}

};
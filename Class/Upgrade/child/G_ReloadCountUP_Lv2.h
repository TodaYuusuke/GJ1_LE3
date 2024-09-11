#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// 一度のリロードで込める弾数増加Lv2
/// </summary>
class ReloadCountUpLv2 : public IUpgrade {

	/// <summary>
	/// 適用関数
	/// </summary>
	/// <param name="player_">プレイヤー本体</param>
	/// <param name="drone">ドローン本体</param>
	void Apply(Player* player_, Drone* drone)override {
		// リロード数増加
		player_->parameters_.bulletData.simultaneouslyLoaded_ += 1;
		// 適用フラグをtrue
		isApplied_ = true;
	}

};
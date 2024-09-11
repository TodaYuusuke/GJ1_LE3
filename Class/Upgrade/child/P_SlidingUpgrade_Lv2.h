#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// スライディング距離増加Lv2
/// </summary>
class SlindingUpGradeLv2 : public IUpgrade {

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
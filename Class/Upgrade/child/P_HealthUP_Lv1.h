#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// 体力増加Lv1
/// </summary>
class HealthUPLv1 : public IUpgrade {

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
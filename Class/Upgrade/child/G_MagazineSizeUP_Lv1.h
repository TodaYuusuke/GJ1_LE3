#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
///　装弾数増加Lv1
/// </summary>
class MagazineSizeUPLv1 : public IUpgrade {

	/// <summary>
	/// 適用関数
	/// </summary>
	/// <param name="player_">プレイヤー本体</param>
	/// <param name="drone">ドローン本体</param>
	void Apply(Player* player_, Drone* drone)override { 
		// リロード数増加
		player_->parameters_.bulletData.maxAmmoNum_ += 1; 
		// 適用フラグをtrue
		isApplied_ = true;
	}

};
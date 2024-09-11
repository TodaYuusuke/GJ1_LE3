#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// リロード開始時間短縮Lv1
/// </summary>
class ReloadStartSpeedUPLv1 : public IUpgrade {

	/// <summary>
	/// 適用関数
	/// </summary>
	/// <param name="player_">プレイヤー本体</param>
	/// <param name="drone">ドローン本体</param>
	void Apply(Player* player_, Drone* drone)override {
		// 上昇率
		float percent = 5.0f;

		// リロード開始時間短縮
		player_->parameters_.bulletData.reloadStartSec_ *= (100.0f - percent) / 100.0f;
		// 適用フラグをtrue
		isApplied_ = true;
	}

};
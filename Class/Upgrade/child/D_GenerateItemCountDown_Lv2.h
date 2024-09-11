#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// アイテム生成までの敵数減少Lv2
/// </summary>
class GenerateItemCountDownLv2 : public IUpgrade {

	/// <summary>
	/// 適用関数
	/// </summary>
	/// <param name="player_">プレイヤー本体</param>
	/// <param name="drone">ドローン本体</param>
	void Apply(Player* player_, Drone* drone)override {

		// 適用フラグをtrue
		isApplied_ = true;
	}

};
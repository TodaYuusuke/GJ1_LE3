#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// 弾の拡散増加Lv1
/// </summary>
class SpreadUPLv1 : public IUpgrade {

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
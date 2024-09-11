#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// eÌgUÁLv1
/// </summary>
class SpreadUPLv1 : public IUpgrade {

	/// <summary>
	/// KpÖ
	/// </summary>
	/// <param name="player_">vC[{Ì</param>
	/// <param name="drone">h[{Ì</param>
	void Apply(Player* player_, Drone* drone)override { 
		// ã¸¦
		float percent = 5.0f;

		// eÌgUlðÅÁ
		player_->parameters_.bulletData.bulletDispersion_ *= (100.0f + percent) / 100.0f;
		// KptOðtrue
		isApplied_ = true;
	}

};
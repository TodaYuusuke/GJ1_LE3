#pragma once
#include"../IUpgrade/IUpgrade.h"

// <summary>
/// リロード開始時間短縮Lv3
/// </summary>
class ReloadStartSpeedUPLv3 : public IUpgrade {
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override {
		// 名称設定
		name_ = "ReloadStartSpeedUPLv3";
	};

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
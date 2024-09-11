#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// ドローン回収速度短縮Lv1
/// </summary>
class CorrectSpeedUPLv1 : public IUpgrade {
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override {
		// 名称設定
		name_ = "CorrectSpeedUPLv1";
	};

	/// <summary>
	/// 適用関数
	/// </summary>
	/// <param name="player_">プレイヤー本体</param>
	/// <param name="drone">ドローン本体</param>
	void Apply(Player* player_, Drone* drone) override { 
		// プレイヤーのジャンプを有効にする
		player_->parameters_.activeFlag.jump = true; 
		// 適用フラグをtrue
		isApplied_ = true;
	}
};
#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// アイテム生成までの敵数減少Lv3
/// </summary>
class GenerateItemCountDownLv3 : public IUpgrade {
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init(const std::string& prevUpgradeName = "") override {
		// 名称設定
		name_ = "GenerateItemCountDownLv3";
		// 前提アップグレード名称
		prevUpgradeName_ = prevUpgradeName;
	};

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
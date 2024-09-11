#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// 切り返し有効化アップグレード
/// </summary>
class ActiveCutBack : public IUpgrade {
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override {
		// 名称設定
		name_ = "ActiveCutBack";
	};

	/// <summary>
	/// 適用関数
	/// </summary>
	/// <param name="player_">プレイヤー本体</param>
	/// <param name="drone">ドローン本体</param>
	void Apply(Player* player_, Drone* drone)override { 
		// プレイヤーの切り返しを有効化
		player_->parameters_.activeFlag.slidingStopShot = true; 
		// 適用フラグをtrue
		isApplied_ = true;
	}

};
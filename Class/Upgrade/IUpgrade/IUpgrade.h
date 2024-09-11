#pragma once
#include"../../Player/Player.h"
#include"../../Drone/Drone.h"

class IUpgrade {

public:

	IUpgrade() = default;
	~IUpgrade() = default;

	//初期化処理（何かるなら用
	virtual void Initialize()=0;
	
	/// <summary>
	/// プレイヤーに実行処理
	/// </summary>
	/// <param name="player_">プレイヤーポインタ</param>
	/// <param name="drone">ドローンポインタ</param>
	virtual void Apply(Player* player_,Drone*drone) = 0;
public:
	//使用フラグ
	bool isUsed_ = false;
};
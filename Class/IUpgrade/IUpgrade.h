#pragma once
#include"../Player/Player.h"

class IUpgrade {

public:

	IUpgrade() = default;
	~IUpgrade() = default;

	//初期化処理（何かるなら用
	virtual void Initialize()=0;

	//プレイヤーに実行処理
	virtual void Apply(Player* player_) = 0;
public:
	//使用フラグ
	bool isUsed_ = false;
};
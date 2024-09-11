#pragma once
#include"../../Player/Player.h"
#include"../../Drone/Drone.h"

/// <summary>
/// アップグレード基底クラス
/// </summary>
class IUpgrade {

public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IUpgrade() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~IUpgrade() = default;

public: // 純粋仮想関数
	
	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 適用関数
	/// </summary>
	/// <param name="player_">プレイヤー本体</param>
	/// <param name="drone">ドローン本体</param>
	virtual void Apply(Player* player_,Drone*drone) = 0;

public:

	// アップグレード名称
	std::string name_ = "";

	//使用フラグ
	bool isApplied_ = false;

};
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
	/// <param name="prevUpgradeName">前提アップグレード名</param>
	virtual void Init(const std::string& prevUpgradeName = "") = 0;

	/// <summary>
	/// 適用関数
	/// </summary>
	/// <param name="player">プレイヤー本体</param>
	/// <param name="drone">ドローン本体</param>
	virtual void Apply(Player* player,Drone*drone) = 0;

	/// <summary>
	/// デバッグ用のGUIを表示する関数
	/// </summary>
	/// <param name="player">プレイヤー本体</param>
	/// <param name="drone">ドローン本体</param>
	void DebugGUI(Player* player, Drone* drone) {
		// 適用済みフラグ確認用チェックボックス
		ImGui::Checkbox("isApplied", &isApplied_);

		// 適用済みでない場合のみ
		if (!isApplied_) {
			// ボタンを押すと適用関数の呼び出し
			if (ImGui::Button("Apply")) {
				Apply(player, drone);
			}
		}
	}

public:

	// アップグレード名称
	std::string name_ = "";
	// 前提アップグレード名称
	std::string prevUpgradeName_ = "";

	//使用フラグ
	bool isApplied_ = false;

};
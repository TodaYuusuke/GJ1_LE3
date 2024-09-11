#pragma once
#include<Adapter.h>
#include"../IUpgrade/IUpgrade.h"

class UpgradeManager {

public:
	UpgradeManager(Player*player);
	~UpgradeManager();

	void Initialize();

	void DebugGUI();

	//アップグレード追加
	void AddUpgrade(const std::string& upgradeName, IUpgrade* upgrade);

	//アップグレードを使用
	void ApplyUpgrade(const std::string& upgradeName);
private:

	//
	std::map<std::string, IUpgrade*>upgradeDatas_;

	//プレイヤーポインタ
	Player* player_;

	//ドローンポインタ
	Drone* drone_;
};



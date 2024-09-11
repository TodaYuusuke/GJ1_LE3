#pragma once
#include<Adapter.h>
#include"../IUpgrade/IUpgrade.h"

class UpgradeManager {

public:
	UpgradeManager(Player*player);
	~UpgradeManager();

	void Initialize();

	void DebugGUI();

	//�A�b�v�O���[�h�ǉ�
	void AddUpgrade(const std::string& upgradeName, IUpgrade* upgrade);

	//�A�b�v�O���[�h���g�p
	void ApplyUpgrade(const std::string& upgradeName);
private:

	//
	std::map<std::string, IUpgrade*>upgradeDatas_;

	//�v���C���[�|�C���^
	Player* player_;

	//�h���[���|�C���^
	Drone* drone_;
};



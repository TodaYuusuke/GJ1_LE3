#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// �̗͑���Lv1
/// </summary>
class HealthUPLv1 : public IUpgrade {

	/// <summary>
	/// �K�p�֐�
	/// </summary>
	/// <param name="player_">�v���C���[�{��</param>
	/// <param name="drone">�h���[���{��</param>
	void Apply(Player* player_, Drone* drone)override { 
		// �v���C���[�̍ő�̗͂𑝉�
		player_->parameters_.maxHp += 1;
		// �v���C���[�̌���HP��+1
		player_->parameters_.hp += 1;

		// �K�p�t���O��true
		isApplied_ = true;
	}

};
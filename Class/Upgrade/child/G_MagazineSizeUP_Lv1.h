#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
///�@���e������Lv1
/// </summary>
class MagazineSizeUPLv1 : public IUpgrade {

	/// <summary>
	/// �K�p�֐�
	/// </summary>
	/// <param name="player_">�v���C���[�{��</param>
	/// <param name="drone">�h���[���{��</param>
	void Apply(Player* player_, Drone* drone)override { 
		// �����[�h������
		player_->parameters_.bulletData.maxAmmoNum_ += 1; 
		// �K�p�t���O��true
		isApplied_ = true;
	}

};
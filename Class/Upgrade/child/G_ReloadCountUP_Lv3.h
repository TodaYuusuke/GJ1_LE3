#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// ��x�̃����[�h�ō��߂�e������Lv3
/// </summary>
class ReloadCountUpLv3 : public IUpgrade {

	/// <summary>
	/// �K�p�֐�
	/// </summary>
	/// <param name="player_">�v���C���[�{��</param>
	/// <param name="drone">�h���[���{��</param>
	void Apply(Player* player_, Drone* drone)override {
		// �����[�h������
		player_->parameters_.bulletData.simultaneouslyLoaded_ += 1;
		// �K�p�t���O��true
		isApplied_ = true;
	}

};
#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// �h���[��������x�Z�kLv3
/// </summary>
class CorrectSpeedUPLv3 : public IUpgrade {

	/// <summary>
	/// �K�p�֐�
	/// </summary>
	/// <param name="player_">�v���C���[�{��</param>
	/// <param name="drone">�h���[���{��</param>
	void Apply(Player* player_, Drone* drone)override {
		// �v���C���[�̃W�����v��L���ɂ���
		player_->parameters_.activeFlag.jump = true;
		// �K�p�t���O��true
		isApplied_ = true;
	}

};
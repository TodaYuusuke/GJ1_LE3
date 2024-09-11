#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// �e�̊g�U����Lv1
/// </summary>
class SpreadUPLv1 : public IUpgrade {

	/// <summary>
	/// �K�p�֐�
	/// </summary>
	/// <param name="player_">�v���C���[�{��</param>
	/// <param name="drone">�h���[���{��</param>
	void Apply(Player* player_, Drone* drone)override { 
		// �㏸��
		float percent = 5.0f;

		// �e�̊g�U�l�������ő���
		player_->parameters_.bulletData.bulletDispersion_ *= (100.0f + percent) / 100.0f;
		// �K�p�t���O��true
		isApplied_ = true;
	}

};
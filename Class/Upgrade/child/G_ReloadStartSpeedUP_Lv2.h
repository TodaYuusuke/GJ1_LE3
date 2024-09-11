#pragma once
#include"../IUpgrade/IUpgrade.h"

// <summary>
/// �����[�h�J�n���ԒZ�kLv2
/// </summary>
class ReloadStartSpeedUPLv2 : public IUpgrade {

	/// <summary>
	/// �K�p�֐�
	/// </summary>
	/// <param name="player_">�v���C���[�{��</param>
	/// <param name="drone">�h���[���{��</param>
	void Apply(Player* player_, Drone* drone)override {
		// �㏸��
		float percent = 5.0f;

		// �����[�h�J�n���ԒZ�k
		player_->parameters_.bulletData.reloadStartSec_ *= (100.0f - percent) / 100.0f;
		// �K�p�t���O��true
		isApplied_ = true;
	}

};
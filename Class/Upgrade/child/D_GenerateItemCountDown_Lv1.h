#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// �A�C�e�������܂ł̓G������Lv1
/// </summary>
class GenerateItemCountDownLv1 : public IUpgrade {

	/// <summary>
	/// �K�p�֐�
	/// </summary>
	/// <param name="player_">�v���C���[�{��</param>
	/// <param name="drone">�h���[���{��</param>
	void Apply(Player* player_, Drone* drone)override { 

		// �K�p�t���O��true
		isApplied_ = true;
	}

};
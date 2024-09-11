#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
///�@���e������Lv2
/// </summary>
class MagazineSizeUPLv2 : public IUpgrade {
public: // �����o�֐�

	/// <summary>
	/// �������֐�
	/// </summary>
	void Init() override {
		// ���̐ݒ�
		name_ = "MagazineSizeUPLv2";
	};

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
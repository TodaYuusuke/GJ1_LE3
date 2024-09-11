#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// �h���[��������x�Z�kLv2
/// </summary>
class CorrectSpeedUPLv2 : public IUpgrade {
public: // �����o�֐�

	/// <summary>
	/// �������֐�
	/// </summary>
	void Init() override {
		// ���̐ݒ�
		name_ = "CorrectSpeedUPLv2";
	};

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
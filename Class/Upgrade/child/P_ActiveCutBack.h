#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// �؂�Ԃ��L�����A�b�v�O���[�h
/// </summary>
class ActiveCutBack : public IUpgrade {
public: // �����o�֐�

	/// <summary>
	/// �������֐�
	/// </summary>
	void Init() override {
		// ���̐ݒ�
		name_ = "ActiveCutBack";
	};

	/// <summary>
	/// �K�p�֐�
	/// </summary>
	/// <param name="player_">�v���C���[�{��</param>
	/// <param name="drone">�h���[���{��</param>
	void Apply(Player* player_, Drone* drone)override { 
		// �v���C���[�̐؂�Ԃ���L����
		player_->parameters_.activeFlag.slidingStopShot = true; 
		// �K�p�t���O��true
		isApplied_ = true;
	}

};
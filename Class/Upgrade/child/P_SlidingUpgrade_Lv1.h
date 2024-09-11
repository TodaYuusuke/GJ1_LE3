#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// �X���C�f�B���O��������Lv1
/// </summary>
class SlindingUpGradeLv1 : public IUpgrade {
public: // �����o�֐�

	/// <summary>
	/// �������֐�
	/// </summary>
	void Init() override {
		// ���̐ݒ�
		name_ = "SlindingUpGradeLv1";
	};

	/// <summary>
	/// �K�p�֐�
	/// </summary>
	/// <param name="player_">�v���C���[�{��</param>
	/// <param name="drone">�h���[���{��</param>
	void Apply(Player* player_, Drone* drone)override { 
		// �㏸��
		float percent = 20.0f;
		
		// �X���C�f�B���O�������Z
		player_->parameters_.slideData.length *= (100.0f + percent) / 100.0f;
		// �K�p�t���O��true
		isApplied_ = true;
	}

};
#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// �e�̊g�U����Lv3
/// </summary>
class SpreadUPLv3 : public IUpgrade {
public: // �����o�֐�

	/// <summary>
	/// �������֐�
	/// </summary>
	void Init() override {
		// ���̐ݒ�
		name_ = "SpreadUPLv3";
	};

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
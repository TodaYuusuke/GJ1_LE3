#pragma once
#include"../IUpgrade/IUpgrade.h"

/// <summary>
/// �A�C�e�������܂ł̓G������Lv2
/// </summary>
class GenerateItemCountDownLv2 : public IUpgrade {
public: // �����o�֐�

	/// <summary>
	/// �������֐�
	/// </summary>
	void Init() override {
		// ���̐ݒ�
		name_ = "GenerateItemCountDownLv2";
	};

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
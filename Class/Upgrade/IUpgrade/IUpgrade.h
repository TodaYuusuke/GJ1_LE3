#pragma once
#include"../../Player/Player.h"
#include"../../Drone/Drone.h"

/// <summary>
/// �A�b�v�O���[�h���N���X
/// </summary>
class IUpgrade {

public: // �R���X�g���N�^��

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	IUpgrade() = default;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~IUpgrade() = default;

public: // �������z�֐�
	
	/// <summary>
	/// �K�p�֐�
	/// </summary>
	/// <param name="player_">�v���C���[�{��</param>
	/// <param name="drone">�h���[���{��</param>
	virtual void Apply(Player* player_,Drone*drone) = 0;

public:

	//�g�p�t���O
	bool isApplied_ = false;

};
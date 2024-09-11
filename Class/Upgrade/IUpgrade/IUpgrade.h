#pragma once
#include"../../Player/Player.h"
#include"../../Drone/Drone.h"

class IUpgrade {

public:

	IUpgrade() = default;
	~IUpgrade() = default;

	//�����������i������Ȃ�p
	virtual void Initialize()=0;
	
	/// <summary>
	/// �v���C���[�Ɏ��s����
	/// </summary>
	/// <param name="player_">�v���C���[�|�C���^</param>
	/// <param name="drone">�h���[���|�C���^</param>
	virtual void Apply(Player* player_,Drone*drone) = 0;
public:
	//�g�p�t���O
	bool isUsed_ = false;
};
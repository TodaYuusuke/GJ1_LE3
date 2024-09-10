#pragma once
#include"../Player/Player.h"

class IUpgrade {

public:

	IUpgrade() = default;
	~IUpgrade() = default;

	//�����������i������Ȃ�p
	virtual void Initialize()=0;

	//�v���C���[�Ɏ��s����
	virtual void Apply(Player* player_) = 0;
public:
	//�g�p�t���O
	bool isUsed_ = false;
};
#pragma once
#include<Adapter.h>
#include"../IUpgrade/IUpgrade.h"
#include "UpGradeList.h"

/// <summary>
/// �A�b�v�O���[�h�}�l�[�W���[
/// </summary>
class UpgradeManager {
private: // �T�u�N���X

	/// <summary>
	/// �P�̂̃A�b�v�O���[�h�f�[�^
	/// </summary>
	struct UpgradeData {
		LWP::Primitive::Sprite ui;		// UI�p�X�v���C�g
		IUpgrade*			   upgrade; // �A�b�v�O���[�h
	};

public: // �R���X�g���N�^��

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UpgradeManager() = default;
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~UpgradeManager() = default;

public: // �����o�֐�

	/// <summary>
	/// �������֐�
	/// </summary>
	/// <param name="player">�v���C���[</param>
	/// <param name="drone">�h���[��</param>
	void Initialize(Player* player, Drone* drone);

	/// <summary>
	/// �X�V�֐�
	/// </summary>
	void Update();

	/// <summary>
	/// �f�o�b�O���\���֐�
	/// </summary>
	void DebugGUI();

private: // �v���C�x�[�g�Ȋ֐�

	/// <summary>
	/// �z��ɃA�b�v�O���[�h��ǉ�����
	/// </summary>
	void AddUpgrades();

	/// <summary>
	/// �����̑ΏۃX�v���C�g�����Z�b�g����֐�
	/// </summary>
	/// <param name="s">�ΏۃX�v���C�g</param>
	void SpriteReset(LWP::Primitive::Sprite& s, const std::string FileName);

private: // �����o�ϐ�

	// �A�b�v�O���[�h�i�[�z��
	std::map<std::string, UpgradeData> upgrades_;

	//�v���C���[�|�C���^
	Player* player_ = nullptr;

	//�h���[���|�C���^
	Drone* drone_ = nullptr;
};



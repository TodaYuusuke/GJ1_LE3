#include "UpgradeManager.h"

void UpgradeManager::Initialize(Player* player, Drone* drone)
{
	// �|�C���^���擾����
	player_ = player; // �v���C���[
	drone_	= drone;  // �h���[��
}

void UpgradeManager::Update()
{

}

void UpgradeManager::DebugGUI()
{
#ifdef DEMO

	// ImGui�J�n
	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("UpgradeManager")) {



			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	// ImGui�I��
	ImGui::End();

#endif // DEMO

}

void UpgradeManager::AddUpgrades()
{
	/// �v���C���[�n�̃A�b�v�O���[�h�ǉ�
	// �W�����v�L�����A�b�v�O���[�h
	UpgradeData data0;
	data0.upgrade = new ActiveCutBack(); // �V�K����
	data0.upgrade->Init();				 // ������
	data0.ui.isUI = 

}
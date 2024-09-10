#include "SkillTree.h"

SkillTree::SkillTree()
{
	//データ取得処理
}

SkillTree::~SkillTree()
{
}

using namespace LWP::Math;

float calculate_x(Vector3 normalized_vector, float target_y) {
    float x_component = normalized_vector.x;
    float y_component = normalized_vector.y;

    // y_component が 0 の場合は計算できないためエラーチェック
    if (y_component == 0) {
        //printf("Error: The Y component of the normalized vector cannot be zero.\n");
        
        return 0.0;
    }

    // スケーリングファクターを計算
    double scale_factor = target_y / y_component;

    // X座標を計算
    double new_x = x_component * scale_factor;

    return new_x;
}


void SkillTree::Initialize()
{
	//初期座標設定

	for (int i = 0; i < _countBaseSkill; i++) {
       float x= -10 + (10 * i);
       datas_[i].model.LoadCube();
       datas_[i].model.worldTF.translation.x=x;
       datas_[i].model.worldTF.Parent(&baseposition_);
	}
}

void SkillTree::Update()
{
#ifdef DEMO
	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("SkillTree")) {
            ImGui::DragFloat3("base postion", &baseposition_.translation.x, 0.01f);
            
			ImGui::EndTabItem();

		}

		ImGui::EndTabBar();
	}
	ImGui::End();
#endif // DEMO



}

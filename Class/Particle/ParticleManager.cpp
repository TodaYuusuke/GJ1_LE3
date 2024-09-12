#include "ParticleManager.h"

using namespace LWP::Object::Collider;

void ParticleManager::Init() {
	// フラグ設定
	for (int i = 0; i < 3; i++) {
		ground_[i].mask.SetBelongFrag(GJMask::Ground());
		ground_[i].mask.SetHitFrag(GJMask::Particle());
		ground_[i].name = "Ground";
	};
	// プレイヤーの歩く部分
	AABB& g0 = ground_[0].SetBroadShape(AABB());
	g0.min = { -250.0f, -0.5f, -1.75f };
	g0.max = { 250.0f, 0.0f, 1.75f };
	// 奥の道1
	AABB& g1 = ground_[1].SetBroadShape(AABB());
	g1.min = { 33.61f, -0.5f, 1.75f };
	g1.max = { 33.61f + 3.5f, 0.0f, 10.0f };
	// 奥の道2
	ground_[2].worldTF.translation.x = 10.01f;
	AABB& g2 = ground_[2].SetBroadShape(AABB());
	g2.min = g1.min;
	g2.max = g1.max;
	// 奥の道3
	ground_[3].worldTF.translation.x = 69.99f;
	AABB& g3 = ground_[3].SetBroadShape(AABB());
	g3.min = g1.min;
	g3.max = g1.max;

	// 水面のコライダー
	waterSurface_.mask.SetBelongFrag(GJMask::WaterSurface());
	waterSurface_.mask.SetHitFrag(GJMask::Particle());
	waterSurface_.name = "WaterSurface";
	AABB& w = waterSurface_.SetBroadShape(AABB());
	w.min = { -250.0f, -1.0f, -25.0f };
	w.max = { 250.0f, -0.5f, 25.0f };

	// スケール変更用
	float s;

	ceilingFragment_.model.LoadShortPath("Debris/Debris.gltf");
	s = 0.05f;
	ceilingFragment_.model.worldTF.scale = { s,s,s };
}
void ParticleManager::DebugGUI() {

#if DEMO
	// ** ImGui用変数 ** //
	static int selectedClass = 0;	// 生成クラス
	static int currentItem = 0;	// 選ばれている番号
	// 名前もセット
	static std::vector<const char*> classText{
		"Spider", "Slime"
	};

	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("Particle")) {

			if (ImGui::TreeNode("CeilingFragment")) {
				if (ImGui::Button("Generate")) { ceilingFragment_.Add(ceilingFragmentNum_); }
				ImGui::InputInt("Num", &ceilingFragmentNum_);
				ceilingFragment_.DebugGUI();
				ImGui::TreePop();
			}

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
#endif
}


void ParticleManager::Ceiling(LWP::Math::Vector3 pos) {
	ceilingFragment_.Add(ceilingFragmentNum_, pos);
}


#include "ParticleManager.h"

using namespace LWP::Math;
using namespace LWP::Object::Collider;

void ParticleManager::Init() {
	// スケール変更用
	float s;

	ceilingFragment_.model.LoadShortPath("Debris/Debris.gltf");
	s = 0.05f;
	ceilingFragment_.model.worldTF.scale = { s,s,s };

#pragma region 当たり判定生成
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

	// 天井のコライダー
	ceiling_.worldTF.translation.y = 7.28f;
	ceiling_.mask.SetBelongFrag(GJMask::Ceiling());
	ceiling_.mask.SetHitFrag(GJMask::Bullet());
	ceiling_.enterLambda = [this](Collider* col) {
		Capsule& c = std::get<Capsule>(col->broad);
		Vector3 start = c.start + col->GetWorldPosition();
		Vector3 end = c.end + col->GetWorldPosition();

		// 天井の座標を求める
		// 2点間のY成分が同じ場合、Y=7の地点がない可能性がある
		if (start.y == end.y) {
			//return std::nullopt;
		}

		// パラメータtを計算
		float t = (7.0f - start.y) / (end.y - start.y);

		// tが0から1の範囲外なら、Y=7の地点は2点の間に存在しない
		if (t < 0.0f || t > 1.0f) {
			//return std::nullopt;
		}

		// tを使ってX, Z座標を計算
		Vector3 result;
		result.x = start.x + t * (end.x - start.x);
		result.y = 7.0f;  // Y座標は7
		result.z = start.z + t * (end.z - start.z);

		Ceiling(result);
	};
	ceiling_.name = "Ceiling";
	AABB& c = ceiling_.SetBroadShape(AABB());
	c.min = g0.min;
	c.max = g0.max;

	// 水面のコライダー
	waterSurface_.mask.SetBelongFrag(GJMask::WaterSurface());
	waterSurface_.mask.SetHitFrag(GJMask::Particle());
	waterSurface_.name = "WaterSurface";
	AABB& w = waterSurface_.SetBroadShape(AABB());
	w.min = { -250.0f, -1.0f, -25.0f };
	w.max = { 250.0f, -0.5f, 25.0f };
#pragma endregion
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


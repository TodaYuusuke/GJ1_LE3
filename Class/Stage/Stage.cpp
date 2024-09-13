#include "Stage.h"

using namespace LWP;
using namespace LWP::Math;
using namespace LWP::Object;

void Stage::Init(LWP::Object::Camera* camera) {
	// ポストプロセス
	camera->pp.use = true;
	camera->pp.bloom.use = true;
	camera->pp.vignetting.use = true;
	camera->pp.vignetting.intensity = 0.1f;
	camera->pp.CreateShaderFile();

	// 背景
	backGround_.worldTF.translation.z = 1000.0f;
	backGround_.worldTF.scale = { 3000.0f,3000.0f,1.0f };
	backGround_.worldTF.Parent(&camera->transform);
	backGround_.material.enableLighting = false;
	backGround_.material.color = Utility::ColorPattern::BLACK;

	// 下水道
	stage_.LoadShortPath("Stage/Stage.gltf");
	stage_.worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY(), 1.57f);
	stage_.materials["LumpMaterial"].enableLighting = false;
	// 水
	water_.worldTF.translation.y = -0.5f;
	water_.worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitX(), 1.57f);
	water_.worldTF.scale = { 720.0f,720.0f,1.0f };
	water_.material.uvTransform.scale = { 720.0f,720.0f,1.0f };
	water_.material.texture = LWP::Resource::LoadTexture("UI/WaterSurface.png");
	water_.name = "Water";

	// 太陽
	sun_.rotation.x = 3.14f;	// 下から照らす
	sun_.intensity = 1.8f;	// 色決定
	sun_.color = { 36,42,52,255 };	// 色決定
	
	// ドアライト
	doorLight_[0].transform.translation = { -34.39f, 2.56f, 7.88f };
	doorLight_[1].transform.translation = { 35.55f, 2.56f, 7.88f };
	for (int i = 0; i < 2; i++) {
		doorLight_[i].color = { 255,0,0,255 };
		doorLight_[i].radius = 1.19f;
	}
}

void Stage::Update() {
	// radian更新
	radian_ += LWP::Info::GetDeltaTimeF();
	if (radian_ > 6.28f) {
		radian_ -= 6.28f;
	}

	// 水面のアニメーション
	water_.material.uvTransform.translation.x += LWP::Info::GetDeltaTimeF() * 0.6f;
	water_.material.uvTransform.translation.y = std::sinf(radian_) * 0.1f;
}

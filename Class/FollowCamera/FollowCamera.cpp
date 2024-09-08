#include "FollowCamera.h"

FollowCamera::FollowCamera()
{
}

FollowCamera::~FollowCamera()
{
}

void FollowCamera::Initialize(LWP::Object::Camera* cam)
{
	camera_ = cam;
}

void FollowCamera::Update(const LWP::Math::Vector3& playerPos)
{
#ifdef DEMO
	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("FollowCam")) {

			ImGui::DragFloat3("coordiname shift", &coordinateShift_.x, 0.1f);

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
#endif // DEMO


	//カメラのxyをプレイヤーに合わせる
	camera_->transform.translation.x = coordinateShift_.x + playerPos.x;
	camera_->transform.translation.y = coordinateShift_.y + playerPos.y;

}

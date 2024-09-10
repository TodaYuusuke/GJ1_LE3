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
	camera_->transform.translation.y = 2.5f;
	camera_->transform.translation.z = -22.0f;
}

void FollowCamera::Update(const LWP::Math::Vector3& playerPos)
{
#ifdef DEMO
	ImGui::Begin("Game");
	if (ImGui::BeginTabBar("LWP")) {
		if (ImGui::BeginTabItem("FollowCam")) {

			ImGui::DragFloat3("coordiname shift", &coordinateShift_.x, 0.1f);
			ImGui::DragFloat3("start follow delay", &delayCameraPos_.x, 0.1f);

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
#endif // DEMO
	LWP::Math::Vector3 pos{0,0,0};

	pos = camera_->transform.translation;

	//ちょっとだけカメラを遅らせる処理
	if (camera_->transform.translation.x < -delayCameraPos_.x + playerPos.x) {
		pos.x = -delayCameraPos_.x + playerPos.x;
	}
	else if (camera_->transform.translation.x > delayCameraPos_.x+playerPos.x) {
		pos.x = delayCameraPos_.x + playerPos.x;
	}
	if (camera_->transform.translation.y < -delayCameraPos_.y + playerPos.y) {
		//pos.y = -delayCameraPos_.y + playerPos.y;
	}
	else if (camera_->transform.translation.y > delayCameraPos_.x + playerPos.y) {
		//pos.y = delayCameraPos_.y+playerPos.y;
	}

	//ずれ修正処理がなかった時にだんだんプレイヤー位置に修正
	//if (camera_->transform.translation == pos) {
	//	pos = LWP::Math::Vector3::Lerp(saveCpos_, playerPos, setPT_);
	//	setPT_ += addTsec_ / 60;
	//	if (setPT_ > 1.0f) {
	//		setPT_ = 1.0f;
	//	}
	//}
	//else {
	//	setPT_ = 0;
	//	saveCpos_ = camera_->transform.translation;
	//	saveCpos_.z = 0;
	//}

	//カメラのxyをプレイヤーに合わせる
	camera_->transform.translation.x = coordinateShift_.x + pos.x;
	//camera_->transform.translation.y = coordinateShift_.y + pos.y;

}

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


	//カメラのxyをプレイヤーに合わせる
	camera_->transform.translation.x = playerPos.x;
	camera_->transform.translation.y = playerPos.y;

}

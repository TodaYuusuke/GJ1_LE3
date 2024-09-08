#pragma once
#include<Adapter.h>

class FollowCamera {

public:
	FollowCamera();
	~FollowCamera();

	void Initialize(LWP::Object::Camera*cam);

	void Update(const LWP::Math::Vector3& playerPos);

private://**変数**//
	
	//カメラ
	LWP::Object::Camera* camera_;
};
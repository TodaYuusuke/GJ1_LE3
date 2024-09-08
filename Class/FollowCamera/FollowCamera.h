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

	//座標ずれ
	LWP::Math::Vector3 coordinateShift_;

	//カメラが追従開始するPとのずれ
	LWP::Math::Vector3 delayCameraPos_;

	float setPT_ = 0;
	float addTsec_ = 1;
	LWP::Math::Vector3 saveCpos_;
};
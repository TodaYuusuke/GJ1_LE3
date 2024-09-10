#pragma once
#include <Adapter.h>

struct BulletData {
	LWP::Resource::RigidModel model;
	LWP::Object::Collider::Collider collider;
	LWP::Object::Collider::Capsule* cap;
	//1秒間に進む量
	LWP::Math::Vector3 velo;

	//一応時間制限で死亡
	float deadCount=0;
	float maxDeadCount = 5;

	bool isAlive = false;
};

class PlayerBullets {

public:
	PlayerBullets();
	~PlayerBullets();

	//データの初期化処理
	void Initialize();

	void Update();

	/// <summary>
	/// 弾のデータを作成
	/// </summary>
	/// <param name="pos">出現座標</param>
	/// <param name="velo">移動量/s</param>
	void SetData(const LWP::Math::Vector3& pos, const LWP::Math::Vector3&velo, const std::string& cName);



	//弾丸のデータを読む
	std::vector<BulletData*>&GetBulletData() { return bullets_; };
private:

	//弾のデータ群
	std::vector<BulletData*>bullets_;
	//使いまわしの数
	int maxBullet_=10;

	//弾のmodel名
	std::string modelname_ = "";

	//弾のコライダー名
	std::string bulletName_ = "playerBullet";
};
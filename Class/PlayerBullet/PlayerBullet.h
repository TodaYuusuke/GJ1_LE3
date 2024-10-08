#pragma once
#include <Adapter.h>
#include "../Particle/ParticleManager.h"

struct BulletData {
	LWP::Resource::RigidModel model;
	LWP::Object::Collider::Collider collider;
	//1秒間に進む量
	LWP::Math::Vector3 velo;

	//一応時間制限で死亡
	float deadCount=0;
	float maxDeadCount = 7.5f;

	bool isAlive = true;
};

class PlayerBullets {

public:
	PlayerBullets();
	~PlayerBullets();

	//データの初期化処理
	void Initialize(ParticleManager* particle);

	void Update();

	/// <summary>
	/// 弾のデータを作成
	/// </summary>
	/// <param name="pos">出現座標</param>
	/// <param name="velo">移動量/s</param>
	void SetData(const LWP::Math::Vector3& pos, const LWP::Math::Vector3&velo, const std::string& cName);
private:

	//弾のデータ群
	std::list<BulletData>bullets_;

	//弾のmodel名
	std::string modelname_ = "";

	// パーティクル管理クラス
	ParticleManager* particle_ = nullptr;

};
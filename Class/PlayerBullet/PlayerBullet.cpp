#include "PlayerBullet.h"

using namespace LWP;
using namespace LWP::Object;

PlayerBullets::PlayerBullets()
{
	//同次にどんだけ出るか知らん

	for (int i = 0; i < maxBullet_; i++) {
		//仮で円を読み込み
		BulletData* data=new BulletData();
		data->model.LoadSphere();
		data->collider.SetFollowTarget(&data->model.worldTF);
		data->cap = &data->collider.SetBroadShape(Collider::Capsule());
		data->collider.name = bulletName_;
		data->isAlive = false;
		bullets_.emplace_back(data);
	}
}

PlayerBullets::~PlayerBullets()
{
	bullets_.clear();
}

void PlayerBullets::Initialize()
{



	for (auto& data : bullets_) {
		data->collider.isActive = false;
		data->model.isActive = false;
		data->isAlive = false;
	}
}

void PlayerBullets::Update()
{
	//どうせ親子関係とらんだろという処理につき注意

	float delta = Info::GetDeltaTimeF();

	for (auto& data : bullets_) {
		if (!data->isAlive) {
			continue;
		}
		//移動量加算
		data->model.worldTF.translation += data->velo*delta;

		//カウント以上で描画フラグと使用フラグOFF
		if (data->deadCount++ >= data->maxDeadCount) {
			data->isAlive = false;
			data->collider.isActive = false;
			data->model.isActive = false;
		}
		
	}

}

void PlayerBullets::SetData(const LWP::Math::Vector3& pos, const LWP::Math::Vector3& velo)
{

	float delta = Info::GetDeltaTimeF();

	for (auto& data : bullets_) {

		if (!data->isAlive) {
			//使用フラグON
			data->isAlive = true;
			data->collider.isActive = true;
			data->model.isActive = true;
			//初期座標指定
			data->model.worldTF.translation = pos;
			data->velo = velo;
			data->deadCount = 0;

			//移動量の差を埋めるよう後ろを長くする
			data->cap->end = (velo * -1) * delta;

			break;
		}
	}
}

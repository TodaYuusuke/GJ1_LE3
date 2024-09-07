#include "PlayerBullet.h"

using namespace LWP;
using namespace LWP::Object;

PlayerBullets::PlayerBullets()
{
	//同次にどんだけ出るか知らん

}

PlayerBullets::~PlayerBullets()
{
}

void PlayerBullets::Initialize()
{
	bullets_.clear();
}

void PlayerBullets::Update()
{
	//どうせ親子関係とらんだろという処理につき注意

	float delta = Info::GetDeltaTimeF();

	for (auto& data : bullets_) {
		//移動前の座標保存
		Math::Vector3 prePos;
		prePos = data->model.worldTF.translation;
		//移動量加算
		data->model.worldTF.translation += data->velo*delta;

		if (data->deadCount++ >= data->maxDeadCount) {
			data->isAlive = false;
		}
		
	}

	//削除処理
	bullets_.remove_if([](BulletData* data) {
		if (!data->isAlive) {
			return true;
		}
		else {
			return false;
		}
		});
}

void PlayerBullets::SetData(const LWP::Math::Vector3& pos, const LWP::Math::Vector3& velo)
{

	float delta = Info::GetDeltaTimeF();

	BulletData* data=new BulletData();
	//仮で円を読み込み
	data->model.LoadSphere();
	//初期座標指定
	data->model.worldTF.translation = pos;
	data->velo = velo;
	//コライダ設定
	data->collider.SetFollowTarget(&data->model.worldTF);
	data->collider.name = bulletName_;
	Collider::Capsule& cap = data->collider.SetBroadShape(Collider::Capsule());
	//移動量の差を埋めるよう後ろを長くする
	cap.end = (velo*-1)*delta;

	//データ群に追加
	bullets_.emplace_back((data));
}

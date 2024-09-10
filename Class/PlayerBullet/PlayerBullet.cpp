#include "PlayerBullet.h"

using namespace LWP;
using namespace LWP::Object;

PlayerBullets::PlayerBullets()
{
	//同次にどんだけ出るか知らん

}

PlayerBullets::~PlayerBullets()
{
	bullets_.clear();
}

void PlayerBullets::Initialize()
{
	bullets_.clear();
}

void PlayerBullets::Update()
{
	//どうせ親子関係とらんだろという処理につき注意
#ifdef DEMO
	ImGui::Begin("bullet data");
	ImGui::DragFloat3("offset pos", &offset_.x, 0.01f);
	ImGui::End();
#endif // DEMO


	float delta = Info::GetDeltaTimeF();

	for (auto& data : bullets_) {
		if (!data.isAlive) {
			continue;
		}
		//移動量加算
		data.model.worldTF.translation += data.velo * delta;

		//カウント以上で描画フラグと使用フラグOFF
		if (data.deadCount++ >= data.maxDeadCount) {
			data.isAlive = false;
			data.collider.isActive = false;
			data.model.isActive = false;
		}

	}

	//falseの弾削除処理
	bullets_.remove_if([](BulletData& data) {
		if (!data.isAlive) {
			return true;
		}
		return false;
		});

}

void PlayerBullets::SetData(const LWP::Math::Vector3& pos, const LWP::Math::Vector3& velo, const std::string& cName)
{

	float delta = Info::GetDeltaTimeF();


	BulletData& newdata = bullets_.emplace_back() ;
	newdata.model.LoadSphere();
	newdata.model.worldTF.translation = pos+offset_;
	newdata.collider.SetFollowTarget(&newdata.model.worldTF);
	LWP::Object::Collider::Capsule& cap = newdata.collider.SetBroadShape(Collider::Capsule());
	cap.end = (velo * -1) * delta;
	cap.radius = 0.2f;
	newdata.collider.name = cName;
	newdata.velo = velo;


}

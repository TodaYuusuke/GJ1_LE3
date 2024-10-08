#pragma once
#include <Adapter.h>
#include "../Mask/Mask.h"

/// <summary>
/// ジャンプ時に破片を散らばらすパーティクル
/// </summary>
class JumpFragment : public LWP::Object::Particle<LWP::Resource::RigidModel> {
public:	// ** パブリックなメンバ ** //
	// ミルククラウンを生成する座標
	std::vector<LWP::Math::Vector3> milkPosition;


private: // ** 純粋仮想関数のオーバーライド ** //

	/// <summary>
	/// パーティクルを１つ生成する度に呼ばれる関数
	/// </summary>
	/// <param name="newData">生成された実体の参照（mに基準となるmodelは既にコピーされている）</param>
	/// <returns></returns>
	void Generate(Data& newData) override {
		// 大きさをランダムにする
		int scale = LWP::Utility::GenerateRandamNum<int>(90, 110);// 既にコピー済みなので意味なし
		newData.m.worldTF.scale *= static_cast<float>(scale) / 100.0f;

		// 速度ベクトルを生成
		int dir1 = LWP::Utility::GenerateRandamNum<int>(-25, 25);
		int dir2 = LWP::Utility::GenerateRandamNum<int>(0, 100);
		int dir3 = LWP::Utility::GenerateRandamNum<int>(-25, 25);
		// 発射のベクトル
		LWP::Math::Vector3 dir{ dir1 / 100.0f,dir2 / 100.0f, dir3 / 100.0f };
		float multiply = LWP::Utility::GenerateRandamNum<int>(10, 30) / 100.0f;
		newData.velocity = dir.Normalize() * multiply;
		
		newData.collider = new lwp::Collider::Collider;
		newData.collider->SetFollowTarget(&newData.m.worldTF);
		newData.collider->mask.SetBelongFrag(GJMask::Particle());	// フラグ設定
		newData.collider->mask.SetHitFrag(GJMask::Ground() | GJMask::WaterSurface());
		newData.collider->enterLambda = [&newData, this](lwp::Collider::Collider* col) {
			// 地面だった場合の処理
			if (col->name == "Ground") {
				// 跳ねる
				newData.velocity.y *= -0.35f;
				newData.velocity.x *= 0.6f;
				newData.velocity.z *= 0.6f;
			}
			// 水面だった場合の処理
			else if (col->name == "WaterSurface") {
				newData.elapsedTime = 3.0f;	// パーティクルを終わらす
				milkPosition.push_back(newData.m.worldTF.GetWorldPosition());
			}
		};

		lwp::Collider::Sphere& s = newData.collider->SetBroadShape(lwp::Collider::Sphere());
		s.radius = 0.05f;
	};
	/// <summary>
	/// パーティクルの更新処理
	/// </summary>
	/// <param name="data">更新する実態の参照</param>
	/// <returns></returns>
	bool UpdateParticle(Data& data) override {
		// 経過フレーム
		data.elapsedTime += LWP::Info::GetDeltaTimeF();

		data.m.worldTF.translation += data.velocity;    // 速度ベクトルを加算
		data.m.worldTF.rotation += data.velocity;    // ついでに回転させとく
		data.m.worldTF.rotation = data.m.worldTF.rotation.Normalize();

		// 重力を加算
		data.velocity.y += -9.8f / 600.0f;
		
		// 3秒経過で削除
		if (data.elapsedTime > 3.0f) {
			return true;
		}

		return false;
	};
};
#pragma once
#include <Adapter.h>
#include "../Mask/Mask.h"

/// <summary>
/// 水面に落ちた破片が出す水しぶき
/// </summary>
class MilkCrown : public LWP::Object::Particle<LWP::Resource::RigidModel> {
private: // ** 純粋仮想関数のオーバーライド ** //
	// 綺麗に円にパーティクルを出すためにインデックスを保持
	int index = 0;
	// パーティクル一つのサイズ
	int size = 12;

	/// <summary>
	/// パーティクルを１つ生成する度に呼ばれる関数
	/// </summary>
	/// <param name="newData">生成された実体の参照（mに基準となるmodelは既にコピーされている）</param>
	/// <returns></returns>
	void Generate(Data& newData) override {
		// 方向ベクトルを生成
		LWP::Math::Vector3 dir{ 0.0f,1.0f,0.0f };

		float x = 0.15f;
		float y = 0.0f;
		// ラジアン
		float radian = (6.28f / float(size)) * index;
		// 回転
		float cosTheta = std::cos(radian);
		float sinTheta = std::sin(radian);
		dir.x = x * cosTheta - y * sinTheta;
		dir.z = x * sinTheta + y * cosTheta;

		// 発射のベクトル
		float multiply = 0.1f;
		newData.velocity = dir.Normalize() * multiply;

		newData.m.materials = model.materials;

		index++;	// インデックス増加
		if (index >= size) {
			index = 0;
		}
	};
	/// <summary>
	/// パーティクルの更新処理
	/// </summary>
	/// <param name="data">更新する実態の参照</param>
	/// <returns></returns>
	bool UpdateParticle(Data& data) override {
		// 経過フレーム
		data.elapsedFrame++;

		// 速度ベクトルを加算
		data.m.worldTF.translation += data.velocity;

		// 重力を加算
		data.velocity.y += -9.8f / 600.0f;
		
		// 3秒経過で削除
		if (data.elapsedFrame > 60) {
			return true;
		}

		return false;
	};
};
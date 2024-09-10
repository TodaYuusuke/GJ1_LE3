#pragma once
#include "Child/Spider.h"
#include "Child/Slime.h"

#include <vector>

class EnemyManager final {
public:
	EnemyManager() = default;
	~EnemyManager() = default;

	// ** メンバ関数 ** //


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="ptr">プレイヤーポインタ</param>
	void Initialize(Player* ptr);
	// 更新
	void Update();

	/// <summary>
	/// 一番近い死体を取得する
	/// </summary>
	/// <param name="pos"></param>
	/// <returns>敵の実態</returns>
	IEnemy* GetNearDeadBody(LWP::Math::Vector3 pos);

private: // ** メンバ変数 ** //

	// 敵の配列
	std::vector<IEnemy*> enemies_;

private: // ** 初期化時に確定するデータ ** //

	// プレイヤーのポインタ
	Player* player_ = nullptr;
	// プレイヤーから画面外への距離
	float screenOutDistance_ = 10.0f;

	// 敵召喚間隔
	bool isSummon = false;
	float summonInterval_ = 0.0f;
	float kSummonInterval_ = 4.5f;
};
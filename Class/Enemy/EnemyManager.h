#pragma once
#include "Child/Spider.h"
#include "Child/Slime.h"

#include <vector>

class EnemyManager final {
public:
	EnemyManager() = default;
	~EnemyManager() = default;

	// ** メンバ関数 ** //

	// 初期化
	void Initialize(Player* ptr);
	// 更新
	void Update();


private: // ** メンバ変数 ** //

	// 敵の配列
	std::vector<IEnemy*> enemies_;
	// プレイヤーのポインタ
	Player* player_ = nullptr;

};
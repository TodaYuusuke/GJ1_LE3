#pragma once
#include "../IEnemy.h"

class Spider final
	: public IEnemy {
public:
	Spider(Player* ptr) : IEnemy(ptr) {}
	~Spider() = default;

	// ** メンバ関数 ** //

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// 自分のタイプを返す関数
	EnemyType GetType() override { return EnemyType::Spider; }


private: // ** メンバ変数 ** //

};
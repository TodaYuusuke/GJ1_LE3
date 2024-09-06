#pragma once
#include "../IEnemy.h"

class Slime final
	: public IEnemy {
public:
	Slime(Player* ptr) : IEnemy(ptr) {}
	~Slime() = default;

	// ** メンバ関数 ** //

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// 自分のタイプを返す関数
	EnemyType GetType() override { return EnemyType::Slime; }


private: // ** メンバ変数 ** //

};
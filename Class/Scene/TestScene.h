#pragma once
#include "../Enemy/EnemyManager.h"

#include "scene/IScene.h"

class TestScene final
	: public IScene {
public:
	TestScene() = default;
	~TestScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: //*** これより先に必要な処理や変数を記述 ***//

	// 敵管理クラス
	EnemyManager enemyManager_;
	// 仮プレイヤークラス
	Player player_;
};
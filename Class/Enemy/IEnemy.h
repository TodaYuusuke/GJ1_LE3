#pragma once
#include <Adapter.h>

#include"../Player/Player.h"


enum class EnemyType {
	Spider,
	Slime,
	Count
};

class IEnemy {
public:
	IEnemy() = delete;
	IEnemy(Player* ptr) { player_ = ptr; }
	virtual ~IEnemy() = default;

	// ** 純粋仮想関数 ** //

	// 初期化
	virtual void Initialize() = 0;
	// 更新
	virtual void Update() = 0;

	// デバッグ用GUI
	virtual void DebugGUI();
	// 自分のタイプを返す関数
	virtual EnemyType GetType() = 0;


public: // ** メンバ変数 ** //

	// モデル
	LWP::Resource::RigidModel model_;

	// プレイヤーのポインタ
	Player* player_ = nullptr;
	// 生存フラグ
	bool isAlive_ = true;
};
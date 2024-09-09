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
	// 死亡後の更新処理
	virtual void DeadUpdate();

	// 被弾時の処理
	virtual void Hit();

	// デバッグ用GUI
	virtual void DebugGUI();
	// 自分のタイプを返す関数
	virtual EnemyType GetType() = 0;


public: // ** メンバ変数 ** //

	// モデル
	LWP::Resource::SkinningModel model_;

	// 体力
	int health_ = 1;
	// 生存フラグ
	bool isAlive_ = true;

	// プレイヤーのポインタ
	Player* player_ = nullptr;


private: // ** パラメーター ** //

	// 天井の高さ
	float kMaxY_ = 0;
	// 死亡時の上昇速度
	float kUpSpeed_ = 1.0f;
};
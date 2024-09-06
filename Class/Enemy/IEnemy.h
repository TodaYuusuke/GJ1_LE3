#pragma once
#include <Adapter.h>

// 前方宣言（仮なので座標をもつだけ）
class Player {
public:
	LWP::Math::Vector3 position;
};

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

private: // ** メンバ変数 ** //

	// モデル
	LWP::Resource::RigidModel model_;

	// プレイヤーのポインタ
	Player* player_ = nullptr;
	// 生存フラグ
	bool isAlive_ = true;
};
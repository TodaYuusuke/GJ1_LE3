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
	// 親クラスの更新
	void Update();
	// 子クラスの更新
	virtual void ChildUpdate() = 0;

	// スライディング射撃被弾の処理
	virtual void SlidingHit();

	// デバッグ用GUI
	virtual void DebugGUI();
	// 自分のタイプを返す関数
	virtual EnemyType GetType() = 0;


public: // ** メンバ変数 ** //

	// モデル
	LWP::Resource::SkinningModel model_;
	// 速度
	LWP::Math::Vector3 velocity_;

	// 体力
	int health_ = 1;
	// 生存フラグ
	bool isAlive_ = true;

	// プレイヤーのポインタ
	Player* player_ = nullptr;

	//各状態
	enum Behavior {
		Normal,		// 通常
		Knockback,	// ノックバック
		Dying,		// 死亡アニメーション
		DeadBody,	// 死体
		Count
	};
	//状態リクエスト
	std::optional<Behavior> behaviorReq_ = std::nullopt;
	//状態
	Behavior behavior_ = Normal;
	//変更前の状態
	Behavior preBehavior_ = Normal;

	//ImGui用
	std::string behaviorStirng_[Behavior::Count] = {
		"Normal",
		"Knockback",
		"Dying",
		"DeadBody"
	};


private: // ** ステートパターン ** //

	//プレイヤーの初期化関数ポインタテーブル
	static void (IEnemy::* BehaviorInitialize[])();
	//プレイヤーの更新関数ポインタテーブル
	static void (IEnemy::* BehaviorUpdate[])();

#pragma region 各状態初期化と更新
	virtual void InitNormal() = 0;	// 通常
	virtual void UpdateNormal() = 0;
	virtual void InitKnockback();	// ノックバック
	virtual void UpdateKnockback();
	virtual void InitDying();		// 死亡アニメーション
	virtual void UpdateDying();
	virtual void InitDeadBody();	// 死体
	virtual void UpdateDeadBody();
#pragma endregion

private: // ** パラメーター ** //

	// 死亡アニメーション
	struct Dying {
		float kMaxY = 5.0f;	// 天井の高さ
		float kUpSpeed = 2.5f;	// 死亡時の上昇速度
	}dying_;
	
	// ノックバック
	struct Knockback {
		float time = 0.0f;	// 経過時間
		float dir;	// ノックバック方向

		float kTotalTime = 0.5f;	// ノックバック拘束時間
		float kDistance = 5.0f;	// 移動距離
	}knockback_;
};
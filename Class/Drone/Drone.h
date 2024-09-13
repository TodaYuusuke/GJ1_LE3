#pragma once
#include "../Player/Player.h"
#include "../Enemy/EnemyManager.h"

#include "HealItem.h"

class Drone final {
public:
	Drone() = default;
	~Drone();

	// ** メンバ関数 ** //


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="plyerPtr">プレイヤーポインタ</param>
	/// <param name="EnemyPtr">エネミー管理ポインタ</param>
	void Initialize(Player* playerPtr, EnemyManager* enemyPtr);
	// 更新
	void Update();


private: // ** メンバ変数 ** //

	// プレイヤーのポインタ
	Player* player_ = nullptr;
	// 敵のポインタ
	EnemyManager* enemies_ = nullptr;

	// モデル
	LWP::Resource::SkinningModel model_;
	LWP::Resource::Animation animation_;
	// 光源
	LWP::Object::PointLight light_;

	// なめらかな移動用の変数
	LWP::Math::Vector3 goalPosition_ = { 0.0f,0.0f,0.0f }; // Slerp使用のため目標座標をこちらに設定
	float kSlerpT_ = 0.04f;	// Slerpの係数

	// 吸収した敵の数
	int suctionedDeadBody_ = 0;
	// 生成した回復アイテムのインスタンス保持
	std::list<HealItem> heals_;

	// 場外判定
	float outArea_ = 94.0f;
	// アクティブ切り替え
	bool isActive_ = true;

public: // ** アップグレードされるパブリックな変数 ** //

	struct UpgradeParameter {
		// アイテム生成に必要な敵の数
		int kNeedDeadBody = 1;
		// 吸収に必要な時間
		float kSuctionNeedTime = 3.0f;
	}upgradeParamater;

private: // ** ステートパターン ** //

	//各状態
	enum Behavior {
		PlayerFollow,	// プレイヤー追従
		MoveDeadBody,	// 死体に寄る
		Suction,		// 吸収
		GenerateItem,	// アイテム生成
		Count
	};
	//状態リクエスト
	std::optional<Behavior> behaviorReq_ = std::nullopt;
	//状態
	Behavior behavior_ = PlayerFollow;
	//変更前の状態
	Behavior preBehavior_ = PlayerFollow;

	//ImGui用
	std::string behaviorStirng_[Behavior::Count] = {
		"PlayerFollow",
		"MoveDeadBody",
		"Suction",
		"GenerateItem"
	};

	//プレイヤーの初期化関数ポインタテーブル
	static void (Drone::* BehaviorInitialize[])();
	//プレイヤーの更新関数ポインタテーブル
	static void (Drone::* BehaviorUpdate[])();

#pragma region 各状態初期化と更新
	void InitPlayerFollow();	// プレイヤー追従
	void UpdatePlayerFollow();
	void InitMoveDeadBody();	// 死体に寄る
	void UpdateMoveDeadBody();
	void InitSuction();			// 吸収
	void UpdateSuction();
	void InitGenerateItem();	// アイテム生成
	void UpdateGenerateItem();
#pragma endregion


private: // ** パラメータ ** //

	struct PlayerFollow {
		LWP::Math::Vector3 kOffset = {-1.2f, 4.0f, 0.0f };	// プレイヤー追従時のオフセット
		float kSearchRange = 3.0f;	// 死体検知範囲
	}playerFollow_;

	struct MoveDeadBody {
		float kRange = 1.5f;	// 吸収距離
	}moveDeadBody_;

	struct Suction {
		IEnemy* enemy = nullptr;	// 回収する死体のポインタ
		LWP::Math::Vector3 worldPos;	// 死体の場所
		LWP::Math::Vector3 scale;	// 死体のスケール

		float time = 0.0f;	// 経過時間
	}suction_;

};
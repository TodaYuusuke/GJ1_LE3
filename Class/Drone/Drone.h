#pragma once
#include "../Player/Player.h"
#include "../Enemy/EnemyManager.h"


class Drone final {
public:
	Drone() = default;
	~Drone() = default;

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
	LWP::Math::Vector3 goalPosition = { 0.0f,0.0f,0.0f }; // Slerp使用のため目標座標をこちらに設定
	float kSlerpT = 0.09f;	// Slerpの係数

	// アクティブ切り替え
	bool isActive = true;

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
		float kSearchRange = 5.0f;	// 死体検知範囲
	}playerFollow_;

	struct MoveDeadBody {
		float kRange = 1.0f;	// 吸収距離
	}moveDeadBody_;

	struct Suction {
		IEnemy* enemy = nullptr;	// 回収する死体のポインタ
		float time = 2.0f;	// 経過時間

		float kTotalTime = 2.0f;	// かかる時間
	}suction_;

};
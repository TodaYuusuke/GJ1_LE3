#pragma once
#include <Adapter.h>
class Player {
public:

	//初期化
	void Initialize();
	//更新
	void Update();

	//各状態
	enum Behavior {
		Moving,		//移動
		Sliding,	//スライド状態
		QuitSlide,	//スライド終了
		Shot,		//攻撃発射処理
		Jump,		//ジャンプ処理
		_countBehavior
	};

	//プレイヤーの現在の状態を取得
	Behavior& GetBehavior() { return behavior_; }

public:

	const LWP::Math::Vector3 GetWorldPosition() { return model_.worldTF.GetWorldPosition(); }

private:// ** 処理をまとめた関数 ** //
	//デバッグ処理
	void Debug();

	//プレイヤーの初期化関数ポインタテーブル
	static void (Player::* BehaviorInitialize[])();
	//プレイヤーの更新関数ポインタテーブル
	static void (Player::* BehaviorUpdate[])();

#pragma region 各状態初期化と更新
	void InitializeMove();			//移動初期化
	void InitializeSlide();			//スライド初期化
	void InitializeQuitSlide();		//スライドやめる初期化
	void InitializeShot();
	void InitializeJump();

	void UpdateMove();				//移動更新
	void UpdateSlide();				//スライド更新
	void UpdateQuitSlide();			//スライドやめる更新
	void UpdateShot();
	void UpdateJump();
#pragma endregion

	//各状態関係なく更新
	void GlovalUpdate();

	//攻撃ヒット時の処理
	//void OnCollision(Collider::Collider* hitT);

private: // ** パラメータ ** //

	//体力
	int hp_ = 10;
	//弾残弾
	int ammoNum_ = 10;
	//最大弾薬数
	int maxAmmoNum_ = 10;

	//移動速度
	float moveSpd_ = 1.0f;

	//重力
	float gravity_ = 0.01f;
	float acceGravity_ = 0.0f;

	//ジャンプの初期ベクトル量
	float jumpVelo_ = 0.1f;

private: // ** 変数 ** //
	//モデル
	LWP::Resource::RigidModel model_;

	LWP::Object::Collider::Collider collider_;


	//移動ベクトル
	LWP::Math::Vector3 velo_;
	
	//加速度
	LWP::Math::Vector3 acce_;

	

	//ジャンプの落下速度の調整
	float t = 0;

	bool isJump_ = false;

	//向き(変更しない
	float pVeloX_ = 1.0f;

	//スライディングのデータ
	struct SlidingData
	{
		//初期位置
		LWP::Math::Vector3 startPos;

		//減速開始の長さ
		float length = 5.0f;

		//速度
		float spd = 0.5f;

		//減速量
		float acceSpd = 1.0f / 30.0f;

	};
	SlidingData slidingData_{};



	//状態リクエスト
	std::optional<Behavior> behaviorReq_ = std::nullopt;
	//状態
	Behavior behavior_ = Moving;
	//変更前の状態
	Behavior preBehavior_=Moving;

	std::string behaviorStirng_[_countBehavior] = {
		"Moving",
		"Sliding",
		"QuitSlide",
		"Shot",
		"Jump"
	};



};
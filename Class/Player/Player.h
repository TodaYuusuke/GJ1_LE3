#pragma once
#include <Adapter.h>

#include"../PlayerBullet/PlayerBullet.h"

class Player {
public:

	Player();
	~Player();

	//初期化
	void Initialize();
	//更新
	void Update();

	//各状態
	enum Behavior {
		Moving,		//移動
		Sliding,	//スライド状態
		QuitSlide,	//スライド終了
		Jump,		//ジャンプ処理
		_countBehavior
	};

	//プレイヤーの現在の状態を取得
	Behavior& GetBehavior() { return behavior_; }

	//プレイヤーの弾データを取得
	std::vector<BulletData*>& GetPlayerBulletsData() { return bullets_->GetBulletData(); }
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

	void InitializeJump();

	void UpdateMove();				//移動更新
	void UpdateSlide();				//スライド更新
	void UpdateQuitSlide();			//スライドやめる更新
	void UpdateJump();
#pragma endregion

	//各状態関係なく更新
	void GlovalUpdate();

	//弾の発射処理
	void ShotBullet(const LWP::Math::Vector3&v, const std::string& cName, float shotNum =5);

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
	float moveSpd_ = 10.0f;

	//重力
	float gravity_ = 50.0f;
	float acceGravity_ = 0.0f;
	  
	//ジャンプの初期ベクトル量
	float jumpVelo_ = 15.0f;

	//弾の速度
	float bulletsSpd_ = 700;

	//弾のばらつき度合い
	float bulletDispersion_=0.17f;

	//弾の同時発射数
	int shotBulletNum_ = 5;

private: // ** 変数 ** //
	//モデル
	LWP::Resource::SkinningModel model_;
	LWP::Resource::Animation animation;

	//プレイヤー本体のコライダー
	LWP::Object::Collider::Collider collider_;

	//プレイヤーの弾データ
	std::unique_ptr<PlayerBullets>bullets_;


	//移動ベクトル
	LWP::Math::Vector3 velo_;
	
	//加速度
	LWP::Math::Vector3 acce_;

	//ジャンプフラグ
	bool isJump_ = false;

	//向き(変更しない
	float pVeloX_ = 1.0f;

	//スライディングのデータ
	struct SlidingData
	{
		//初期位置
		LWP::Math::Vector3 startPos;

		//減速開始の長さ
		float length = 10.0f;

		//速度
		float spd = 20.0f;

		//減速量
		float acceSpd = 80.0f;

	};
	SlidingData slidingData_{};

	//各射撃時の名前
	std::string standShot = "StandBullet";
	std::string slideShot = "SlidingBullet";


	//状態リクエスト
	std::optional<Behavior> behaviorReq_ = std::nullopt;
	//状態
	Behavior behavior_ = Moving;
	//変更前の状態
	Behavior preBehavior_=Moving;

	//ImGui用
	std::string behaviorStirng_[_countBehavior] = {
		"Moving",
		"Sliding",
		"QuitSlide",
		"Jump"
	};

	//デバッグ用無敵フラグ
	bool isHit_ = true;

};
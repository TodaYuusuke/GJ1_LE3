#pragma once
#include <Adapter.h>

#include"../PlayerBullet/PlayerBullet.h"

class Player {
public://構造体＆enum群
	enum AnimatinNameType {
		A_Idle,
		A_Run,
		A_SlidingStart,
		A_Sliding,
		A_SlidingEnd,
		A_StandShot,
		A_SlidingShot,
		A_JumpStart,
		A_Jumping,
		A_Land,		//着地
		A_Damage,
		A_Recovery,
		A_Dead,
		A_CutBack,
		_countAnimeType
	};

	//弾薬データ
	struct BulletData
	{
		bool isShot_ = false;
		//弾発生位置のオフセット
		LWP::Math::Vector3 offset_{ 0,1.0f,0 };

		//弾の速度
		float bulletsSpd_ = 100;

		//弾残弾
		int ammoRemaining_ = 4;

		//最大弾薬数
		int maxAmmoNum_ = 4;

		//弾のばらつき度合い
		float bulletDispersion_ = 0.5f;

		//同時リロード数
		int simultaneouslyLoaded_ = 1;

		//リロード開始時間
		float reloadStartSec_ = 2.0f;

		//一発のリロードにかかる時間
		float putBulletInSec_ = 0.4f;

		//弾の同時発射数
		int shotpelletNum_ = 5;

		//リロード開始時間カウント
		float currentReloadStartSec_ = 0;

		//リロードの時間
		float currentPutBulletInSec_ = 0;
	};

	//ヒット関連
	struct HitData {
		//HPが減るフラグ
		bool isDownHP_ = true;
		//ヒットフラグ
		bool isHit_ = true;
		//ヒット時の無敵時間
		float noHitSec_ = 3.0f;
		float currentNoHit_ = 0;
		//ヒット時の吹っ飛ぶX方向
		float hitDirection_;
		//吹っ飛ぶときの角度
		float hitHeight_ = 0.3f;
		//初速
		float hitVelocity_ = 20.0f;

		int animeCount_= 0;
		int tenmetuCount_ = 5;
	};

	//スライディングのデータ
	struct SlidingData
	{
		//初期位置
		LWP::Math::Vector3 startPos;

		//減速開始の長さ
		float length = 8.0f;

		//速度
		float spd = 11.0f;

		//減速量
		float acceSpd = 80.0f;

		//発射のが描く
		float shotSlope = 0.0f;

		//反動の角度
		float jumpSlope_ = 0.6f;

		//反動初期ベクトル
		float startVelo = 20.0f;
	};

	struct JumpData {
		//ジャンプフラグ
		bool isJump_ = false;

		//ジャンプの初期ベクトル量
		float jumpVelo_ = 28.0f;

		//ジャンプ時の傾きの量
		float jumpSlope_ = 1.5f;
	};

	struct DeadData {
		//傾き
		float slope=1.0f;
		//初速
		float startVelo = 10.0f;

		float gravity = 20.0f;
	};

	struct  ActiveFlag
	{
		//生きているか
		bool isActive = true;

		//ジャンプできるか
		bool jump = false;

		//弾を消費でスライド停止
		bool slidingStopShot = false;

	};

	//パラメータまとめ
	struct Parameters
	{
		//体力
		int hp = 4;

		//最大HP
		int maxHp = 4;

		//移動速度
		float moveSpd = 10.0f;

		//重力
		float gravity = 60.0f;

		//向き切り替え処理
		bool isturn = false;

		//振り向き速度
		float turnSec = 0.2f;
		float currentTurnSec = 0;

		//移動の慣性処理
		float movingInertiaSec = 0.45f;
		float currentInertia = 0.0f;

		//止まった時の減速処理の増加量
		float stopDecelerationDouble_ = 2.0f;

		//立ち射撃時の減速処理の増加量
		float standShotDecelerationDouble_ = 3.0f;

		//ジャンプ関連
		JumpData jumpData;
		//スライディング関連
		SlidingData slideData;
		//ヒット関連
		HitData hitData;
		//弾関連
		BulletData bulletData;

		//死亡関連
		DeadData deadData;

		//各有効処理フラグ
		ActiveFlag activeFlag;
	};
public:

	Player();
	~Player();

	//初期化
	void Initialize(ParticleManager* particle);
	//更新
	void Update();

	//各状態
	enum Behavior {
		Moving,		//移動
		Sliding,	//スライド状態
		QuitSlide,	//スライド終了
		Jump,		//ジャンプ処理
		SlideStopShot,
		HitSomeone,//ヒット時処理
		Dead,		//死亡処理
		_countBehavior
	};

	//無理やりエリア制限
	void SetArea();

	void StopAllLoopSE();

public:

	//プレイヤーの現在の状態を取得
	Behavior& GetBehavior() { return behavior_; }

	const std::string& GetAnimationType() { return nowPlayAnimeName_; }

	const LWP::Math::Vector3 GetWorldPosition() { return model_.worldTF.GetWorldPosition(); }

	//-1,1で向きを返却
	float GetPlayerDirection();

	//生きているかどうか（生きてる:true。４:false)
	bool GetIsActive() { return parameters_.activeFlag.isActive; }
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
	void InitializeSlideStopShot(); //スライディングキャンセルする発砲処理
	void InitializeHitSomeone();
	void InitializeDead();

	void UpdateMove();				//移動更新
	void UpdateSlide();				//スライド更新
	void UpdateQuitSlide();			//スライドやめる更新
	void UpdateJump();
	void UpdateSlideStopShot();     //スライディングキャンセルする発砲処理
	void UpdateHitSomeone();
	void UpdateDead();
#pragma endregion

	//各状態関係なく更新
	void GlovalUpdate();

	//無敵時間の処理
	void HitUpdate();

	//弾のリロード処理
	void ReloadBullet(float delta);

	//弾の発射処理
	bool ShotBullet(const LWP::Math::Vector3&v, const std::string& cName, float shotNum =5);



	//各入力による処理
	void ToSliding();
	bool ToShot(const LWP::Math::Vector3& velo, const std::string& ammoName);
	void ToJump();
	

public://外部でほしいパラメータ


	//プレイヤーアニメーションデータ
	LWP::Resource::Animation animation;

	//プレイヤーの弾データ
	std::unique_ptr<PlayerBullets>bullets_;

	

	//プレイヤーのパラメータ群
	Parameters parameters_;
private: // ** パラメータ ** //
	struct MinMax {
		LWP::Math::Vector3 min;
		LWP::Math::Vector3 max;

	};
	
	MinMax standAABB_;
	  
	MinMax slideAABB_;

	// 場外判定
	float outArea_ = 94.0f;

public: // ** 変数 ** //
	//モデル
	LWP::Resource::SkinningModel model_;

private:
	
	// パーティクル用
	ParticleManager* particle_ = nullptr;


	std::string animeName_[_countAnimeType] = {
		"00_Idle",
		"01_Run",
		"02_SlidingStart",
		"03_Sliding",
		"04_SlidingEnd",
		"05_StandShot",
		"06_SlidingShot",
		"07_JumpStart",
		"08_Jumping",
		"09_Land",
		"10_Damage",
		"11_Recovery",
		"12_Dead",
		"13_CutBack"
	};

	//アニメーションセット
	void SetAnimation(AnimatinNameType type, bool loop=true);

	std::string nowPlayAnimeName_;

	//プレイヤー本体のコライダー
	struct AABBCollider {
		LWP::Object::Collider::Collider collider;    // 当たり判定
		LWP::Object::Collider::AABB& aabb;    // 形状

		/// <summary>
		/// コンストラクタ
		/// <para>参照変数のために用意</para>
		/// </summary>
		AABBCollider() : aabb(collider.SetBroadShape(LWP::Object::Collider::AABB())) {}
	}aabb_;


	//プレ座標
	std::optional<LWP::Math::Vector3> prePos_=std::nullopt;

	//移動ベクトル
	LWP::Math::Vector3 velo_;
	
	//加速度
	LWP::Math::Vector3 acce_;

	//向き(変更しない
	float pVeloX_ = 1.0f;


	//各射撃時の名前
	std::string standShot = "StandBullet";
	std::string slideShot = "SlidingBullet";

	//状態リクエスト
	std::optional<Behavior> behaviorReq_ = Moving;
	//状態
	Behavior behavior_ = Moving;
	//変更前の状態
	Behavior preBehavior_=Moving;



	//ImGui用
	std::string behaviorStirng_[_countBehavior] = {
		"Moving",
		"Sliding",
		"QuitSlide",
		"Jump",
		"SlideStopShot",
		"HitSomeone",
		"Dead"
	};

	//デバッグ用無敵フラグ
	bool isHitOnDebug_ = true;

	//オーディオがあるフォルダまでのパス
	std::string audioPath_ ="SE/player/";
	//音量
	float audioVolume_ = 1.0f;
	//走る音
	LWP::Resource::Audio audioRun_;
	LWP::Resource::Audio audioSlide_;
	LWP::Resource::Audio audioGetHeal_;
	LWP::Resource::Audio audioShot_;
	LWP::Resource::Audio audioReload_;
	LWP::Resource::Audio audioHit_;

	std::string runPath_ = "run.mp3";
	std::string slidePath_ = "slide2.mp3";
	std::string healPath_ = "heal.mp3";
	std::string shotPath_ = "shot.mp3";
	std::string reloadPath_ = "reload.mp3";
	std::string hitPath_ = "hitEnemy.mp3";

};
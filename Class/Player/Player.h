#pragma once
#include <Adapter.h>
class Player {
public:

	//初期化
	void Initialize();
	//更新
	void Update();

private:
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

	void UpdateMove();				//移動更新
	void UpdateSlide();				//スライド更新
	void UpdateQuitSlide();			//スライドやめる更新
#pragma endregion

private:
	//モデル
	LWP::Resource::RigidModel model_;


	//移動ベクトル
	LWP::Math::Vector3 velo_;

	//移動速度
	float moveSpd_=1.0f;

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


	//向き(変更しない
	float pVeloX_ = 1.0f;

	//各状態
	enum Behavior {
		Moving,		//移動
		Sliding,	//スライド状態
		QuitSlide,	//スライド終了
		_countBehavior
	};

	//状態リクエスト
	std::optional<Behavior> behaviorReq_ = std::nullopt;
	//状態
	Behavior behavior_ = Moving;

	std::string behaviorStirng_[_countBehavior] = {
		"Moving",
		"Sliding",
		"QuitSlide"
	};
};
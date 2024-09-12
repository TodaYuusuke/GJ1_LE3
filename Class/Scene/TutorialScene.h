#pragma once
#include <Adapter.h>
#include "scene/IScene.h"
#include"../Player/Player.h"
#include"../FollowCamera/FollowCamera.h"
#include"../Drone/Drone.h"
#include"../Stage/Stage.h"
#include "../Upgrade/UpgradeManager/UpgradeManager.h"

class TutorialScene final
	: public IScene {
public:
	TutorialScene() = default;
	~TutorialScene() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: //*** これより先に必要な処理や変数を記述 ***//

	//各タスクテーブル
	static void (TutorialScene::* TaskInitialize[])();
	static void (TutorialScene::* TaskUpdate[])();

	//各タスク初期化処理
	void MoveInitialize();
	void SlideInitialize();
	void ShotInitialize();
	void SlideShotInitialize();
	void AnotherInitialize();

	//各タスク更新処理
	void MoveUpdate();
	void SlideUpdate();
	void ShotUpdate();
	void SlideShotUpdate();
	void AnotherUpdate();

	//デバッグ処理
	void Debug();
	//シーン変更処理
	void SceneChange();
private://パラメータ
#pragma region 各タスクのカウント処理
	struct MoveTask {
		float needMovingSec = 5.0f;//かかる時間
		float currentMoving = 0.0f;//カウント
	};
	struct SlideTask
	{
		int maxCount = 5;	//必要な行動回数
		int currentCount = 0;//カウント
		bool isCount = true;//カウント有効フラグ
	};
	struct ShotTask
	{
		int maxCount = 5;
		int currentCount = 0;
		bool isCount = true;
	};
	struct SlideShotTask
	{
		int maxCount = 5;
		int currentCount = 0;
		bool isCount = true;
	};
	struct AnotherTask {
		float needMovingSec = 3.0f;
		float currentMoving = 0.0f;
	};
#pragma endregion

	struct TaskNormas {
		MoveTask move;
		SlideTask slide;
		ShotTask shot;
		SlideShotTask slideShot;
		AnotherTask another;
	}normas_;

private://変数


	//タスク表
	enum Tasks {
		Move,
		Slide,
		Shot,
		SlideShot,
		Another,
		_countTasks
	}tasks_;
	std::optional<Tasks>taskReq_ = Move;


	//プレイヤータスク
	std::string taskName_[_countTasks] = {
		"Move",
		"Slide",
		"Shot,",
		"SlideShot",
		"anotherText"
	};

	//各操作をまとめて終わられるようのもの（現在無意味
	std::map<std::string, bool>taskDatas_;

	//シーン変更フラグ
	bool isSceneChange_ = false;


	//説明ボード画像
	LWP::Primitive::Sprite spriteBoad_;

	//進捗ゲージ画像
	LWP::Primitive::Sprite spriteGage_;

	//画像データ群
	LWP::Resource::Texture taskBoads_[_countTasks];


	//各必要な奴
	FollowCamera followCamera_;
	Player player_;
	Drone drone_;
	Stage stage_;
	// パーティクル管理
	ParticleManager particleManager_;
	EnemyManager enemyManager_; // 敵管理

#pragma region 音関係

	//オーディオがあるフォルダまでのパス
	std::string audioPath_ = "BGM/";
	//bgm
	LWP::Resource::Audio bgm_;
	std::string bgmPath_ = "game3.mp3";

#pragma endregion
};

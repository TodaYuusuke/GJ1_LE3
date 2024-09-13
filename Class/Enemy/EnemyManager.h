#pragma once
#include "Child/Spider.h"
#include "Child/Slime.h"

#include <vector>

class EnemyManager final {
public:
	EnemyManager() = default;
	~EnemyManager();

	// ** メンバ関数 ** //


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="ptr">プレイヤーポインタ</param>
	void Initialize(Player* ptr);
	// 更新
	void Update();

	/// <summary>
	/// ウェーブ開始
	/// </summary>
	/// <param name="waveNum">開始するウェーブ</param>
	void StartWave(int waveNum);
	/// <summary>
	/// ウェーブが終わったかを返す関数
	/// </summary>
	/// <returns>終わっていたらtrue</returns>
	bool GetEndWave();

	/// <summary>
	/// 一番近い死体を取得する
	/// </summary>
	/// <param name="pos"></param>
	/// <returns>敵の実態</returns>
	IEnemy* GetNearDeadBody(LWP::Math::Vector3 pos);
	/// <summary>
	/// 敵の残り数を返す
	/// </summary>
	/// <param name="pos"></param>
	/// <returns>敵の実態</returns>
	int GetRemainingEnemy();

	/// <summary>
	/// クモを召喚する
	/// </summary>
	void SummonSpider(LWP::Math::Vector3 pos);

private: // ** メンバ変数 ** //

	// 敵の配列
	std::vector<IEnemy*> enemies_;

	// 敵のスポーンデータ
	struct EnemyProperty {
		int spawn = 0;	// このウェーブでスポーンする残り数
		int kMaxSpawn = 0;	// 同時にスポーンする最大数

		float summonInterval_ = 0.0f;	// 召喚までの時間
		float kSummonInterval_ = 4.5f;	// 召喚間隔
		// 敵の召喚関数
		std::function<void(LWP::Math::Vector3 pos)> summonFunction_;
	};
	std::map<int, EnemyProperty> enemyProperty_;

	// 一番近くの敵を指すビルボード
	LWP::Primitive::Billboard3D billboard_;


private: // ** 初期化時に確定するデータ ** //

	// プレイヤーのポインタ
	Player* player_ = nullptr;
	
	// 敵召喚間隔
	bool isSummon = true;
	// プレイヤーから画面外への距離
	float screenOutDistance_ = 15.0f;

	// 場外判定
	float outArea_ = 94.0f;
};
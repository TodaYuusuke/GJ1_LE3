#pragma once
#include<Adapter.h>
#include"../IUpgrade/IUpgrade.h"
#include "UpGradeList.h"

// クラスの前方宣言
class GameUIManager;

/// <summary>
/// アップグレードマネージャー
/// </summary>
class UpgradeManager {
private: // サブクラス

	/// <summary>
	/// 単体のアップグレードデータ
	/// </summary>
	struct UpgradeData {
		LWP::Primitive::Sprite ui;		// UI用スプライト
		IUpgrade*			   upgrade; // アップグレード
	};

	/// <summary>
	/// アップグレードの大枠カテゴリ
	/// </summary>
	enum Category {
		BODY,	// 身体
		GUN,	// 銃
		DRONE	// ドローン
	};

public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	UpgradeManager() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~UpgradeManager() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="drone">ドローン</param>
	/// <param name="ui">UIマネージャー</param>
	void Initialize(Player* player, Drone* drone, GameUIManager* ui);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// デバッグ情報表示関数
	/// </summary>
	void DebugGUI();

public: // アクセッサ等

	/// <summary>
	/// スキルポイント追加関数
	/// </summary>
	/// <param name="addingPoint">追加されるスキルポイント</param>
	void AddSkilPoint(uint32_t addingPoint = 1) { skilPoint_ += addingPoint; }

	/// <summary>
	/// 表示トリガーセッター
	/// </summary>
	/// <param name="isDisplay">表示するか</param>
	void SetIsDisplay(bool isDisplay) { isOpenObserver_.t = isDisplay; }

private: // プライベートな関数

	/// <summary>
	/// 配列にアップグレードを追加する
	/// </summary>
	void AddUpgrades();

	/// <summary>
	/// 引数の対象スプライトをリセットする関数
	/// </summary>
	/// <param name="s">対象スプライト</param>
	/// <param name="texName">テクスチャ名</param>
	void SpriteReset(LWP::Primitive::Sprite& s, const std::string& texName);

	/// <summary>
	/// UIの追加関数
	/// </summary>
	/// <param name="upgrade">追加するアップグレードのインスタンス</param>
	/// <param name="category">ペアレントされるカテゴリ</param>
	/// <param name="position">アップグレードの座標</param>
	/// <param name="scale">スケール</param>
	/// <param name="texName">テクスチャ名</param>
	void AddUI(IUpgrade* upgrade, Category category, const LWP::Math::Vector2 position, float scale, const std::string& texName);

	/// <summary>
	/// UIの表示、非表示の切り替え関数
	/// </summary>
	/// <param name="isDisplay">表示、非表示フラグ</param>
	void SwitchDisplayUI(bool isDisplay);

private: // メンバ変数

	// アップグレード格納配列
	std::map<std::string, UpgradeData> upgrades_;

	//プレイヤーポインタ
	Player* player_ = nullptr;

	//ドローンポインタ
	Drone* drone_ = nullptr;

	// ゲームUIマネージャー
	GameUIManager* uiManager_ = nullptr;

	// ウィンドウ管理用オブザーバー
	LWP::Utility::Observer<bool> isOpenObserver_ = false;
	// アップグレードウィンドウが開かれているか
	bool isOpenUpgradeWindow_ = false;
	
	// 所持中のスキルポイント
	uint32_t skilPoint_ = 0;

	// 背景用スプライト
	LWP::Primitive::Sprite backGround_;
	// 大カテゴリ用スプライト
	LWP::Primitive::Sprite bodyParent_;		// 身体
	LWP::Primitive::Sprite gunParent_;		// 銃
	LWP::Primitive::Sprite droneParent_;	// ドローン
};



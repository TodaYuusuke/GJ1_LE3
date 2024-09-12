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
		std::string			   textTex; // テキスト用テクスチャ
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
	~UpgradeManager();

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="drone">ドローン</param>
	void Initialize(Player* player, Drone* drone);

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
	void AddSkilPoint(uint32_t addingPoint = 1) { skillPoint_ += addingPoint; }

	/// <summary>
	/// 表示トリガーセッター
	/// </summary>
	/// <param name="isDisplay">表示するか</param>
	void SetIsDisplay(bool isDisplay) { isOpenObserver_.t = isDisplay; }
	/// <summary>
	/// 表示トリガーゲッター
	/// </summary>
	/// <returns>表示トリガー</returns>
	bool GetIsDisplay() { return isOpenObserver_.t; }

	/// <summary>
	/// 表示トリガー変更状態ゲッター
	/// </summary>
	/// <returns>表示トリガー</returns>
	bool GetIsChangeDisplay() { return isOpenObserver_.GetChanged(); }

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
	/// <param name="textTexName">テキスト用テクスチャ名</param>
	/// <param name="prevUpgradeName">前提アップグレード名</param>
	void AddUI(IUpgrade* upgrade, Category category, const LWP::Math::Vector2 position, float scale, const std::string& texName, const std::string& textTexName, const std::string& prevUpgradeName = "");

	/// <summary>
	/// UIの表示、非表示の切り替え関数
	/// </summary>
	/// <param name="isDisplay">表示、非表示フラグ</param>
	void SwitchDisplayUI(bool isDisplay);

	/// <summary>
	/// 全てのアップグレードとの当たり判定を確認する関数
	/// </summary>
	void CheckCollisionUpgrades();

	/// <summary>
	/// カーソルとアップグレート1つの当たり判定を検証する関数
	/// </summary>
	/// <param name="p1">カーソル座標</param>
	/// <param name="r1">カーソル半径</param>
	/// <param name="p2">アップグレート座標</param>
	/// <param name="r2">アップグレートサイズ</param>
	/// <returns>衝突しているか</returns>
	bool CheckCollision2Upgrade(const LWP::Math::Vector2& p1, const float r1, const LWP::Math::Vector2& p2, const float r2);

	/// <summary>
	/// カーソル移動関数
	/// </summary>
	void CursorInput();

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
	int skillPoint_ = 0;

	// 背景用スプライト
	LWP::Primitive::Sprite backGround_;
	// コネクタ用スプライト
	LWP::Primitive::Sprite backConectorGround_;
	// 大カテゴリ用スプライト
	LWP::Primitive::Sprite bodyParent_;		// 身体
	LWP::Primitive::Sprite gunParent_;		// 銃
	LWP::Primitive::Sprite droneParent_;	// ドローン

	// カーソルスプライト
	LWP::Primitive::Sprite* cursorSprite_;

	// アップグレードテキストスプライト
	LWP::Primitive::Sprite upgradeText_;
};



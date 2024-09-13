#pragma once
#include <Adapter.h>

// フェードインアウト管理クラス
class FadeManager {
public:
	FadeManager() = default;
	~FadeManager();

	// メンバ関数

	// 初期化
	void Init();
	// 更新
	void Update();

	// 終了したか確認
	bool GetIn() { return endIn; }

	// フェードアウト開始したか確認
	bool GetStartOut() { return startOut; }
	// フェードアウト
	void Out();
	// 終了したか確認
	bool GetOut() { return endOut; }

private: //*** これより先に必要な処理や変数を記述 ***//

	// 画面に上からかぶせるスプライト
	LWP::Primitive::Sprite sprite_;

	// アニメーションにかかる時間
	float totalTime_ = 1.0f;
	// アニメーション経過時間
	float time_ = 0.0f;

	// フェードアウト開始フラグ
	bool startOut = false;
		
	// フェードイン終了フラグ
	bool endIn = false;
	// フェードアウト開始フラグ
	bool endOut = false;
};
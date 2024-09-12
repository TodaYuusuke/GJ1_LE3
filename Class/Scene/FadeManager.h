#pragma once
#include <Adapter.h>x

// ポイントライトのあるシーンからポイントライトのあるシーンに飛ぶとクラッシュするため回避用に何もないシーンを作成
class FadeManager {
public:
	FadeManager();
	~FadeManager() = default;

	// メンバ関数

	// フェードイン
	void In();
	// 終了したか確認
	bool GetIn();

	// フェードアウト
	void Out();
	// 終了したか確認
	bool GetOut();

private: //*** これより先に必要な処理や変数を記述 ***//

	LWP::Primitive::Sprite sprite_;
};
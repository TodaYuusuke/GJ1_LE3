#include <Adapter.h>
#include "Class/Scene/Title.h"
#include "Class/Scene/GameScene.h"
#include "Class/Scene/Result.h"
#include "Class/Scene/TestScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::StartUp("Window Title");
	// ReleaseのFPSチェックのためにImGui表示
#if DEMO
	LWP::System::Run(new GameScene);
#else
	LWP::Window::ChangeFullScreenMode();
	LWP::Info::ChangeShowDebugGUI();
	LWP::System::Run(new GameScene);
#endif
	return 0;
}
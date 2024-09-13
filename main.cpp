#include <Adapter.h>
#include "Class/Scene/Title.h"
#include "Class/Scene/GameScene.h"
#include "Class/Scene/Result.h"
#include"Class/Scene/TutorialScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::StartUp("Window Title");
#if DEMO
	LWP::System::Run(new Result(true));
#else
	LWP::Window::ChangeFullScreenMode();
	LWP::Info::ChangeShowDebugGUI();
	LWP::System::Run(new Title);
#endif
	return 0;
}
#include "stdafx.h"
#include <ExampleNodes.h>
#include <Game/Game.h>
#include <Game/GameNodes.h>
#include <Script/Editor/ScriptEditor.h>
#include <Script/Nodes/CommonNodes.h>
#include <Script/ScriptNodeTypeRegistry.h>
#include <tge/imguiinterface/ImGuiInterface.h>
#include <tge/input/InputManager.h>
#include <imgui.h>


void Go(void);

int main(const int /*argc*/, const char* /*argc*/[])
{
	Go();
	return 0;
}

Tga::InputManager* SInputManager;


LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	lParam;
	wParam;
	hWnd;

	if (SInputManager->UpdateEvents(message, wParam, lParam))
		return 0;

	switch (message)
	{
		// this message is read when the window is closed
		case WM_DESTROY:
		{
			// close the application entirely
			PostQuitMessage(0);
			return 0;
		}
	}

	return 0;
}

void Go()
{
	Tga::LoadSettings(TGE_PROJECT_SETTINGS_FILE);

	Tga::EngineConfiguration winconf;

	winconf.myApplicationName = L"TGE - Scriptkurs";
	winconf.myWinProcCallback = [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {return WinProc(hWnd, message, wParam, lParam); };
#ifdef _DEBUG
	winconf.myActivateDebugSystems = Tga::DebugFeature::Fps | Tga::DebugFeature::Mem | Tga::DebugFeature::Filewatcher | Tga::DebugFeature::Cpu | Tga::DebugFeature::Drawcalls | Tga::DebugFeature::OptimizeWarnings;
#else
	winconf.myActivateDebugSystems = Tga::DebugFeature::Filewatcher;
#endif

	if (!Tga::Engine::Start(winconf))
	{
		ERROR_PRINT("Fatal error! Engine could not start!");
		system("pause");
		return;
	}
	
	{
		Tga::RegisterCommonNodes();
		RegisterGameNodes();
		RegisterExampleNodes();

		Game game;
		game.Init();

		Tga::ScriptEditor scriptEditor;
		scriptEditor.Init();

		Tga::Engine& engine = *Tga::Engine::GetInstance();


		Tga::InputManager inputManager(*engine.GetHWND());
		SInputManager = &inputManager;

		while (engine.BeginFrame()) 
		{		
			scriptEditor.Update(engine.GetDeltaTime());

			inputManager.Update();

			game.Update(inputManager, engine.GetDeltaTime());
			game.Render();

			engine.EndFrame();
		}
	}

	Tga::Engine::GetInstance()->Shutdown();
}


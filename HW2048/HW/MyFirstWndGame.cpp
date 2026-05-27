#include "MyFirstWndGame.h"

bool MyFirstWndGame::Initialize()
{
	bool isOn = false;
	_InitTimer();
	isOn = _InitWindow();

	return isOn;
}

void MyFirstWndGame::Run()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_LBUTTONDOWN
				//|| msg.message ==
				)
			{
				std::cout << "***" << __FUNCTION__ << std::endl;
			}
			else if (msg.message == WM_KEYDOWN)
			{
				bool moved = false;

				if (msg.wParam == VK_LEFT)
					moved = MoveLeft();
				else if (msg.wParam == VK_RIGHT)
					moved = MoveRight();
				else if (msg.wParam == VK_UP)
					moved = MoveUp();
				else if (msg.wParam == VK_DOWN)
					moved = MoveDown();

				if (moved)
				{
					//AddRandomTile();
				}
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			Update();
			Render();
		}
	}
}

void MyFirstWndGame::Finalize()
{
	delete m_pGameTimer;
	m_pGameTimer = nullptr;
}

void MyFirstWndGame::_InitTimer()
{
	m_pGameTimer = new GameTimer();
	m_pGameTimer->Reset();
}

bool MyFirstWndGame::_InitWindow()
{
	const wchar_t* className = L"MyFirstWndGame";
	const wchar_t* windowName = L"MyFirstWndGame_HW2048";

	if (false == __super::Create(className, windowName, 1024, 720))
	{
		return false;
	}

	RECT rcClient = {};
	GetClientRect(m_hWnd, &rcClient);
	m_width = rcClient.right - rcClient.left;
	m_height = rcClient.bottom - rcClient.top;

	m_hFrontDC = GetDC(m_hWnd);
	m_hBackDC = CreateCompatibleDC(m_hFrontDC);
	m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);
	m_hDefaultBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);
}

void MyFirstWndGame::Update()
{
	m_pGameTimer->Tick();

	//LogicUpdate();

	m_fDeltaTime = m_pGameTimer->DeltaTimeMS();
	m_fFrameCount += m_fDeltaTime;

	while (m_fFrameCount >= 200.0f)
	{
		//	FixedUpdate();
		m_fFrameCount -= 200.0f;
	}
}

void MyFirstWndGame::Render()
{
	//Clear the back buffer
	::PatBlt(m_hBackDC, 0, 0, m_width, m_height, WHITENESS);

	//메모리 DC에 그려진 결과를 실제 DC(m_hFrontDC)로 복사
	BitBlt(m_hFrontDC, 0, 0, m_width, m_height, m_hBackDC, 0, 0, SRCCOPY);
}

bool MyFirstWndGame::MoveLeft()
{
	std::cout << __FUNCTION__ << std::endl;

	return false;
}

bool MyFirstWndGame::MoveRight()
{
	std::cout << __FUNCTION__ << std::endl;

	return false;
}

bool MyFirstWndGame::MoveUp()
{
	std::cout << __FUNCTION__ << std::endl;

	return false;
}

bool MyFirstWndGame::MoveDown()
{
	std::cout << __FUNCTION__ << std::endl;

	return false;
}
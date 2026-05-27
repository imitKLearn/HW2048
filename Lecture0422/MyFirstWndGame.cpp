#include "INC_Windows.h"
#include "MyFirstWndGame.h"
#include "GameTimer.h"
#include "Collider.h"
#include "GameObject.h"
#include "RenderHelp.h"
#include <iostream>
#include <assert.h>

using namespace learning;

constexpr int MAX_GAME_OBJECT_COUNT = 1000;

bool MyFirstWndGame::Initialize()
{
	m_pGameTimer = new GameTimer();
	m_pGameTimer->Reset();

	const wchar_t* className = L"MyFirstWndGame";
	const wchar_t* windowName = L"MyFirstWndGame";

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

	m_GameObjectPtrTable = new GameObjectBase * [MAX_GAME_OBJECT_COUNT];

	for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
	{
		m_GameObjectPtrTable[i] = nullptr;
	}

#pragma region Random

	// 1. 시드값을 설정하여 실행할 때마다 다른 숫자가 나오게 함
	std::random_device rd;
	std::mt19937 gen(rd());

#pragma endregion

	// 필요한 리소스를 로드해볼까요
#pragma region resource

	// 파일의 절대 경로와 상대 경로 구분
	// IDE 에서 인지하는 현재 경로와 실제 실행 파일을 바로 실행했을 때의 경로 기준이 달라요.
	m_pPlayerBitmapInfo = renderHelp::CreateBitmapInfo(L"./Resource/redbird.png");
	m_pEnemyBitmapInfo = renderHelp::CreateBitmapInfo(L"./Resource/graybird.png");

#pragma endregion

	// 리소스를 먼저 로드한 후에 세팅을 합니다.

	// [CHECK]. 첫 번째 게임 오브젝트는 플레이어 캐릭터로 고정!
	CreatePlayer();

	return true;
}

void MyFirstWndGame::Run()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_LBUTTONDOWN)
			{
				MyFirstWndGame::OnLButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
			}
			else if (msg.message == WM_RBUTTONDOWN)
			{
				MyFirstWndGame::OnRButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
			}
			else if (msg.message == WM_MOUSEMOVE)
			{
				MyFirstWndGame::OnMouseMove(LOWORD(msg.lParam), HIWORD(msg.lParam));
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

	if (m_GameObjectPtrTable)
	{
		for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
		{
			if (m_GameObjectPtrTable[i])
			{
				delete m_GameObjectPtrTable[i];
				m_GameObjectPtrTable[i] = nullptr;
			}
		}
		delete m_GameObjectPtrTable;
	}

	__super::Destroy();
}

void MyFirstWndGame::FixedUpdate()
{
	if (m_EnemySpawnPos.x != 0 && m_EnemySpawnPos.y != 0)
	{
		CreateEnemy();
	}
}

void MyFirstWndGame::LogicUpdate()
{
	UpdatePlayerInfo();

	for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
	{
		if (m_GameObjectPtrTable[i])
		{
			if (i != 0)
			{
				UpdateEnemyInfo(m_GameObjectPtrTable[i]);
			}
			m_GameObjectPtrTable[i]->Update(m_fDeltaTime);
		}
	}

	OnCollisionEnemyEnemy();
}

void MyFirstWndGame::CreatePlayer()
{
	assert(m_GameObjectPtrTable[0] == nullptr && "Player object already exists!");

	GameObject* pNewObject = new GameObject(ObjectType::PLAYER);

	pNewObject->SetName("Player");
	pNewObject->SetPosition(50.0f, 50.0f); // 일단, 임의로 설정
	pNewObject->SetSpeed(2.0f); // 일단, 임의로 설정

	pNewObject->SetWidth(100); // 일단, 임의로 설정
	pNewObject->SetHeight(100); // 일단, 임의로 설정

	pNewObject->SetColliderCircle(50.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.
	pNewObject->SetBitmapInfo(m_pPlayerBitmapInfo); // 여러분은 따로 빼지 않아서 그냥, 하면 됩니다.

	m_GameObjectPtrTable[0] = pNewObject;
}

void MyFirstWndGame::CreateEnemy()
{
	GameObject* pNewObject = new GameObject(ObjectType::ENEMY);

	pNewObject->SetName("Enemy");

	float x = m_EnemySpawnPos.x;
	float y = m_EnemySpawnPos.y;

	m_EnemySpawnPos = { 0, 0 };

	pNewObject->SetPosition(x, y);
	pNewObject->SetSpeed(1.0f); // 일단, 임의로 설정

	pNewObject->SetWidth(100); // 일단, 임의로 설정
	pNewObject->SetHeight(100); // 일단, 임의로 설정

	pNewObject->SetColliderCircle(50.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.
	pNewObject->SetBitmapInfo(m_pEnemyBitmapInfo);

	int i = 0;
	while (++i < MAX_GAME_OBJECT_COUNT) //0번째는 언제나 플레이어!
	{
		if (nullptr == m_GameObjectPtrTable[i])
		{
			m_GameObjectPtrTable[i] = pNewObject;
			break;
		}
	}

	if (i == MAX_GAME_OBJECT_COUNT)
	{
		// 게임 오브젝트 테이블이 가득 찼습니다.
		delete pNewObject;
		pNewObject = nullptr;
	}
}

void MyFirstWndGame::UpdatePlayerInfo()
{
	static GameObject* pPlayer = GetPlayer();

	assert(pPlayer != nullptr);

	Vector2f mousePos(m_PlayerTargetPos.x, m_PlayerTargetPos.y);
	Vector2f playerPos = pPlayer->GetPosition();

	Vector2f playerDir = mousePos - playerPos;
	float distance = playerDir.Length(); // 거리 계산

	if (distance > 50.f) //임의로 설정한 거리
	{
		playerDir.Normalize(); // 정규화
		pPlayer->SetDirection(playerDir); // 플레이어 방향 설정
	}
	else
	{
		pPlayer->SetDirection(Vector2f(0, 0)); // 플레이어 정지
	}
}

/// <summary>
/// 방향 정보, 적과 플레이어 사이 거리
/// </summary>
/// <param name="pEnemy"></param>
void MyFirstWndGame::UpdateEnemyInfo(GameObjectBase* pEnemy)
{
	static GameObject* pPlayer = GetPlayer();

	assert(pPlayer != nullptr);
	assert(pEnemy != nullptr);

	Vector2f playerPos = pPlayer->GetPosition();
	Vector2f EnemyPos = pEnemy->GetPosition();

	Vector2f EnemyDir = playerPos - EnemyPos;
	float distance = EnemyDir.Length(); // 거리 계산

	if (distance > 80.f) //임의로 설정한 거리
	{
		EnemyDir.Normalize(); // 정규화
		pEnemy->SetDirection(EnemyDir); //  방향 설정
	}
	else
	{
		pEnemy->SetDirection(Vector2f(0, 0)); //  정지
	}
}

void MyFirstWndGame::OnCollisionEnemyEnemy()
{
	for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
	{
		GameObject* pEnemy = (GameObject*)m_GameObjectPtrTable[i];
		if (pEnemy == nullptr)
			continue;

		for (int j = i + 1; j < MAX_GAME_OBJECT_COUNT; ++j)
		{
			GameObject* pEnemy2 = (GameObject*)m_GameObjectPtrTable[j];
			if (pEnemy2 == nullptr)
				continue;

			if (pEnemy->GetColliderCircle() == nullptr
				|| pEnemy2->GetColliderCircle() == nullptr)
			{
				continue;
			}

			if (Intersect(*pEnemy->GetColliderCircle(), *pEnemy2->GetColliderCircle()))
			{
				//@todo 충돌했을때 변화 넣기

#pragma region //위치변경
				Vector2f pEnemyPos = pEnemy->GetPosition();
				Vector2f pEnemyPos2 = pEnemy2->GetPosition();

				//#pragma region //// 랜덤하게 -> 너무지저분해보여
				//				static int min = -26;
				//				static int max = 26;
				//				pEnemy->SetPosition(pEnemyPos.x + GetRandomNum(min, max), pEnemyPos.y + GetRandomNum(min, max));
				//				pEnemy2->SetPosition(pEnemyPos2.x + GetRandomNum(min, max), pEnemyPos2.y + GetRandomNum(min, max));
				//#pragma endregion

#pragma region //서로 반대 방향으로,겹치는 만큼 살짝 떨어졌다가 다시 붙기
				Vector2f EnemyDir21 = pEnemyPos - pEnemyPos2;
				Vector2f EnemyDir12 = pEnemyPos2 - pEnemyPos;
				//float distanceOverlap = EnemyDir12.Length(); // 거리 계산
				float distanceOverlap = pEnemy->GetColliderCircle()->radius + pEnemy2->GetColliderCircle()->radius - EnemyDir12.Length();
				distanceOverlap *= 0.5f;

				//float distance = 0100;

				EnemyDir21.Normalize();
				EnemyDir12.Normalize();

				if (i != 0)
				{
					pEnemy->SetPosition((-EnemyDir12 * distanceOverlap + pEnemyPos).x, (-EnemyDir12 * distanceOverlap + pEnemyPos).y);
				}
				pEnemy2->SetPosition((-EnemyDir21 * distanceOverlap + pEnemyPos2).x, (-EnemyDir21 * distanceOverlap + pEnemyPos2).y);

				pEnemy->SetDirection(Vector2f(0, 0)); //  정지
				pEnemy2->SetDirection(Vector2f(0, 0)); //  정지

#pragma endregion

#pragma endregion
			}
		}
	}
}

int MyFirstWndGame::GetRandomNum(int min, int max)
{
	// 1. 시드값을 설정하여 실행할 때마다 다른 숫자가 나오게 함
	std::random_device rd;
	std::mt19937 gen(rd());

	// 2. 범위를 -50에서 50으로 설정 (양 끝값 포함)
	std::uniform_int_distribution<int> dis(min, max);

	// 3. 숫자 생성 및 출력
	int random_number = dis(gen);

	return random_number;
}

void MyFirstWndGame::Update()
{
	m_pGameTimer->Tick();

	LogicUpdate();

	m_fDeltaTime = m_pGameTimer->DeltaTimeMS();
	m_fFrameCount += m_fDeltaTime;

	while (m_fFrameCount >= 200.0f)
	{
		FixedUpdate();
		m_fFrameCount -= 200.0f;
	}
}

void MyFirstWndGame::Render()
{
	//Clear the back buffer
	::PatBlt(m_hBackDC, 0, 0, m_width, m_height, WHITENESS);

	//메모리 DC에 그리기
	for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
	{
		if (m_GameObjectPtrTable[i])
		{
			m_GameObjectPtrTable[i]->Render(m_hBackDC);
		}
	}

	//메모리 DC에 그려진 결과를 실제 DC(m_hFrontDC)로 복사
	BitBlt(m_hFrontDC, 0, 0, m_width, m_height, m_hBackDC, 0, 0, SRCCOPY);
}

void MyFirstWndGame::OnResize(int width, int height)
{
	std::cout << __FUNCTION__ << std::endl;

	learning::SetScreenSize(width, height);

	__super::OnResize(width, height);

	m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

	HANDLE hPrevBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

	DeleteObject(hPrevBitmap);
}

void MyFirstWndGame::OnClose()
{
	std::cout << __FUNCTION__ << std::endl;

	SelectObject(m_hBackDC, m_hDefaultBitmap);

	DeleteObject(m_hBackBitmap);
	DeleteDC(m_hBackDC);

	ReleaseDC(m_hWnd, m_hFrontDC);
}

void MyFirstWndGame::OnMouseMove(int x, int y)
{
	/*   std::cout << __FUNCTION__ << std::endl;
	   std::cout << "x: " << x << ", y: " << y << std::endl;*/
	m_MousePosPrev = m_MousePos;
	m_MousePos = { x, y };
}

void MyFirstWndGame::OnLButtonDown(int x, int y)
{
	std::cout << __FUNCTION__ << std::endl;
	std::cout << "x: " << x << ", y: " << y << std::endl;

	m_PlayerTargetPos.x = x;
	m_PlayerTargetPos.y = y;
}

void MyFirstWndGame::OnRButtonDown(int x, int y)
{
	std::cout << __FUNCTION__ << std::endl;
	std::cout << "x: " << x << ", y: " << y << std::endl;

	m_EnemySpawnPos.x = x;
	m_EnemySpawnPos.y = y;
}
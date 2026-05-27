#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif


//#define _CRTDBG_MAP_ALLOC

//#include "MyFirstWndGame.h"
#include <iostream>
#include <crtdbg.h>

int main()
{
	// 첫 줄은 꼭 이 함수를 호출하고 시작 합니다.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetBreakAlloc(84);

	int* pNum = new int;

	

	return 0;
}
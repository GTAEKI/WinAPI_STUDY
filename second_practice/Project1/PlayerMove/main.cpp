#include<Windows.h>
#include<tchar.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT messageType, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT paintStruct;
	
	static int px = 50;
	static int py = 50;


	// 마우스 클릭
	// 좌클릭 메시지, 좌클릭 뗌 메시지, 더블 클릭 메세지
	// 윈도우 클래스 생성시 style 변수에 CS_DBLCLKS 플래그를 세팅해야 함 -> 더블클릭이 방해가 될 수 있기 때문임
	// lParam을 통해서 마우스 좌표를 얻어올수있음 (옛날에는 마우스는 x,y니까 더 큰 곳에 넣어야겠네? 그래서 longParam에 넣은것이다. 현대에는 
	// int mouseX = LOWORD(lParam); :: 이거는 워드 형변환이고 (DWORD -> WORD) 그럼 하위 비트만 남겠죠?
	// int mouseY = HIWORD(lParam); -> 이 두개는 비트연산을 통해 남기는것 : 하위비트를 밀어버림 ->> 00011111 4개 시프트 00000001
	// 단, 듀얼 모니터 이상을 사용하는 경우에는, 메인 모니터를 기준으로 short형으로 혀ㅛㅇ 변환해줘야 제대로 좌표를 받을 수 있음(메인 모니터 왼쪽에 모니터가 더 있다면 좌표가 음수가 전달되는 경우도 있으므로)
	//  
    // 김성엽의 Win32 -> 우리나라에서 유일하게 모던하게 가르치는곳

	// MouseMove도 있다. WM_MOUSEMOVE -> 움직일때마다 이벤트 발생
	// 아무것도 안해도 마우스 위치 알고싶다면? GetCursorPos(LPPOINT point);가 있다.
	// POINT nowCursorPos;
	// GetCursorPos(&nowCursorPos)로 써서, 내가 원본을 전달하여 값을 받아온다. C# out 매개변수와 동일

	switch (messageType)
	{
	case WM_PAINT:
		
		hdc = BeginPaint(hWnd, &paintStruct);
		TextOut(hdc, px, py, TEXT("옷"), _tcslen(TEXT("옷")));
		EndPaint(hWnd, &paintStruct);

		return S_OK;

	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_LEFT: px -= 16; break;
		case VK_UP: py -= 16; break;
		case VK_RIGHT: px += 16; break;
		case VK_DOWN: py += 16; break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return S_OK;

	case WM_DESTROY:
		PostQuitMessage(0);
		return S_OK;

	}

	return DefWindowProc(hWnd, messageType, wParam, lParam);
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{

	//WNDCLASSEX windowClass;
	WNDCLASS windowClass;

	windowClass.lpszClassName = TEXT("Second Project");
	windowClass.lpszMenuName = NULL;

	windowClass.lpfnWndProc = WndProc;
	windowClass.hInstance = hInstance;

	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.style = CS_VREDRAW | CS_HREDRAW;

	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	RegisterClass(&windowClass);

	HWND hWND = CreateWindow(windowClass.lpszClassName, windowClass.lpszClassName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWND, nCmdShow);

	MSG msg;

	while (TRUE)
	{
		if (GetMessage(&msg, NULL, 0, 0) == FALSE)
		{
			break;
		}

		TranslateMessage(&msg);
		// char 메세지를 위해서만 존재하는것이다.
		// 내부가 구현하기 귀찮아서 복잡하다. 추상화가 잘되어있는 함수,. 무거운것이라고 볼수도 있고?
		// 문자키인지 확인하는것
		// 문자키 확인은 키보드 드라이버가 해준다. -> 실제로 하드웨어를 컨트롤하는 녀석이니까.

		DispatchMessage(&msg);
	}

	return msg.wParam; 
}
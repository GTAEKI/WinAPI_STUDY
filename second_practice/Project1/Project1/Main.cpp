#include<Windows.h>
#include<tchar.h>

#pragma comment(lib, "winmm.lib")

void DrawEllipse(HDC hdc, int x, int y, int radWidth, int radHeight)
{
	Ellipse(hdc, x - radWidth, y - radHeight, x + radWidth, y + radHeight);
}


void DrawRect(HDC hdc, RECT rt)
{
	Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);
}

//CDECL, 함수호출 규약 아래에서는 stdCALL을 window에서 CALLBACK로 이름을 붙여놨다.
//함수 스택프레임 싸악 올라가면 함수를 ~~~
LRESULT CALLBACK WndProc(HWND hWnd, UINT messageType, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT paintStruct;
	static TCHAR str[256] = { 0, };
	int strLen;

	//
	static int px = 50;
	static int py = 50;

	switch (messageType)
	{
	case WM_CHAR:

		if((TCHAR)wParam == ' ')
		{
			str[0] = '1';
			str[1] = '2';
			str[2] = '\0';
		}
		else
		{
			strLen = _tcslen(str);
			str[strLen] = (TCHAR)wParam;
			str[strLen + 1] = '\0';
		}
		InvalidateRect(hWnd, NULL, TRUE); // WM_PAINT가 호출되도록 세팅한 함수, 왜냐? WM_CHAR만 하면 랜더링을 하지 않으므로 텍스트가 반영이 안되어있기 때문에
										// 클라이언트 영역을 무효화 시키는 함수
										// 유효하지 않은 영역이니까 너 칠해야돼~ 한글은 조립과정이 있어서 조립이 다 되었을때만 보임
		// 무효영역이란
		// 원래 프로그램이 보여야 할 모습을 보여주지 못할때, 유효한 상태로 되돌리려는 성질이다.
		// ex_ 창을 옆으로 옮겼을때 그림이 사라졌던 현상을 기억하자
		// [ABCDE]에서 스페이스바를 누르면, [12CDE]가 될것이다. 세번째 매개변수가 FALSE일 경우임. / TRUE일때는 전체를 다 색칠하고 그 위에 그리겠다는 의미
		return S_OK;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &paintStruct);
		TextOut(hdc, 100, 100, str, _tcslen(str));
		EndPaint(hWnd, &paintStruct);
		return S_OK;

	case WM_KEYDOWN:
		// ESC키, 방향키, 엔터키 같은 특수키는 CHAR에서 받지 못함
		// 이것을 특수키로 WM_KEYDOWN에서 가상키가 아니므로,  받음 방향키는 숫자를 두개입력받아야함 (검색하면 나옴)
		// 방향키를 표현하는 값은 하나의 숫자로 표현을 못하는 여러 가지 숫자로 표현됨 (스캔코드 : 검색해보기)
		// 이게 너무 불편해서 하나로 묶은것이 가상키이다. (숫자를 더 확장시켜서 258이면 오른족 빵향키 이런식)
		// 일관성을 만들기위해 만든것이다.
		// OS단에서 키의 스캔 코드를 인식해서 하드웨어에 독립적인 가상 키값을 우리에게 보내주는 것임 , 마우스 또한 가상키값을 가짐

		// 단, 숫자 문자와 영문자 같은 경우는 아스키 문자값 그대로 건네주기 때문에 
		// if(wParam == 'a');

		switch(wParam)
		{
		case VK_LEFT: px -= 16; break;
		case VK_UP: py -= 16; break;
		case VK_RIGHT: px += 16; break;
		case VK_DOWN: py += 16; break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

		//// 연습예제
		//if (wParam == VK_ESCAPE)
		//{
		//	int mbType = MessageBox(hWnd, TEXT("끌꺼야?"), TEXT("알림"), MB_YESNO | MB_ICONASTERISK);
		//	if(mbType == IDYES)
		//	{
		//		SendMessage(hWnd, WM_DESTROY, NULL, NULL);
		//		
		//	}
		//}
		//else if(wParam == '1')
		//{
		//	hdc = GetDC(hWnd);

		//	RECT rect;
		//	rect.left = 50;
		//	rect.top = 50;
		//	rect.bottom = 80;
		//	rect.right = 90;

		//	SelectObject(hdc, GetStockObject(DC_BRUSH));
		//	SetDCBrushColor(hdc, RGB(255, 0, 0));

		//	DrawRect(hdc, rect);

		//	ReleaseDC(hWnd, hdc);
		//}
		//else if(wParam == '2')
		//{
		//	hdc = GetDC(hWnd);

		//	SelectObject(hdc, GetStockObject(DC_BRUSH));
		//	SetDCBrushColor(hdc, RGB(0, 0, 255));

		//	DrawEllipse(hdc, 300, 300, 150, 200);

		//	ReleaseDC(hWnd, hdc);
		//}

		//return S_OK;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		return S_OK;

	}

	return DefWindowProc(hWnd,messageType,wParam,lParam); // 우리는 윈도우를 어떻게 처리해야하는지 모른다. 기본적으로 API로 제공된다.
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
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW); //null -> 기본값을 쓰겠다. IDC :ID커서
	windowClass.hIcon = LoadIcon(NULL,IDI_APPLICATION); 
	windowClass.style = CS_VREDRAW | CS_HREDRAW;

	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;

	//windowClass.cbSize = sizeof(windowClass);
	//windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//RegisterClassEx(&windowClass);
	RegisterClass(&windowClass);

	HWND hWND = CreateWindow(windowClass.lpszClassName, windowClass.lpszClassName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWND, nCmdShow);

	MSG msg;

	while(TRUE)
	{
		if(GetMessage(&msg,NULL,0,0) == FALSE) // WM_WUIT 되었을때 FALSE를 반환함
		{
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam; // 메세지마다 wParam, lParam이 다르다. 문서 읽어야함
}
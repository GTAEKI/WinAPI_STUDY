#include<Windows.h>

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

	switch (messageType)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			int mbType = MessageBox(hWnd, TEXT("끌꺼야?"), TEXT("알림"), MB_YESNO | MB_ICONASTERISK);
			if(mbType == IDYES)
			{
				SendMessage(hWnd, WM_DESTROY, NULL, NULL);
				
			}
		}
		else if(wParam == '1')
		{
			hdc = GetDC(hWnd);

			RECT rect;
			rect.left = 50;
			rect.top = 50;
			rect.bottom = 80;
			rect.right = 90;

			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SetDCBrushColor(hdc, RGB(255, 0, 0));

			DrawRect(hdc, rect);

			ReleaseDC(hWnd, hdc);
		}
		else if(wParam == '2')
		{
			hdc = GetDC(hWnd);

			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SetDCBrushColor(hdc, RGB(0, 0, 255));

			DrawEllipse(hdc, 300, 300, 150, 200);

			ReleaseDC(hWnd, hdc);
		}

		return S_OK;
		
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
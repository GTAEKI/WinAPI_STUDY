#include<Windows.h>
#include<tchar.h>

void DrawEllipse(HDC hdc, int x, int y, int radWidth, int radHeight)
{
	Ellipse(hdc, x - radWidth, y - radHeight, x + radWidth, y + radHeight);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messageType, WPARAM wParam, LPARAM lParam)
{
	static POINT point;
	static int isShoot;
	static TCHAR str[256];

	HDC hdc;
	PAINTSTRUCT paintStruct;

	switch (messageType)
	{
	case WM_PAINT:

		hdc = BeginPaint(hWnd, &paintStruct);
		SelectObject(hdc, GetStockObject(DC_PEN));

		
		SetDCPenColor(hdc, RGB(255, 0, 0));
		DrawEllipse(hdc, point.x, point.y, 30, 30); // 큰 원

		if (isShoot == FALSE) 
		{
			SetDCPenColor(hdc, RGB(0, 255, 0));
			DrawEllipse(hdc, point.x, point.y, 2, 2); // 초점
		}
		else 
		{
			SetTextAlign(hdc, TA_CENTER);
			TextOut(hdc, point.x - 2, point.y - 2, TEXT("Shoot~"), _tcslen(TEXT("Shoot~"))); // 발사시 슛
		}

		SetTextAlign(hdc, TA_LEFT);
		_stprintf(str, TEXT("x: %d, y: %d"), point.x, point.y);
		TextOut(hdc, 10, 10, str, _tcslen(str));

		// TCHAR str[256];
		// _stprintf(str,TEXT("x:%d),nowPos.x);

		EndPaint(hWnd, &paintStruct);

		return S_OK;
	case WM_CREATE: //init관련 작업

		GetCursorPos(&point);

		break;

	case WM_LBUTTONDOWN:
		isShoot = TRUE;
		InvalidateRect(hWnd, NULL, TRUE);
		return S_OK;
	case WM_LBUTTONUP:
		isShoot = FALSE;
		InvalidateRect(hWnd, NULL, TRUE);
		return S_OK;
	case WM_MOUSEMOVE:
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
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
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

// 메세지 정보 만들 수 있음
/* MAKEWPARAM(lowWord, highWord);
*  MAKELPARAM(lowWord, highWord); 여기서 같은 워드인 이유는 L이랑 W랑 이제 같기 때문

// 함수와 반대로 합쳐주는 함수
MAKEWORD(lowByte, highByte);
MAKELONG(longWord, highWord);

분리도 0: 워드랑, 바이트까지 분리가 가능함
딱 봐도 워드는 바이트로 분리하겠고, 롱은 워드로 분리하겠지??
*/
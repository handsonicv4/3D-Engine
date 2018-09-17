#include"windows.h"

class ArkWindow : public WNDCLASSEX
{
public:
	ArkWindow(LPCSTR name);
	~ArkWindow();
	int screenWidth, screenHeight;

	HWND hwnd;
	void Show();
private:

};

ArkWindow::ArkWindow(LPCSTR name)
{
	style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	//lpfnWndProc = WndProc;
	cbClsExtra = 0;
	cbWndExtra = 0;
	hInstance = GetModuleHandle(NULL);
	hIcon = LoadIcon(NULL, IDI_WINLOGO);
	hIconSm = hIcon;
	hCursor = LoadCursor(NULL, IDC_ARROW);
	hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	lpszMenuName = NULL;
	lpszClassName = name;
	cbSize = sizeof(WNDCLASSEX);

	screenWidth = 800;
	screenHeight = 600;

}

ArkWindow::~ArkWindow()
{
	// Remove the window.
	DestroyWindow(hwnd);
	hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(lpszClassName, GetModuleHandle(NULL));
}

void ArkWindow::Show()
{
	int posX, posY;
	RegisterClassEx(this);

	// Place the window in the middle of the screen.
	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, lpszClassName, lpszClassName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN| WS_BORDER| WS_CAPTION,
		posX, posY, screenWidth, screenHeight, NULL, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
}
#pragma once
#include <d2d1.h>
#include <wincodec.h>

class Graphics
{
	ID2D1Factory* m_pFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	IWICImagingFactory* m_pImagingFactory;

public:
	Graphics();
	~Graphics();

	void Init(HWND* phWnd);
	void ResizeWnd(HWND* phWnd, int scale);
	void CreateBrush(float r, float g, float b, ID2D1SolidColorBrush** ppBrush);
	void CreateBitmap(LPCWSTR fileName, ID2D1Bitmap** ppBitmap);
	void BeginDraw();
	void EndDraw();

	void ClearScreen(float r = 0, float g = 0, float b = 0);
	void DrawBitmapTile(ID2D1Bitmap** ppBitmap, D2D1_RECT_F destination, D2D1_RECT_F target);
	void DrawBitmap(ID2D1Bitmap** ppBitmap, D2D1_RECT_F destination);

};

static Graphics* graphics = new Graphics();
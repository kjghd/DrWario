#include "Graphics.h"
#include "SafeRelease.h"
#include "Globals.h"
#include "DIPs.h"

#pragma comment(lib, "d2d1")

Graphics::Graphics()
	:
	m_pFactory(NULL),
	m_pRenderTarget(NULL),
	m_pImagingFactory(NULL)
{
}

Graphics::~Graphics()
{
	SafeRelease(&m_pImagingFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pFactory);
	CoUninitialize();
}

void Graphics::Init(HWND* phWnd)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);
	if (SUCCEEDED(hr))
	{
		RECT rc;
		GetClientRect(*phWnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		m_pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(*phWnd, size),
			&m_pRenderTarget);
	}
	if (SUCCEEDED(hr))
	{
		hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if (SUCCEEDED(hr))
		{
			hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pImagingFactory));
		}
	}
}

void Graphics::ResizeWnd(HWND* phWnd, int scale)
{
	// Get postition on screen
	RECT window;
	GetWindowRect(*phWnd, &window);
	
	// Calculate new dimensions
	RECT adjust = { 0, 0, screenX * scale, screenY * scale };
	AdjustWindowRect(&adjust, WS_CAPTION, FALSE);

	// Set window
	MoveWindow(*phWnd, adjust.right - adjust.left, adjust.bottom - adjust.top, window.left, window.top, FALSE);

	// Update render target
	RECT client;
	GetClientRect(*phWnd, &client);
	m_pRenderTarget->Resize(D2D1::SizeU(client.right, client.bottom));
}

void Graphics::CreateBitmap(LPCWSTR fileName, ID2D1Bitmap** ppBitmap)
{
	// Create decoder
	IWICBitmapDecoder* pDecoder;
	HRESULT hr = m_pImagingFactory->CreateDecoderFromFilename(fileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
	if (SUCCEEDED(hr))
	{
		// Get first frame
		IWICBitmapFrameDecode* pFrame;
		hr = pDecoder->GetFrame(0, &pFrame);
		if (SUCCEEDED(hr))
		{
			// convert to a usable format
			IWICFormatConverter* pConverter;
			m_pImagingFactory->CreateFormatConverter(&pConverter);
			if (SUCCEEDED(hr))
			{
				hr = pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0, WICBitmapPaletteTypeMedianCut);
				if (SUCCEEDED(hr))
				{
					IWICBitmap* pWicBitmap;
					m_pImagingFactory->CreateBitmapFromSource(pConverter, WICBitmapCacheOnDemand, &pWicBitmap);
					if (SUCCEEDED(hr))
					{
						// convert to bitmap
						m_pRenderTarget->CreateBitmapFromWicBitmap(pWicBitmap, ppBitmap);
					}
					SafeRelease(&pWicBitmap);
				}
			}
			SafeRelease(&pConverter);
		}
		SafeRelease(&pFrame);
	}
	SafeRelease(&pDecoder);
}

void Graphics::CreateBrush(float r, float g, float b, ID2D1SolidColorBrush** ppBrush)
{
	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(r,g,b), ppBrush);
}

void Graphics::BeginDraw()
{
	m_pRenderTarget->BeginDraw();
}

void Graphics::EndDraw()
{
	m_pRenderTarget->EndDraw();
}

void Graphics::ClearScreen(float r, float g, float b)
{
	m_pRenderTarget->Clear(D2D1::ColorF(r,g,b));
}

void Graphics::DrawBitmapTile(ID2D1Bitmap** ppBitmap, D2D1_RECT_F destination, D2D1_RECT_F target)
{
	destination.left   = ToDIPsX(destination.left);
	destination.top    = ToDIPsY(destination.top);
	destination.right  = ToDIPsX(destination.right);
	destination.bottom = ToDIPsY(destination.bottom);

	m_pRenderTarget->DrawBitmap(*ppBitmap, destination, 1.f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, target);
}

void Graphics::DrawBitmap(ID2D1Bitmap** ppBitmap, D2D1_RECT_F destination)
{
	destination.left   = ToDIPsX(destination.left);
	destination.top    = ToDIPsY(destination.top);
	destination.right  = ToDIPsX(destination.right);
	destination.bottom = ToDIPsY(destination.bottom);

	m_pRenderTarget->DrawBitmap(*ppBitmap, destination, 1.f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
}
#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include "imgui/imgui.h"
class Menu
{
public:
	Menu();
	~Menu();

	void DrawMenu();

private:
	HWND m_hWnd;

	void BeginDraw();
	void EndDraw();
	void DrawWindow();
	HWND CreateMenuWindow();
	HRESULT CreateDeviceD3D(HWND hWnd);
	IDXGISwapChain* g_pSwapChain;
	ID3D11Device* g_pd3dDevice;
	ID3D11DeviceContext* g_pd3dDeviceContext;
	ID3D11RenderTargetView* g_mainRenderTargetView;

	unsigned m_main_tab = 0;


	//visual

	int m_current_item = 0;
};


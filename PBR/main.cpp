#include <Windows.h>
#include <ctime>

#include "D3DContext.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "Sphere.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "Scene.h"

#include <DirectXMath.h>

#pragma comment (lib, "D3D11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxgi.lib")

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
	HINSTANCE hinstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdArg
) {
	HWND hWnd;
	WNDCLASSEX wc = {};

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hinstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);


	hWnd = CreateWindowEx(
		NULL,
		L"WindowClass",
		L"Direct3D App",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hinstance,
		NULL);

	ShowWindow(hWnd, nCmdArg);

	RECT rect{};
	GetWindowRect(hWnd, &rect);

	WindowProperties windowProperties = {
		rect.right - rect.left,
		rect.bottom - rect.top,
		false,
		false
	};

	D3DContext::currentContext = new D3DContext(hWnd, windowProperties);
	Shader* shader = new Shader("default.hlsl", "vs_main", "ps_main");
	Scene* scene = new Scene(shader);
	Camera* camera = new Camera(
		(float)windowProperties.width / (float)windowProperties.height,
		60,
		1.f,
		0.f,
		2.f
	);
	Sphere* pSphere = new Sphere();
	Mesh* pMesh = new Mesh(
		pSphere->data.size(),
		&pSphere->data[0],
		sizeof(SphereData),
		pSphere->indices.size(),
		&pSphere->indices[0],
		shader,
		new Material()
	);

	pMesh->SetScale({ .2, .2, .2 });
	pMesh->SetPosition({ .5, .5, 0 });

	Light* pLight = new Light();
	pLight->m_position = {.5, .5, -1 };
	pLight->m_color = { 300.47, 200.31, 200.79 };

	scene->AddMesh(pMesh);
	scene->AddLight(pLight);

	//VertexBuffer* buffer = new VertexBuffer(shader, sizeof(float) * pSphere->positions.size() * 3, &pSphere->positions[0]);
	//buffer->Bind();

	//IndexBuffer* pIndexBuffer = new IndexBuffer(&pSphere->indices[0], pSphere->indices.size());

	MSG msg;
	//std::time_t time;
	//std::ofstream file;
	//file.open("log.txt");
	//std::streambuf* sbuf = std::cout.rdbuf();
	//std::cout.rdbuf(file.rdbuf());

	while (TRUE) {
		//time = std::time(nullptr);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}

		camera->UpdateCamera();

		const float color[] = { 0.f, 0.f, 0.f, 1.f };
		D3DContext::GetDeviceContext()->ClearRenderTargetView(D3DContext::GetBackBuffer(), color);

		D3DContext::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//D3DContext::GetDeviceContext()->Draw(3, 0);
		//D3DContext::GetDeviceContext()->DrawIndexed(pIndexBuffer->GetCount(), 0, 0);
		scene->Draw();

		D3DContext::GetSwapChain()->Present(0, 0);
		//std::cout << time << std::endl;

	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}



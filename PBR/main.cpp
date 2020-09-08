#include <Windows.h>
//#include <windowsx.h>
//#include <ctime>
#include <DirectXMath.h>


#include "D3DContext.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "Sphere.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "Scene.h"
#include "Keyboard.h"
#include "Model.h"
#include "vendor/imgui/imgui_impl_dx11.h"
#include "vendor/imgui/imgui_impl_win32.h"



#pragma comment (lib, "D3D11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxgi.lib")
//#ifdef _DEBUG
//#pragma comment (lib, "assimp/debug/assimp-vc142-mtd.lib")
//#else
//#pragma comment (lib, "assimp/release/assimp-vc142-mtd.lib")
//#endif

bool _TEST_CHECKBOX_STATE = false;

Keyboard* pKeyboard = new Keyboard();

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
		CW_USEDEFAULT, CW_USEDEFAULT, 1280, 728,
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
		0.f,
		0.f,
		-1.f
	);
	Sphere* pSphere = new Sphere();
	Material* pMaterial = new Material();

	Model* pModel = new Model("../models/LP1.obj", shader);

	Mesh* pMesh = new Mesh(
		pSphere->data.size(),
		&pSphere->data[0],
		sizeof(SphereData),
		pSphere->indices.size(),
		&pSphere->indices[0],
		shader,
		pMaterial
	);


	pMesh->SetScale({ .2, .2, .2 });
	pMesh->SetPosition({ 0., 0., 0 });

	Light* pLight = new Light();
	pLight->m_position = { 1, 1, 1, 0 };
	pLight->m_ambient = { .2 * 2., .2 * .7, .2 * 1.3, 0 };
	pLight->m_diffuse = { .1, .35, .65, 0 };
	pLight->m_specular = { 1., 1., 1., 0 };

	//scene->AddMesh(pMesh);
	for (auto mesh : pModel->getMeshes()) {
		mesh->SetScale({ .01, .01, .01 });
		scene->AddMesh(mesh);
	}
	scene->AddLight(pLight);

//	VertexBuffer* buffer = new VertexBuffer(shader, sizeof(float) * pSphere->positions.size() * 3, &pSphere->positions[0]);
//	buffer->Bind();

	IndexBuffer* pIndexBuffer = new IndexBuffer(&pSphere->indices[0], pSphere->indices.size());


	pKeyboard->EnableAutoRepeatKeys();


	ImGui::CreateContext();

	ImGui::StyleColorsLight();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(D3DContext::GetDevice(), D3DContext::GetDeviceContext());



	MSG msg;
	//std::time_t time;
	//std::ofstream file;
	//file.open("log.txt");
	//std::streambuf* sbuf = std::cout.rdbuf();
	//std::cout.rdbuf(file.rdbuf());

	XMFLOAT3 pos = camera->getCameraPosition();
	XMFLOAT3 lookAt = camera->getLookAtPosition();
	bool isOpen = true;
	ImGuiWindowFlags imGuiFlags = ImGuiWindowFlags_AlwaysAutoResize;


	while (TRUE) {
		//time = std::time(nullptr);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}


		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		//ImGui::ShowDemoWindow();
		ImGui::Begin("HELLO WORLD!", &isOpen, imGuiFlags);
		ImGui::Text("Position\n");
		if (ImGui::SliderFloat("x", &pos.x, -10.0, 10.)) {
			camera->SetPosition(pos.x, pos.y, pos.z);
		}
		if (ImGui::SliderFloat("y", &pos.y, -10.0, 10.)) {
			camera->SetPosition(pos.x, pos.y, pos.z);
		}
		if (ImGui::SliderFloat("z", &pos.z, -10.0, 10.)) {
			camera->SetPosition(pos.x, pos.y, pos.z);
		}

		ImGui::Text("LookAt\n");
		if (ImGui::SliderFloat("lx", &lookAt.x, -10.0, 10.)) {
			camera->SetLookAt(lookAt);
		}
		if (ImGui::SliderFloat("ly", &lookAt.y, -10.0, 10.)) {
			camera->SetLookAt(lookAt);
		}
		if (ImGui::SliderFloat("lz", &lookAt.z, -10.0, 10.)) {
			camera->SetLookAt(lookAt);
		}

		ImGui::Text("light pos\n");
		if (ImGui::SliderFloat("light x", &pLight->m_position.x, -10.0, 10.)) {
			camera->SetLookAt(lookAt);
		}
		if (ImGui::SliderFloat("light y", &pLight->m_position.y, -10.0, 10.)) {
			camera->SetLookAt(lookAt);
		}
		if (ImGui::SliderFloat("light z", &pLight->m_position.z, -10.0, 10.)) {
			camera->SetLookAt(lookAt);
		}

		ImGui::Text("Material\n");
		if (ImGui::SliderFloat("metallic", &pMaterial->m_r_ao.x, .01, 1.)) {
			pMesh->UpdateMaterialConstantBuffer();
		}
		if (ImGui::SliderFloat("roughness", &pMaterial->m_r_ao.y, .01, 1.)) {
			pMesh->UpdateMaterialConstantBuffer();
		}
		if (ImGui::SliderFloat("ao", &pMaterial->m_r_ao.z, .01, 1.)) {
			pMesh->UpdateMaterialConstantBuffer();
		}
		if (ImGui::SliderFloat("albedo z", &pMaterial->albedo.z, .01, 1.)) {
			pMesh->UpdateMaterialConstantBuffer();
		}

		if (ImGui::IsItemActive()) {
			OutputDebugStringA("IS_ITEM_ACTIVE\n");
		}

		if (ImGui::IsItemDeactivatedAfterChange()) {
			OutputDebugStringA("IS_ITEM_DEACTIVATED_AFTER_CHANGE\n");
		}


		for (auto& mesh : pModel->getMeshes()) {
			bool drawable = mesh->isDrawable();
			if (ImGui::Checkbox(mesh->getName().c_str(), &drawable)) {
				mesh->ChangeDrawable(drawable);
			}
		}




		ImGui::End();

		ImGui::Render();



		while (!pKeyboard->CharBufferIsEmpty()) {
			auto ch = pKeyboard->ReadChar();
			std::string outmsg = "Char: ";
			outmsg += ch;
			outmsg += "\n";
			OutputDebugStringA(outmsg.c_str());
		}

		// TODO: 
		// 1. Lighting 

		while (!pKeyboard->KeyBufferIsEmpty()) {
			KeyboardEvent kbe = pKeyboard->ReadKey();
			unsigned char keycode = kbe.GetKeyCode();
			std::string outmsg = "";
			if (kbe.IsPress()) {
				outmsg += "Key pressed: ";
				if (keycode == 'W') {
					camera->SetPosition(pos.x, pos.y, pos.z + .1);
				}
				if (keycode == 'R') {
					XMFLOAT3 pos = camera->getCameraPosition();
					camera->SetPosition(pos.x, pos.y, pos.z - .1);
				}
				if (keycode == 'A') {
					XMFLOAT3 pos = camera->getCameraPosition();
					camera->SetPosition(pos.x - .1, pos.y, pos.z);
				}
				if (keycode == 'S') {
					XMFLOAT3 pos = camera->getCameraPosition();
					camera->SetPosition(pos.x + .1, pos.y, pos.z);
				}
			}
			else {
				outmsg += "Key released: ";
			}
			outmsg += keycode;
			outmsg += "\n";
			OutputDebugStringA(outmsg.c_str());
		}
		// TODO:
		camera->UpdateCamera();
		// ENDTODO:

		const float color[] = { 0.f, 0.f, 0.f, 1.f };
		D3DContext::GetDeviceContext()->ClearRenderTargetView(D3DContext::GetBackBuffer(), color);

		D3DContext::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//D3DContext::GetDeviceContext()->Draw(3, 0);
		//D3DContext::GetDeviceContext()->DrawIndexed(pIndexBuffer->GetCount(), 0, 0);
		scene->Draw();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		D3DContext::GetSwapChain()->Present(0, 0);
		//std::cout << time << std::endl;

	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		//ImGuiIO& io = ImGui::GetIO();
		//io.MouseDown[0] = true;
		return 0;
	}
	case WM_LBUTTONUP:
	{
		//ImGuiIO& io = ImGui::GetIO();
		//io.MouseDown[0] = true;
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		//ImGuiIO& io = ImGui::GetIO();
		//io.MousePos.x = (signed short)(lParam);
		//io.MousePos.y = (signed short)(lParam >> 16);
		//std::string otp = "X: " + std::to_string(io.MousePos.x) + " Y: " + std::to_string(io.MousePos.y);
		//OutputDebugStringA(otp.c_str());
		return 0;
	}
	case WM_KEYDOWN:
	{
		const unsigned char keycode = static_cast<unsigned char>(wParam);
		if (pKeyboard->IsKeyAutoRepeat()) {
			pKeyboard->OnKeyPressed(keycode);
		}
		else {
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed) {
				pKeyboard->OnKeyPressed(keycode);
			}
		}
		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		pKeyboard->OnKeyReleased(keycode);
		return 0;
	}
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char>(wParam);
		if (pKeyboard->IsCharAutoRepeat()) {
			pKeyboard->OnChar(ch);
		}
		else {
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed) {
				pKeyboard->OnChar(ch);
			}
		}
		return 0;

	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}






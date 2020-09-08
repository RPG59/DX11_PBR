#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <stdexcept>
#include <d3dcompiler.h>
#include <map>
#include <DirectXMath.h>

#include "D3DContext.h"
#include "ConstantBuffer.h"
#include "Material.h"

using namespace DirectX;

class Shader
{
private:
	ID3DBlob* VS, * PS;
	ID3D11VertexShader* m_VS;
	ID3D11PixelShader* m_PS;
	std::map < std::string, void* > m_ConstantBuffers;

public:
	Shader(const char* path, const char* vs_main, const char* ps_main);
	inline ID3DBlob* GetVsBlob() { return VS; }
	inline std::map<std::string, void*>& GetUniformMap() { return m_ConstantBuffers; }
private:
	ID3DBlob* Compile(std::string, std::string, std::string);
};


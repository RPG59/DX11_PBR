#include "Shader.h"

Shader::Shader(const char* path, const char* vs_main, const char* ps_main) {
	std::fstream ifs(path);
	std::string result((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	VS = this->Compile(result, "vs_4_0", vs_main);
	PS = this->Compile(result, "ps_4_0", ps_main);

	if (VS && PS) {
		D3DContext::GetDevice()->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_VS);
		D3DContext::GetDevice()->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_PS);

		D3DContext::GetDeviceContext()->VSSetShader(m_VS, 0, 0);
		D3DContext::GetDeviceContext()->PSSetShader(m_PS, 0, 0);
	}
	else {
		std::cout << "[SHADER]: Init ERROR!" << std::endl;
		return;
	}

	m_ConstantBuffers.insert({ "ModelMatrix", new ConstantBuffer(1, sizeof(XMFLOAT4X4)) });
	m_ConstantBuffers.insert({ "Material", new ConstantBuffer(2, sizeof(Material)) });
}

ID3DBlob* Shader::Compile(std::string source, std::string profile, std::string main) {
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	auto hr_compile = D3DCompile(
		source.c_str(),
		source.size(),
		NULL,
		NULL,
		NULL,
		main.c_str(),
		profile.c_str(),
		D3DCOMPILE_DEBUG,
		NULL,
		&shaderBlob,
		&errorBlob);

	if (errorBlob && errorBlob->GetBufferSize()) {
		auto tmp = (const char*)errorBlob->GetBufferPointer();
			
		DebugBreak();
		errorBlob->Release();
	}

	if (FAILED(hr_compile)) {
		std::cout << "[SHADER]: ERROR!" << std::endl;
		throw std::runtime_error("Shader conpile error, " + main);
	}

	return shaderBlob;
}



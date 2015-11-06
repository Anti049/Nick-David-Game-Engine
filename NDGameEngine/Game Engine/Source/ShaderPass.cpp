#include "Precompiled.h"
#include "ShaderPass.h"
#include "Renderer.h"

ShaderPass::ShaderPass(void)
{
	m_pVertexShader = nullptr;
	m_pGeometryShader = nullptr;
	m_pHullShader = nullptr;
	m_pDomainShader = nullptr;
	m_pPixelShader = nullptr;
	m_pInputLayout = nullptr;
}

ShaderPass::~ShaderPass(void)
{
	SafeRelease(&m_pVertexShader);
	SafeRelease(&m_pGeometryShader);
	SafeRelease(&m_pHullShader);
	SafeRelease(&m_pDomainShader);
	SafeRelease(&m_pPixelShader);
	SafeRelease(&m_pInputLayout);
}

bool ShaderPass::CreateVertexShaderFromCompiledFile(const char* fileName)
{
	char *byteCode;
	size_t byteCodeSize;
	if(!LoadCompiledShaderData(&byteCode, byteCodeSize, fileName))
		return false;

	HRESULT hr = Renderer::m_pDevice->CreateVertexShader(
		byteCode, byteCodeSize, nullptr, &m_pVertexShader);

	delete [] byteCode;

	if(S_OK != hr)
		return false;

	return true;
}

bool ShaderPass::CreateGeometryShaderFromCompiledFile(const char* fileName)
{
	char *byteCode;
	size_t byteCodeSize;
	if(!LoadCompiledShaderData(&byteCode, byteCodeSize, fileName))
		return false;

	HRESULT hr = Renderer::m_pDevice->CreateGeometryShader(
		byteCode, byteCodeSize, nullptr, &m_pGeometryShader);

	delete [] byteCode;

	if(S_OK != hr)
		return false;

	return true;
}

bool ShaderPass::CreateHullShaderFromCompiledFile(const char* fileName)
{
	char *byteCode;
	size_t byteCodeSize;
	if(!LoadCompiledShaderData(&byteCode, byteCodeSize, fileName))
		return false;

	HRESULT hr = Renderer::m_pDevice->CreateHullShader(
		byteCode, byteCodeSize, nullptr, &m_pHullShader);

	delete [] byteCode;

	if(S_OK != hr)
		return false;

	return true;
}

bool ShaderPass::CreateDomainShaderFromCompiledFile(const char* fileName)
{
	char *byteCode;
	size_t byteCodeSize;
	if(!LoadCompiledShaderData(&byteCode, byteCodeSize, fileName))
		return false;

	HRESULT hr = Renderer::m_pDevice->CreateDomainShader(
		byteCode, byteCodeSize, nullptr, &m_pDomainShader);

	delete [] byteCode;

	if(S_OK != hr)
		return false;

	return true;
}

bool ShaderPass::CreatePixelShaderFromCompiledFile(const char* fileName)
{
	char *byteCode;
	size_t byteCodeSize;
	if(!LoadCompiledShaderData(&byteCode, byteCodeSize, fileName))
		return false;

	HRESULT hr = Renderer::m_pDevice->CreatePixelShader(
		byteCode, byteCodeSize, nullptr, &m_pPixelShader);

	delete [] byteCode;

	if(S_OK != hr)
		return false;

	return true;
}

bool ShaderPass::CreateComputeShaderFromCompiledFile(const char* fileName)
{
	char *byteCode;
	size_t byteCodeSize;
	if(!LoadCompiledShaderData(&byteCode, byteCodeSize, fileName))
		return false;

	HRESULT hr = Renderer::m_pDevice->CreateComputeShader(
		byteCode, byteCodeSize, nullptr, &m_pComputeShader);

	delete [] byteCode;

	if(S_OK != hr)
		return false;

	return true;
}

bool ShaderPass::CreateInputLayout(const char* fileName, ShaderType eType)
{
	std::ifstream load; 	
	load.open(fileName, std::ios_base::binary);
	if (load.is_open())
	{
		load.seekg( 0, std::ios_base::end ); 
		size_t vs_byte_code_size = 0;
		vs_byte_code_size = (size_t)load.tellg();
		char *vs_byte_code = new char[vs_byte_code_size];
		load.seekg( 0, std::ios_base::beg ); 
		load.read( vs_byte_code, vs_byte_code_size );
		load.close();

		if (eType == eVERTEX_POSITION)
		{
			const unsigned int unElements = 1;
			D3D11_INPUT_ELEMENT_DESC pInput[unElements] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			Renderer::m_pDevice->CreateInputLayout(pInput, unElements, vs_byte_code, vs_byte_code_size, &m_pInputLayout);
		}
		else if (eType == eVERTEX_POSCOLOR)
		{
			const unsigned int unElements = 2;
			D3D11_INPUT_ELEMENT_DESC pInput[unElements] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			Renderer::m_pDevice->CreateInputLayout(pInput, unElements, vs_byte_code, vs_byte_code_size, &m_pInputLayout);
		}

		delete [] vs_byte_code;
	}

	return (m_pInputLayout != nullptr);
}

bool ShaderPass::LoadCompiledShaderData(char** byteCode, size_t& byteCodeSize, const char* fileName)
{
	std::ifstream load; 	
	load.open(fileName, std::ios_base::binary);
	if(!load.is_open())
		return false;
	load.seekg(0, std::ios_base::end); 
	byteCodeSize = size_t(load.tellg());
	*byteCode = new char[byteCodeSize];
	load.seekg(0, std::ios_base::beg); 
	load.read(*byteCode, byteCodeSize);
	load.close();

	return true;
}
#pragma once

#include "VertexFormats.h"

class ShaderPass
{
public:
	ShaderPass(void);
	~ShaderPass(void);

	bool								CreateVertexShaderFromCompiledFile(const char* fileName);
	bool								CreateGeometryShaderFromCompiledFile(const char* fileName);
	bool								CreateHullShaderFromCompiledFile(const char* fileName);
	bool								CreateDomainShaderFromCompiledFile(const char* fileName);
	bool								CreatePixelShaderFromCompiledFile(const char* fileName);
	bool								CreateComputeShaderFromCompiledFile(const char* fileName);
	bool								CreateInputLayout(const char* fileName, ShaderType eType);

	inline ID3D11VertexShader*			GetVertexShader(void)			{ return m_pVertexShader; }
	inline ID3D11GeometryShader*		GetGeometryShader(void)			{ return m_pGeometryShader; }
	inline ID3D11HullShader*			GetHullShader(void)				{ return m_pHullShader; }
	inline ID3D11DomainShader*			GetDomainShader(void)			{ return m_pDomainShader; }
	inline ID3D11PixelShader*			GetPixelShader(void)			{ return m_pPixelShader; }
	inline ID3D11ComputeShader*			GetComputeShader(void)			{ return m_pComputeShader;	}
	inline ID3D11InputLayout*			GetInputLayout(void)			{ return m_pInputLayout; }

	bool								LoadCompiledShaderData(char** byteCode, size_t& byteCodeSize, const char* fileName);

private:
	ID3D11VertexShader*					m_pVertexShader;
	ID3D11GeometryShader*				m_pGeometryShader;
	ID3D11HullShader*					m_pHullShader;
	ID3D11DomainShader*					m_pDomainShader;
	ID3D11PixelShader*					m_pPixelShader;
	ID3D11ComputeShader*				m_pComputeShader;
	ID3D11InputLayout*					m_pInputLayout;
};
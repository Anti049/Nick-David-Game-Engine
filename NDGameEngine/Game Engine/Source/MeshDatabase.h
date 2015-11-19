#pragma once

#include "RenderMesh.h"
#include "VertexFormats.h"

class MeshDatabase
{
public:
	MeshDatabase(void);
	~MeshDatabase(void);

	template <typename VertexFormat>
	RenderMesh*							BuildMesh(std::vector<VertexFormat> vVertices, std::vector<unsigned int> vIndices, bool bDynamic = false, D3D_PRIMITIVE_TOPOLOGY eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	template <typename VertexFormat>
	RenderMesh*							LoadFromFile(std::string& szFilePath);
	RenderMesh*							BuildSkybox(std::string& szMeshName = std::string("Skybox"));
	RenderMesh*							CreateScreenQuad(std::string& szMeshName, float fLeft, float fTop, float fRight, float fBottom, float4 vColor = float4(1.0f));
	RenderMesh*							CreateScreenQuadTex(std::string& szMeshName, float fLeft, float fTop, float fRight, float fBottom, float4 vColor = float4(1.0f));
	RenderMesh*							CreateParticles(std::string& szMeshName, vector<ParticleVertex> vParticles);

	RenderMesh*							FindRenderMesh(std::string& szMeshName);
	std::string							FindRenderMeshKey(RenderMesh* pMesh);
	void								RemoveRenderMesh(std::string& szMeshName);
	void								RemoveRenderMesh(RenderMesh* pMesh);
	void								ClearMeshMap(void);

private:
	std::map<std::string, RenderMesh*>	m_pMeshMap;
};

template <typename VertexFormat>
RenderMesh* MeshDatabase::BuildMesh(std::vector<VertexFormat> vVertices, std::vector<unsigned int> vIndices, bool bDynamic, D3D_PRIMITIVE_TOPOLOGY eTopology)
{
	RenderMesh* pMesh = new RenderMesh;
	pMesh->AddVertices(vVertices, eTopology, bDynamic);
	pMesh->AddIndices(vIndices);
	return pMesh;
}
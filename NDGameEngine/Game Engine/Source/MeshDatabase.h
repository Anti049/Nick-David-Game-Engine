#pragma once

#include "RenderMesh.h"

class MeshDatabase
{
public:
	MeshDatabase(void);
	~MeshDatabase(void);

	template <typename VertexFormat>
	RenderMesh*							BuildMesh(std::vector<VertexFormat> vVertices, std::vector<unsigned int> vIndices);

	template <typename VertexFormat>
	RenderMesh*							LoadFromFile(std::string& szFilePath);
	RenderMesh*							BuildSkybox(std::string& szMeshName = std::string("Skybox"));
	RenderMesh*							CreateScreenQuad(std::string& szMeshName, float fLeft, float fTop, float fRight, float fBottom, float4 vColor = float4(1.0f));
	RenderMesh*							CreateScreenQuadTex(std::string& szMeshName, float fLeft, float fTop, float fRight, float fBottom, float4 vColor = float4(1.0f));
	RenderMesh*							CreateParticle(void);

	RenderMesh*							FindRenderMesh(std::string& szMeshName);
	std::string							FindRenderMeshKey(RenderMesh* pMesh);
	void								RemoveRenderMesh(std::string& szMeshName);
	void								RemoveRenderMesh(RenderMesh* pMesh);
	void								ClearMeshMap(void);

private:
	std::map<std::string, RenderMesh*>	m_pMeshMap;
};

template <typename VertexFormat>
RenderMesh* MeshDatabase::BuildMesh(std::vector<VertexFormat> vVertices, std::vector<unsigned int> vIndices)
{
	RenderMesh* pMesh = new RenderMesh;
	pMesh->AddVertices(vVertices);
	pMesh->AddIndices(vIndices);
	return pMesh;
}
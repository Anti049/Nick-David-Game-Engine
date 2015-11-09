#include "Precompiled.h"
#include "MeshDatabase.h"

MeshDatabase::MeshDatabase(void)
{

}

MeshDatabase::~MeshDatabase(void)
{
	ClearMeshMap();
}

RenderMesh* MeshDatabase::FindRenderMesh(std::string& szMeshName)
{
	if (m_pMeshMap.find(szMeshName) != m_pMeshMap.end())
		return m_pMeshMap[szMeshName];
	return nullptr;
}
std::string MeshDatabase::FindRenderMeshKey(RenderMesh* pMesh)
{
	if (pMesh)
	{
		for (auto iter = m_pMeshMap.begin(); iter != m_pMeshMap.end(); iter++)
		{
			if ((*iter).second == pMesh)
				return std::string((*iter).first);
		}
	}
	return std::string("Not Found");
}
void MeshDatabase::RemoveRenderMesh(std::string& szMeshName)
{
	if (m_pMeshMap.find(szMeshName) != m_pMeshMap.end())
	{
		RenderMesh* pMesh = FindRenderMesh(szMeshName);
		if (pMesh)
			SafeDelete(pMesh);
		m_pMeshMap.erase(szMeshName);
	}
}
void MeshDatabase::RemoveRenderMesh(RenderMesh* pMesh)
{
	if (pMesh)
	{
		for (auto iter = m_pMeshMap.begin(); iter != m_pMeshMap.end(); iter++)
		{
			if ((*iter).second == pMesh)
			{
				SafeDelete((*iter).second);
				m_pMeshMap.erase(iter);
				return;
			}
		}
	}
}
void MeshDatabase::ClearMeshMap(void)
{
	for (auto iter = m_pMeshMap.begin(); iter != m_pMeshMap.end(); iter++)
	{
		if ((*iter).second)
			SafeDelete((*iter).second);
	}
	m_pMeshMap.clear();
}

template <>
RenderMesh* MeshDatabase::LoadFromFile<VERTEX_POSITION>(std::string& szFilePath)
{
	RenderMesh* pMesh = FindRenderMesh(szFilePath);
	if (pMesh)
		return pMesh;

	std::vector<VERTEX_POSITION> vVertices;
	std::vector<unsigned int> vIndices;

	std::string szFile = szFilePath;
	szFile.erase(szFile.begin() + szFile.find("fbx"), szFile.end());
	szFile.append("bin");
	std::fstream binaryFile(szFile.c_str(), std::ios_base::in | std::ios_base::binary);
	bool bBinary = false;
	if (binaryFile.is_open())
	{
		std::streampos begin, end;
		begin = binaryFile.tellg();
		binaryFile.seekg(0, std::ios_base::end);
		end = binaryFile.tellg();
		if (end - begin > 0)
		{
			binaryFile.seekg(0, std::ios_base::beg);

			unsigned int unNumVerts = 0, unNumIndices = 0;
			binaryFile.read((char*)&unNumVerts, sizeof(unNumVerts));
			VERTEX_POSITION* pVertices = new VERTEX_POSITION[unNumVerts];
			binaryFile.read((char*)pVertices, unNumVerts * sizeof(VERTEX_POSITION));
			binaryFile.read((char*)&unNumIndices, sizeof(unNumIndices));
			unsigned int* pIndices = new unsigned int[unNumIndices];
			binaryFile.read((char*)pIndices, unNumIndices * sizeof(UINT));

			for (unsigned int i = 0; i < unNumVerts; i++)
				vVertices.push_back(pVertices[i]);
			for (unsigned int i = 0; i < unNumIndices; i++)
				vIndices.push_back(pIndices[i]);

			if (vVertices.size() > 0 && vIndices.size() > 0)
				bBinary = true;

			delete [] pVertices;
			delete [] pIndices;
		}
		binaryFile.close();
	}
	if (!bBinary)
	{
		FbxManager* pManager = FbxManager::Create();
		FbxIOSettings* pIOSettings = FbxIOSettings::Create(pManager, IOSN_IMPORT);
		pManager->SetIOSettings(pIOSettings);
		FbxImporter* pImporter = FbxImporter::Create(pManager, "");
		bool bStatus = pImporter->Initialize(szFilePath.c_str(), -1, pManager->GetIOSettings());
		if( !bStatus )
		{
			int lFileMajor, lFileMinor, lFileRevision;
			int lSDKMajor,  lSDKMinor,  lSDKRevision;
			FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);
			pImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);
			FbxString error = pImporter->GetStatus().GetErrorString();
			FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
			FBXSDK_printf("Error returned: %s\n\n", error.Buffer());
			if (pImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
			{
				FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
				FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", szFilePath.c_str(), lFileMajor, lFileMinor, lFileRevision);
			}
		}

		FbxScene* pScene = FbxScene::Create(pManager, "tempScene");
		if (!pImporter->Import(pScene))
		{
			FbxStatus status = pImporter->GetStatus();
			int test = 0;
		}
		//pImporter->Destroy();

		FbxNode* pRootNode = pScene->GetRootNode();
		int nNumChildren = pRootNode->GetChildCount();
		FbxNode* pChildNode = nullptr;
		for (int i = 0; i < nNumChildren; i++)
		{
			pChildNode = pRootNode->GetChild(i);
			FbxMesh* pFBXMesh = pChildNode->GetMesh();
			if (!pFBXMesh)
			{
				FbxStatus status = pImporter->GetStatus();
				int test = 0;
			}
			if (pFBXMesh)
			{
				int polyCount, vertCount, vertID;

				polyCount = pFBXMesh->GetPolygonCount();
				FbxVector4* pControlPoints = pFBXMesh->GetControlPoints();
				int controlPointCount = pFBXMesh->GetControlPointsCount();
				vertID = 0;
				for (int p = 0; p < polyCount; p++)
				{
					vertCount = pFBXMesh->GetPolygonSize(p);
					for (int v = 0; v < vertCount; v++)
					{
						VERTEX_POSITION vVertex;
						int controlPointIndex = pFBXMesh->GetPolygonVertex(p, v);

						vVertex.m_vPosition.x = (float)pControlPoints[controlPointIndex].mData[0];
						vVertex.m_vPosition.y = (float)pControlPoints[controlPointIndex].mData[1];
						vVertex.m_vPosition.z = (float)pControlPoints[controlPointIndex].mData[2];
						vVertex.m_vPosition.z = -vVertex.m_vPosition.z;
						vVertex.m_vPosition.w = 1.0f;
						// Indices
						unsigned int size = (unsigned int)vVertices.size(), i;
						for (i = 0; i < size; i++)
						{
							if (vVertex == vVertices[i])
								break;
						}
						if (i == size)
							vVertices.push_back(vVertex);
						vIndices.push_back(i);
						vertID++;
					}
				}
			}
		}
		for (unsigned int i = 0; i < vIndices.size(); i += 3)
		{
			unsigned int temp = vIndices[i];
			vIndices[i] = vIndices[i + 2];
			vIndices[i + 2] = temp;
		}
		pManager->Destroy();

		binaryFile.open(szFile.c_str(), std::ios_base::out | std::ios_base::binary);
		unsigned int numVerts = (unsigned int)vVertices.size(), numIndices = (unsigned int)vIndices.size();
		if (numVerts && numIndices)
		{
			binaryFile.write((char*)&numVerts, sizeof(numVerts));
			binaryFile.write((char*)&vVertices[0], numVerts * sizeof(VERTEX_POSITION));
			binaryFile.write((char*)&numIndices, sizeof(numIndices));
			binaryFile.write((char*)&vIndices[0], numIndices * sizeof(unsigned int));
		}
		binaryFile.close();
	}

	m_pMeshMap[szFilePath] = BuildMesh(vVertices, vIndices);
	return m_pMeshMap[szFilePath];
}
template <>
RenderMesh* MeshDatabase::LoadFromFile<VERTEX_POSCOLOR>(std::string& szFilePath)
{
	RenderMesh* pMesh = FindRenderMesh(szFilePath);
	if (pMesh)
		return pMesh;

	std::vector<VERTEX_POSCOLOR> vVertices;
	std::vector<unsigned int> vIndices;

	std::string szFile = szFilePath;
	szFile.erase(szFile.begin() + szFile.find("fbx"), szFile.end());
	szFile.append("bin");
	std::fstream binaryFile(szFile.c_str(), std::ios_base::in | std::ios_base::binary);
	bool bBinary = false;
	if (binaryFile.is_open())
	{
		std::streampos begin, end;
		begin = binaryFile.tellg();
		binaryFile.seekg(0, std::ios_base::end);
		end = binaryFile.tellg();
		if (end - begin > 0)
		{
			binaryFile.seekg(0, std::ios_base::beg);

			unsigned int unNumVerts = 0, unNumIndices = 0;
			binaryFile.read((char*)&unNumVerts, sizeof(unNumVerts));
			VERTEX_POSCOLOR* pVertices = new VERTEX_POSCOLOR[unNumVerts];
			binaryFile.read((char*)pVertices, unNumVerts * sizeof(VERTEX_POSCOLOR));
			binaryFile.read((char*)&unNumIndices, sizeof(unNumIndices));
			unsigned int* pIndices = new unsigned int[unNumIndices];
			binaryFile.read((char*)pIndices, unNumIndices * sizeof(UINT));

			for (unsigned int i = 0; i < unNumVerts; i++)
				vVertices.push_back(pVertices[i]);
			for (unsigned int i = 0; i < unNumIndices; i++)
				vIndices.push_back(pIndices[i]);

			if (vVertices.size() > 0 && vIndices.size() > 0)
				bBinary = true;

			delete [] pVertices;
			delete [] pIndices;
		}
		binaryFile.close();
	}
	if (!bBinary)
	{
		FbxManager* pManager = FbxManager::Create();
		FbxIOSettings* pIOSettings = FbxIOSettings::Create(pManager, IOSN_IMPORT);
		pManager->SetIOSettings(pIOSettings);
		FbxImporter* pImporter = FbxImporter::Create(pManager, "");
		bool bStatus = pImporter->Initialize(szFilePath.c_str(), -1, pManager->GetIOSettings());
		if( !bStatus )
		{
			int lFileMajor, lFileMinor, lFileRevision;
			int lSDKMajor,  lSDKMinor,  lSDKRevision;
			FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);
			pImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);
			FbxString error = pImporter->GetStatus().GetErrorString();
			FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
			FBXSDK_printf("Error returned: %s\n\n", error.Buffer());
			if (pImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
			{
				FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
				FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", szFilePath.c_str(), lFileMajor, lFileMinor, lFileRevision);
			}
		}

		FbxScene* pScene = FbxScene::Create(pManager, "tempScene");
		if (!pImporter->Import(pScene))
		{
			FbxStatus status = pImporter->GetStatus();
			int test = 0;
		}
		//pImporter->Destroy();

		FbxNode* pRootNode = pScene->GetRootNode();
		int nNumChildren = pRootNode->GetChildCount();
		FbxNode* pChildNode = nullptr;
		for (int i = 0; i < nNumChildren; i++)
		{
			pChildNode = pRootNode->GetChild(i);
			FbxMesh* pFBXMesh = pChildNode->GetMesh();
			if (!pFBXMesh)
			{
				FbxStatus status = pImporter->GetStatus();
				int test = 0;
			}
			if (pFBXMesh)
			{
				int polyCount, vertCount, vertID;

				polyCount = pFBXMesh->GetPolygonCount();
				FbxVector4* pControlPoints = pFBXMesh->GetControlPoints();
				int controlPointCount = pFBXMesh->GetControlPointsCount();
				vertID = 0;
				for (int p = 0; p < polyCount; p++)
				{
					vertCount = pFBXMesh->GetPolygonSize(p);
					for (int v = 0; v < vertCount; v++)
					{
						VERTEX_POSCOLOR vVertex;
						int controlPointIndex = pFBXMesh->GetPolygonVertex(p, v);
						vVertex.m_vPosition.x = (float)pControlPoints[controlPointIndex].mData[0];
						vVertex.m_vPosition.y = (float)pControlPoints[controlPointIndex].mData[1];
						vVertex.m_vPosition.z = (float)pControlPoints[controlPointIndex].mData[2];
						vVertex.m_vPosition.z = -vVertex.m_vPosition.z;
						vVertex.m_vPosition.w = 1.0f;
						vVertex.m_vColor = vVertex.m_vPosition;
						// Indices
						unsigned int size = (unsigned int)vVertices.size(), i;
						for (i = 0; i < size; i++)
						{
							if (vVertex.m_vPosition == vVertices[i].m_vPosition)
								break;
						}
						if (i == size)
							vVertices.push_back(vVertex);
						vIndices.push_back(i);
						vertID++;
					}
				}
			}
		}
		for (unsigned int i = 0; i < vIndices.size(); i += 3)
		{
			unsigned int temp = vIndices[i];
			vIndices[i] = vIndices[i + 2];
			vIndices[i + 2] = temp;
		}
		pManager->Destroy();

		binaryFile.open(szFile.c_str(), std::ios_base::out | std::ios_base::binary);
		unsigned int numVerts = (unsigned int)vVertices.size(), numIndices = (unsigned int)vIndices.size();
		if (numVerts && numIndices)
		{
			binaryFile.write((char*)&numVerts, sizeof(numVerts));
			binaryFile.write((char*)&vVertices[0], numVerts * sizeof(VERTEX_POSCOLOR));
			binaryFile.write((char*)&numIndices, sizeof(numIndices));
			binaryFile.write((char*)&vIndices[0], numIndices * sizeof(unsigned int));
		}
		binaryFile.close();
	}

	m_pMeshMap[szFilePath] = BuildMesh(vVertices, vIndices);
	return m_pMeshMap[szFilePath];
}
template <>
RenderMesh* MeshDatabase::LoadFromFile<VERTEX_POSTEX>(std::string& szFilePath)
{
	RenderMesh* pMesh = FindRenderMesh(szFilePath);
	if (pMesh)
		return pMesh;

	std::vector<VERTEX_POSTEX> vVertices;
	std::vector<unsigned int> vIndices;

	std::string szFile = szFilePath;
	szFile.erase(szFile.begin() + szFile.find("fbx"), szFile.end());
	szFile.append("bin");
	std::fstream binaryFile(szFile.c_str(), std::ios_base::in | std::ios_base::binary);
	bool bBinary = false;
	if (binaryFile.is_open())
	{
		std::streampos begin, end;
		begin = binaryFile.tellg();
		binaryFile.seekg(0, std::ios_base::end);
		end = binaryFile.tellg();
		if (end - begin > 0)
		{
			binaryFile.seekg(0, std::ios_base::beg);

			unsigned int unNumVerts = 0, unNumIndices = 0;
			binaryFile.read((char*)&unNumVerts, sizeof(unNumVerts));
			VERTEX_POSTEX* pVertices = new VERTEX_POSTEX[unNumVerts];
			binaryFile.read((char*)pVertices, unNumVerts * sizeof(VERTEX_POSTEX));
			binaryFile.read((char*)&unNumIndices, sizeof(unNumIndices));
			unsigned int* pIndices = new unsigned int[unNumIndices];
			binaryFile.read((char*)pIndices, unNumIndices * sizeof(UINT));

			for (unsigned int i = 0; i < unNumVerts; i++)
				vVertices.push_back(pVertices[i]);
			for (unsigned int i = 0; i < unNumIndices; i++)
				vIndices.push_back(pIndices[i]);

			if (vVertices.size() > 0 && vIndices.size() > 0)
				bBinary = true;

			delete [] pVertices;
			delete [] pIndices;
		}
		binaryFile.close();
	}
	if (!bBinary)
	{
		FbxManager* pManager = FbxManager::Create();
		FbxIOSettings* pIOSettings = FbxIOSettings::Create(pManager, IOSN_IMPORT);
		pManager->SetIOSettings(pIOSettings);
		FbxImporter* pImporter = FbxImporter::Create(pManager, "");
		bool bStatus = pImporter->Initialize(szFilePath.c_str(), -1, pManager->GetIOSettings());
		//if( !bStatus )
		{
			int lFileMajor, lFileMinor, lFileRevision;
			int lSDKMajor,  lSDKMinor,  lSDKRevision;
			FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);
			pImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);
			FbxString error = pImporter->GetStatus().GetErrorString();
			FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
			FBXSDK_printf("Error returned: %s\n\n", error.Buffer());
			if (pImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
			{
				FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
				FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", szFilePath.c_str(), lFileMajor, lFileMinor, lFileRevision);
			}
		}

		FbxScene* pScene = FbxScene::Create(pManager, "tempScene");
		if (!pImporter->Import(pScene))
		{
			FbxStatus status = pImporter->GetStatus();
			int test = 0;
		}
		//pImporter->Destroy();

		FbxNode* pRootNode = pScene->GetRootNode();
		int nNumChildren = pRootNode->GetChildCount();
		FbxNode* pChildNode = nullptr;
		for (int i = 0; i < nNumChildren; i++)
		{
			pChildNode = pRootNode->GetChild(i);
			FbxMesh* pFBXMesh = pChildNode->GetMesh();
			if (!pFBXMesh)
			{
				FbxStatus status = pImporter->GetStatus();
				int test = 0;
			}
			if (pFBXMesh)
			{
				int polyCount, vertCount, texCoordCount, vertID;

				polyCount = pFBXMesh->GetPolygonCount();
				FbxVector4* pControlPoints = pFBXMesh->GetControlPoints();
				int controlPointCount = pFBXMesh->GetControlPointsCount();
				vertID = 0;
				for (int p = 0; p < polyCount; p++)
				{
					vertCount = pFBXMesh->GetPolygonSize(p);
					for (int v = 0; v < vertCount; v++)
					{
						VERTEX_POSTEX vVertex;
						int controlPointIndex = pFBXMesh->GetPolygonVertex(p, v);

						vVertex.m_vPosition.x = (float)pControlPoints[controlPointIndex].mData[0];
						vVertex.m_vPosition.y = (float)pControlPoints[controlPointIndex].mData[1];
						vVertex.m_vPosition.z = (float)pControlPoints[controlPointIndex].mData[2];
						vVertex.m_vPosition.z = -vVertex.m_vPosition.z;
						vVertex.m_vPosition.w = 1.0f;

						// UVs
						texCoordCount = pFBXMesh->GetElementUVCount();
						for (int uv = 0; uv < texCoordCount; uv++)
						{
							FbxGeometryElementUV* UVElement = pFBXMesh->GetElementUV(uv);
							FbxLayerElement::EMappingMode uvMappingMode = UVElement->GetMappingMode();
							FbxLayerElement::EReferenceMode uvReferenceMode = UVElement->GetReferenceMode();
							int directIndex = -1;

							if (uvMappingMode == FbxLayerElement::eByControlPoint)
							{
								if (uvReferenceMode == FbxLayerElement::eDirect)
									directIndex = controlPointIndex;
								else if (uvReferenceMode == FbxLayerElement::eIndexToDirect)
									directIndex = UVElement->GetIndexArray().GetAt(controlPointIndex);
							}
							else if (uvMappingMode == FbxLayerElement::eByPolygonVertex)
							{
								if (uvReferenceMode == FbxLayerElement::eDirect || uvReferenceMode == FbxLayerElement::eIndexToDirect)
									directIndex = pFBXMesh->GetTextureUVIndex(p, v);
							}

							if (directIndex != -1)
							{
								FbxVector2 fbxUV = UVElement->GetDirectArray().GetAt(directIndex);
								vVertex.m_vTexCoord.x = (float)fbxUV.mData[0];
								vVertex.m_vTexCoord.y = (float)fbxUV.mData[1];
								vVertex.m_vTexCoord.y = -vVertex.m_vTexCoord.y;
							}
						}
						// Indices
						unsigned int size = (unsigned int)vVertices.size(), i;
						for (i = 0; i < size; i++)
						{
							if (vVertex == vVertices[i])
								break;
						}
						if (i == size)
							vVertices.push_back(vVertex);
						vIndices.push_back(i);
						vertID++;
					}
				}
			}
		}
		for (unsigned int i = 0; i < vIndices.size(); i += 3)
		{
			unsigned int temp = vIndices[i];
			vIndices[i] = vIndices[i + 2];
			vIndices[i + 2] = temp;
		}
		pManager->Destroy();

		binaryFile.open(szFile.c_str(), std::ios_base::out | std::ios_base::binary);
		unsigned int numVerts = (unsigned int)vVertices.size(), numIndices = (unsigned int)vIndices.size();
		if (numVerts && numIndices)
		{
			binaryFile.write((char*)&numVerts, sizeof(numVerts));
			binaryFile.write((char*)&vVertices[0], numVerts * sizeof(VERTEX_POSTEX));
			binaryFile.write((char*)&numIndices, sizeof(numIndices));
			binaryFile.write((char*)&vIndices[0], numIndices * sizeof(unsigned int));
		}
		binaryFile.close();
	}

	m_pMeshMap[szFilePath] = BuildMesh(vVertices, vIndices);
	return m_pMeshMap[szFilePath];
}
template <>
RenderMesh* MeshDatabase::LoadFromFile<VERTEX_POSNORMTEX>(std::string& szFilePath)
{
	RenderMesh* pMesh = FindRenderMesh(szFilePath);
	if (pMesh)
		return pMesh;

	std::vector<VERTEX_POSNORMTEX> vVertices;
	std::vector<unsigned int> vIndices;

	std::string szFile = szFilePath;
	szFile.erase(szFile.begin() + szFile.find("fbx"), szFile.end());
	szFile.append("bin");
	std::fstream binaryFile(szFile.c_str(), std::ios_base::in | std::ios_base::binary);
	bool bBinary = false;
	if (binaryFile.is_open())
	{
		std::streampos begin, end;
		begin = binaryFile.tellg();
		binaryFile.seekg(0, std::ios_base::end);
		end = binaryFile.tellg();
		if (end - begin > 0)
		{
			binaryFile.seekg(0, std::ios_base::beg);

			unsigned int unNumVerts = 0, unNumIndices = 0;
			binaryFile.read((char*)&unNumVerts, sizeof(unNumVerts));
			VERTEX_POSNORMTEX* pVertices = new VERTEX_POSNORMTEX[unNumVerts];
			binaryFile.read((char*)pVertices, unNumVerts * sizeof(VERTEX_POSNORMTEX));
			binaryFile.read((char*)&unNumIndices, sizeof(unNumIndices));
			unsigned int* pIndices = new unsigned int[unNumIndices];
			binaryFile.read((char*)pIndices, unNumIndices * sizeof(UINT));

			for (unsigned int i = 0; i < unNumVerts; i++)
				vVertices.push_back(pVertices[i]);
			for (unsigned int i = 0; i < unNumIndices; i++)
				vIndices.push_back(pIndices[i]);

			if (vVertices.size() > 0 && vIndices.size() > 0)
				bBinary = true;

			delete [] pVertices;
			delete [] pIndices;
		}
		binaryFile.close();
	}
	if (!bBinary)
	{
		FbxManager* pManager = FbxManager::Create();
		FbxIOSettings* pIOSettings = FbxIOSettings::Create(pManager, IOSN_IMPORT);
		pManager->SetIOSettings(pIOSettings);
		FbxImporter* pImporter = FbxImporter::Create(pManager, "");
		bool bStatus = pImporter->Initialize(szFilePath.c_str(), -1, pManager->GetIOSettings());
		//if( !bStatus )
		{
			int lFileMajor, lFileMinor, lFileRevision;
			int lSDKMajor,  lSDKMinor,  lSDKRevision;
			FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);
			pImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);
			FbxString error = pImporter->GetStatus().GetErrorString();
			FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
			FBXSDK_printf("Error returned: %s\n\n", error.Buffer());
			if (pImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
			{
				FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
				FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", szFilePath.c_str(), lFileMajor, lFileMinor, lFileRevision);
			}
		}

		FbxScene* pScene = FbxScene::Create(pManager, "tempScene");
		if (!pImporter->Import(pScene))
		{
			FbxStatus status = pImporter->GetStatus();
			int test = 0;
		}
		//pImporter->Destroy();

		FbxNode* pRootNode = pScene->GetRootNode();
		int nNumChildren = pRootNode->GetChildCount();
		FbxNode* pChildNode = nullptr;
		for (int i = 0; i < nNumChildren; i++)
		{
			pChildNode = pRootNode->GetChild(i);
			FbxMesh* pFBXMesh = pChildNode->GetMesh();
			if (!pFBXMesh)
			{
				FbxStatus status = pImporter->GetStatus();
				int test = 0;
			}
			if (pFBXMesh)
			{
				int polyCount, vertCount, texCoordCount, normalCount, vertID;

				polyCount = pFBXMesh->GetPolygonCount();
				FbxVector4* pControlPoints = pFBXMesh->GetControlPoints();
				int controlPointCount = pFBXMesh->GetControlPointsCount();
				vertID = 0;
				for (int p = 0; p < polyCount; p++)
				{
					vertCount = pFBXMesh->GetPolygonSize(p);
					for (int v = 0; v < vertCount; v++)
					{
						VERTEX_POSNORMTEX vVertex;
						int controlPointIndex = pFBXMesh->GetPolygonVertex(p, v);

						vVertex.m_vPosition.x = (float)pControlPoints[controlPointIndex].mData[0];
						vVertex.m_vPosition.y = (float)pControlPoints[controlPointIndex].mData[1];
						vVertex.m_vPosition.z = (float)pControlPoints[controlPointIndex].mData[2];
						vVertex.m_vPosition.z = -vVertex.m_vPosition.z;
						vVertex.m_vPosition.w = 1.0f;

						// UVs
						texCoordCount = pFBXMesh->GetElementUVCount();
						for (int uv = 0; uv < texCoordCount; uv++)
						{
							FbxGeometryElementUV* UVElement = pFBXMesh->GetElementUV(uv);
							FbxLayerElement::EMappingMode uvMappingMode = UVElement->GetMappingMode();
							FbxLayerElement::EReferenceMode uvReferenceMode = UVElement->GetReferenceMode();
							int directIndex = -1;

							if (uvMappingMode == FbxLayerElement::eByControlPoint)
							{
								if (uvReferenceMode == FbxLayerElement::eDirect)
									directIndex = controlPointIndex;
								else if (uvReferenceMode == FbxLayerElement::eIndexToDirect)
									directIndex = UVElement->GetIndexArray().GetAt(controlPointIndex);
							}
							else if (uvMappingMode == FbxLayerElement::eByPolygonVertex)
							{
								if (uvReferenceMode == FbxLayerElement::eDirect || uvReferenceMode == FbxLayerElement::eIndexToDirect)
									directIndex = pFBXMesh->GetTextureUVIndex(p, v);
							}

							if (directIndex != -1)
							{
								FbxVector2 fbxUV = UVElement->GetDirectArray().GetAt(directIndex);
								vVertex.m_vTexCoord.x = (float)fbxUV.mData[0];
								vVertex.m_vTexCoord.y = (float)fbxUV.mData[1];
								vVertex.m_vTexCoord.y = -vVertex.m_vTexCoord.y;
							}
						}
						// Normals
						normalCount = pFBXMesh->GetElementNormalCount();
						for (int n = 0; n < normalCount; n++)
						{
							FbxGeometryElementNormal* NormalElement = pFBXMesh->GetElementNormal(n);
							FbxLayerElement::EMappingMode normalMappingMode = NormalElement->GetMappingMode();
							FbxLayerElement::EReferenceMode normalReferenceMode = NormalElement->GetReferenceMode();
							int directIndex = -1;

							if (normalMappingMode == FbxGeometryElement::eByPolygonVertex)
							{
								if (normalReferenceMode == FbxGeometryElement::eDirect)
									directIndex = vertID;
								else if (normalReferenceMode == FbxGeometryElement::eIndexToDirect)
									directIndex = NormalElement->GetIndexArray().GetAt(vertID);
							}
							else if(normalMappingMode == FbxGeometryElement::eByControlPoint)
							{
								directIndex = controlPointIndex;
							}

							if (directIndex != -1)
							{
								FbxVector4 fbxNormal = NormalElement->GetDirectArray().GetAt(directIndex);
								vVertex.m_vNormal.x = (float)fbxNormal.mData[0];
								vVertex.m_vNormal.y = (float)fbxNormal.mData[1];
								vVertex.m_vNormal.z = (float)fbxNormal.mData[2];
								vVertex.m_vNormal.z = -vVertex.m_vNormal.z;
							}
						}
						// Indices
						unsigned int size = (unsigned int)vVertices.size(), i;
						for (i = 0; i < size; i++)
						{
							if (vVertex == vVertices[i])
								break;
						}
						if (i == size)
							vVertices.push_back(vVertex);
						vIndices.push_back(i);
						vertID++;
					}
				}
			}
		}
		for (unsigned int i = 0; i < vIndices.size(); i += 3)
		{
			unsigned int temp = vIndices[i];
			vIndices[i] = vIndices[i + 2];
			vIndices[i + 2] = temp;
		}
		pManager->Destroy();

		binaryFile.open(szFile.c_str(), std::ios_base::out | std::ios_base::binary);
		unsigned int numVerts = (unsigned int)vVertices.size(), numIndices = (unsigned int)vIndices.size();
		if (numVerts && numIndices)
		{
			binaryFile.write((char*)&numVerts, sizeof(numVerts));
			binaryFile.write((char*)&vVertices[0], numVerts * sizeof(VERTEX_POSNORMTEX));
			binaryFile.write((char*)&numIndices, sizeof(numIndices));
			binaryFile.write((char*)&vIndices[0], numIndices * sizeof(unsigned int));
		}
		binaryFile.close();
	}

	m_pMeshMap[szFilePath] = BuildMesh(vVertices, vIndices);
	return m_pMeshMap[szFilePath];
}

RenderMesh* MeshDatabase::CreateScreenQuad(std::string& szMeshName, float fLeft, float fTop, float fRight, float fBottom, float4 vColor)
{
	RenderMesh* pMesh = FindRenderMesh(szMeshName);
	if (pMesh)
		return pMesh;

	std::vector<VERTEX_POSCOLOR2D> vVertices;
	std::vector<unsigned int> vIndices;
	VERTEX_POSCOLOR2D vVertex;

	vVertex.m_vColor = vColor;
	vVertex.m_vPosition = float2(fLeft, fTop);
	vVertices.push_back(vVertex);
	vVertex.m_vPosition = float2(fRight, fTop);
	vVertices.push_back(vVertex);
	vVertex.m_vPosition = float2(fLeft, fBottom);
	vVertices.push_back(vVertex);
	vVertex.m_vPosition = float2(fRight, fBottom);
	vVertices.push_back(vVertex);

	vIndices.push_back(0);
	vIndices.push_back(1);
	vIndices.push_back(2);
	vIndices.push_back(2);
	vIndices.push_back(1);
	vIndices.push_back(3);

	m_pMeshMap[szMeshName] = BuildMesh(vVertices, vIndices);
	return m_pMeshMap[szMeshName];
}
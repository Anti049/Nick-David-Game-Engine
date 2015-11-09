#pragma once

class Texture;

class TextureDatabase
{
public:
	TextureDatabase(void);
	~TextureDatabase(void);

	Texture*										LoadTexture(wstring szFilePath);

	void											AddTexture(wstring szTexture, Texture* pTexture);
	
	void											RemoveTexture(Texture* pTexture);
	void											RemoveTexture(wstring szTexture);

	void											ClearTextureMap(void);

private:
	map<wstring, Texture*>							m_pTextureMap;
};
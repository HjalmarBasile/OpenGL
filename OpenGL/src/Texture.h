#pragma once

#include <string>

#include "Renderer.h"

#define DICE_TEXTURE_PATH "res/textures/dice.png"

class Texture
{
private:
	unsigned int m_RendererID;
	int m_Width, m_Height;
	int m_Channels;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline int GetChannels() const { return m_Channels; }
};
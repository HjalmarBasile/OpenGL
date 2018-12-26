#pragma once

#include <string>

#include "Renderer.h"

static constexpr const char* AWESOME_FACE_TEXTURE_PATH = "res/textures/awesome-face.png";
static constexpr const char* CRATE_TEXTURE_PATH = "res/textures/crate.png";
static constexpr const char* DICE_TEXTURE_PATH = "res/textures/dice.png";

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
	static void Unbind();

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline int GetChannels() const { return m_Channels; }
};

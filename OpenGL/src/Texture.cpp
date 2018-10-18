#include "Texture.h"

#include "stb/stb_image.h"

static constexpr int RGBA_CHANNELS = 4;

Texture::Texture(const std::string& path)
{
	/*
		Flip the image vertically, so the first pixel in the output array is the bottom left.
		This is needed to load PNGs with OpenGL.
	*/
	stbi_set_flip_vertically_on_load(1);

	/* Load texture into memory */
	unsigned char* localBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, RGBA_CHANNELS);

	GLCheckErrorCall(glGenTextures(1, &m_RendererID));
	GLCheckErrorCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	/* Set mandatory parameters */
	GLCheckErrorCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCheckErrorCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCheckErrorCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCheckErrorCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	/* Specify a two-dimensional texture image */
	GLCheckErrorCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));

	this->Unbind();

	if (localBuffer) {
		stbi_image_free(localBuffer);
	}
}

Texture::~Texture()
{
	GLCheckErrorCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot /* = 0 */) const
{
	/* Texture slots vary from 0x84C0 to 0x84DF */
	GLenum textureSlot = GL_TEXTURE0 + slot;

	/* Select active texture unit */
	GLCheckErrorCall(glActiveTexture(textureSlot));

	GLCheckErrorCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind()
{
	GLCheckErrorCall(glBindTexture(GL_TEXTURE_2D, 0));
}

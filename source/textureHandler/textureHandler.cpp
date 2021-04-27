#include "textureHandler.h"
#include <glad/glad.h>

TextureHandler::TextureHandler()
	: m_Width(0)
	,m_Height(0)
	,m_ImageFormat(GL_RGB)
	,m_InternalFormat(GL_RGB)
	,m_WrapS(GL_REPEAT)
	,m_WrapT(GL_REPEAT)
	,m_FilterMin(GL_LINEAR)
	,m_FilterMax(GL_LINEAR)
{
	glGenTextures(1, &m_ID);
}

TextureHandler::~TextureHandler()
{
	//@TODO remove below while testing
	//glDeleteTextures(1, &m_ID);
}

TextureHandler::TextureHandler(unsigned int width, unsigned int height, unsigned int internalFormat, unsigned int imageFormat, unsigned int wrapS, unsigned int wrapT, unsigned int filterMin, unsigned int filterMax)
	: m_Width(width)
	, m_Height(height)
	, m_ImageFormat(internalFormat)
	, m_InternalFormat(imageFormat)
	, m_WrapS(wrapS)
	, m_WrapT(wrapT)
	, m_FilterMin(wrapT)
	, m_FilterMax(wrapS)
{
}


void TextureHandler::setInternalFormat(unsigned int format)
{
	m_InternalFormat = format;
}

void TextureHandler::setImageFormat(unsigned int format)
{
	m_ImageFormat = format;
}

void TextureHandler::generateTex(unsigned int width, unsigned int height, unsigned char* data)
{
	m_Width = width;
	m_Height = height;

	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_ImageFormat, GL_UNSIGNED_BYTE, data);
	//wrap and filter mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_WrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_FilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_FilterMax);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureHandler::bindTex() const
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void TextureHandler::clear()
{
	glDeleteTextures(1, &m_ID);
}

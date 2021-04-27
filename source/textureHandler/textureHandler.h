#pragma once


class TextureHandler
{
public:
	TextureHandler();
	~TextureHandler();
	TextureHandler(
		unsigned int width,
		unsigned int height,
		unsigned int internalFormat,
		unsigned int imageFormat,
		unsigned int wrapS,
		unsigned int wrapT,
		unsigned int filterMin,
		unsigned int filterMax
	);

	void setInternalFormat(unsigned int format);
	void setImageFormat(unsigned int format);

	void generateTex(unsigned int width, unsigned int height, unsigned char* data);
	void bindTex() const;
	void clear();
private:
	unsigned int m_ID;
	unsigned int m_Width;
	unsigned int m_Height;
	unsigned int m_InternalFormat;
	unsigned int m_ImageFormat;
	unsigned int m_WrapS;
	unsigned int m_WrapT;
	unsigned int m_FilterMin;
	unsigned int m_FilterMax;
};


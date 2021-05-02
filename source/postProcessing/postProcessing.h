#pragma once

#include <glad/glad.h>
#include <iostream>
#include "../shaderHandler/shaderHandler.h"
#include "../textureHandler/textureHandler.h"

class PostProcessing
{
public:
	PostProcessing(ShaderHandler& shaderHandler, unsigned int width, unsigned int height)
		:m_pShaderHandler(&shaderHandler)
		,m_width(width)
		,m_height(height)
	{
		/* first creating a renderbuffer object and attaching it to multisample framebuffer object*/
		glGenFramebuffers(1, &MSFBO);
		glGenFramebuffers(1, &RFBO);
		glGenRenderbuffers(1, &MSRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, MSRBO);

		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, MSRBO);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Failed to initialize multisampled frame buffer" << std::endl;
		}
		/* end of attahing the renderframebuffer to MSFBO*/

		/*create the regular framebuffer object and attach the texture to it*/
		glBindFramebuffer(GL_FRAMEBUFFER, RFBO);
		m_textureHandler.generateTex(m_width, m_height, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureHandler.m_ID, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Failed to initialize multisampled frame buffer" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		initRenderData();
		m_pShaderHandler->setInt("scene", 0, true);
		float offset = 1.0f / 300.0f;
		float offsets[9][2] = {
			{ -offset,  offset  },
			{  0.0f,    offset  },
			{  offset,  offset  },
			{ -offset,  0.0f    },
			{  0.0f,    0.0f    },
			{  offset,  0.0f    },
			{ -offset, -offset  },
			{  0.0f,   -offset  },
			{  offset, -offset  }
		};
		glUniform2fv(glGetUniformLocation(m_pShaderHandler->m_shaderId, "offsets"), 9, (float*)offsets);
		int edgekernel[9] = {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
		};
		glUniform1iv(glGetUniformLocation(m_pShaderHandler->m_shaderId, "kernal"), 9, edgekernel);
		float blurkernel[9] = {
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
		};
		glUniform1fv(glGetUniformLocation(m_pShaderHandler->m_shaderId, "blur"), 9, blurkernel);
	}


	void renderPostProcessing(float time) {
		m_pShaderHandler->useShader();
		m_pShaderHandler->setFloat("time", time);
		m_pShaderHandler->setInt("chaos", m_chaos);
		m_pShaderHandler->setInt("confuse", m_confuse);
		m_pShaderHandler->setInt("shake", m_shake);

		glActiveTexture(GL_TEXTURE0);
		m_textureHandler.bindTex();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void beginRenderer() {
		glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void endRenderer() {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, MSFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, RFBO);
		glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	bool m_chaos = false;
	bool m_confuse = false;
	bool m_shake = false;
private:
	ShaderHandler* m_pShaderHandler = nullptr;
	TextureHandler m_textureHandler;
	unsigned int m_width;
	unsigned int m_height;

	unsigned int MSFBO; // multisampled frame buffer object
	unsigned int RFBO;  // regular frame buffer object
	unsigned int MSRBO; // multisampled render buffer object
	unsigned int VAO;

	void initRenderData() {
		unsigned int VBO;
		float vertices[] = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f,

			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindVertexArray(VAO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

};


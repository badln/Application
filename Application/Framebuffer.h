#pragma once
#include "main.h"
#include "global.h";
#include "Texture.h"

enum FramebufferMode {
	READWRITE = GL_FRAMEBUFFER,
	READ      = GL_READ_FRAMEBUFFER,
	WRITE     = GL_DRAW_FRAMEBUFFER
};


class Framebuffer {
private:
	bool created = false;
	unsigned int fBufferObj;
	unsigned int rBufferObj;
	DebugLogger fbLog;
	FramebufferMode _fbMode = FramebufferMode::READWRITE;
public:

	Texture colourBuffer;
	Texture depthBuffer; 
	Texture stencilBuffer;

	FramebufferMode fbMode() { return _fbMode; }
	static Framebuffer* Default;
	static unsigned int boundFB;

	/// <summary>
	/// Returns the assigned fBufferObj.
	/// </summary>
	/// <returns>fBufferObj</returns>
	const unsigned int& ID() const { return fBufferObj; }
	
	Framebuffer(FramebufferMode fbMode = FramebufferMode::READWRITE, Vector2 size = KTYGlobal::renderSize, GLenum colourBuffer_internalformat = GL_RGBA) {
		Create(fbMode, size, colourBuffer_internalformat);
	}

	/// <summary>
	/// Creates framebuffer. Other functions will not run unless the framebuffer has been created.
	/// </summary>
	/// <param name="fbMode = FramebufferMode::READWRITE"></param>
	/// <param name="size = KTYGlobal::renderSize"></param>
	/// <param name="colourBuffer_internalformat = GL_RGBA"></param>
	void Create(FramebufferMode fbMode = FramebufferMode::READWRITE, Vector2 size = KTYGlobal::renderSize, GLenum colourBuffer_internalformat = GL_RGBA) {
		if (created)
			return;
		created = true;
		fbLog.LoggerName = (const char*)"FRAMEBUFFER";
		glGenFramebuffers(1, &fBufferObj);
		glBindFramebuffer(fbMode, fBufferObj);
		_fbMode = fbMode;

		//TODO : Properly understand stencil buffer

		//Max render res is 200%
		size = Vector2(Math::clamp(size.x, (double)1, KTYGlobal::windowSize.x * 2), Math::clamp(size.y, (double)1, KTYGlobal::windowSize.y * 2));

		colourBuffer.CreateTexture(size, GL_RGBA, colourBuffer_internalformat, GL_UNSIGNED_BYTE, false);
		depthBuffer.CreateTexture(size, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, false);
		//stencilBuffer.CreateTexture(size, GL_STENCIL_INDEX, GL_STENCIL_INDEX8, GL_UNSIGNED_INT);

		AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, &colourBuffer);
		AttachTexture(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, &depthBuffer);
		//AttachTexture(GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, &stencilBuffer);

	}
	/// <summary>
	/// Attach a texture given an attachment type.
	/// Attachment types are 
	/// GL_COLOR_ATTACHMENT0,
	/// GL_DEPTH_ATTACHMENT,
	/// GL_STENCIL_ATTACHMENT.
	/// Note: For the Depth Attachment, the texture's format and internalformat should become GL_DEPTH_COMPONENT.
	/// For the Stencil Attachment, the format must be GL_STENCIL_INDEX.
	/// </summary>
	/// <param name="attachment"></param>
	/// <param name="textarget"></param>
	/// <param name="textureID"></param>
	/// <param name="renderBufferFormat"></param>
	/// <param name="mipmapLevel = 0"></param>
	/// <param name="zoffset = 0"></param>
	/// <summary>
	void AttachTexture(GLenum attachment, GLenum textarget, Texture* texture, GLint mipmapLevel = 0, GLint zoffset = 0) {
		AttachTextureID(attachment, textarget, texture->ID(), mipmapLevel, zoffset);
	}
	/// Attach a texture given an attachment type.
	/// Attachment types are 
	/// GL_COLOR_ATTACHMENT0,
	/// GL_DEPTH_ATTACHMENT,
	/// GL_STENCIL_ATTACHMENT.
	/// Note: For the Depth Attachment, the texture's format and internalformat should become GL_DEPTH_COMPONENT.
	/// For the Stencil Attachment, the format must be GL_STENCIL_INDEX.
	/// </summary>
	/// <param name="attachment"></param>
	/// <param name="textarget"></param>
	/// <param name="textureID"></param>
	/// <param name="renderBufferFormat"></param>
	/// <param name="mipmapLevel = 0"></param>
	/// <param name="zoffset = 0"></param>
	void AttachTextureID(GLenum attachment, GLenum textarget, GLint textureID, GLint mipmapLevel = 0, GLint zoffset = 0) {

		switch (textarget) {
		case GL_TEXTURE_1D:
			glFramebufferTexture1D(_fbMode, attachment, textarget, textureID, mipmapLevel);
			break;
		case GL_TEXTURE_2D:
			glFramebufferTexture2D(_fbMode, attachment, textarget, textureID, mipmapLevel);
			break;
		case GL_TEXTURE_3D:
			glFramebufferTexture3D(_fbMode, attachment, textarget, textureID, mipmapLevel, zoffset);
			break;
		}
	}

	/// <summary>
	/// fbMode types are:
	/// GL_FRAMEBUFFER,
	/// GL_READ_FRAMEBUFFER,
	/// GL_DRAW_FRAMEBUFFER.
	/// The framebuffer bound to GL_READ_FRAMEBUFFER will be used for functions such as glReadPixels.
	/// </summary>
	/// <param name="fbMode"></param>
	void Use(FramebufferMode fbMode = FramebufferMode::READWRITE) {
		_fbMode = fbMode;
		if (!created) {
			fbLog.Error("Framebuffer not created!");
		}
		glBindFramebuffer(_fbMode, fBufferObj);
		if (glCheckFramebufferStatus(_fbMode) != GL_FRAMEBUFFER_COMPLETE) {
			
			std::ostringstream fbAddress;
			fbAddress << &fBufferObj;
			std::string address = fbAddress.str();
			
			fbLog.Error("GL_FRAMEBUFFER_COMPLETE false! Framebuffer @ '" + address + "' is incomplete. Attach something using fb.AttachTexture()");
			switch (glCheckFramebufferStatus(_fbMode)) {
			case GL_FRAMEBUFFER_UNDEFINED:
				fbLog.Error("FRAMEBUFFER BINDING ERROR: GL_FRAMEBUFFER_UNDEFINED");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				fbLog.Error("FRAMEBUFFER BINDING ERROR: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				fbLog.Error("FRAMEBUFFER BINDING ERROR: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				fbLog.Error("FRAMEBUFFER BINDING ERROR: GL_FRAMEBUFFER_UNSUPPORTED");
				break;
			default:
				fbLog.Error("FRAMEBUFFER BINDING ERROR: UNKNOWN");
				break;
			}
			glBindFramebuffer(_fbMode, 0);
		}
	}
	/// <summary>
	/// Delete this framebuffer.
	/// </summary>
	void Delete() {
		if (!created)
			return;
		created = false;
		glDeleteFramebuffers(1, &fBufferObj);
	}
};
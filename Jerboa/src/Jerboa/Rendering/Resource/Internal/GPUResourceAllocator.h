#pragma once

#include "Jerboa/Core/Enum.h"
#include "Jerboa/Core/Singleton.h"
#include "Jerboa/Rendering/Resource/IndexBuffer.h"
#include "Jerboa/Rendering/Resource/VertexBuffer.h"
#include "Jerboa/Rendering/Resource/Config/FrameBufferConfig.h"
#include "Jerboa/Rendering/Resource/Config/TextureConfig.h"
#include "Jerboa/Rendering/Resource/Internal/GPUResource.h"
#include "Jerboa/Resource/Data/TextureData.h"
#include "Jerboa/Resource/Data/ShaderDataGLSL.h"

#include <array>

namespace Jerboa
{
	struct FrameBufferGPUResourceConfig
	{
		std::array<GPUResource*, EnumToInt<int>(ColorAttachmentSlot::Count)> m_ColorAttachments = { nullptr };
		std::array<TextureUsage, EnumToInt<int>(ColorAttachmentSlot::Count)> m_ColorAttachmentsUsage = { TextureUsage::None };
		
		GPUResource* m_DepthAttachment = nullptr;
		TextureUsage m_DepthAttachmentUsage = TextureUsage::None;

		GPUResource* m_StencilAttachment = nullptr;
		TextureUsage m_StencilAttachmentUsage = TextureUsage::None;
	};

	struct GPUTextureResourceData
	{
		uint32			m_Width = 0;
		uint32			m_Height = 0;
		PixelFormat		m_PixelFormat = PixelFormat::None;
		bool			m_GenerateMipmaps = false;
		unsigned char*	m_PixelData = nullptr;
	};

	class GPUResourceAllocator : Singleton<GPUResourceAllocator>
	{
	public:
		virtual GPUResource	CreateVertexBuffer(const VertexBufferData& bufferData) const = 0;
		virtual GPUResource CreateVertexArrayObject() const; // Needed for OpenGL
		virtual GPUResource	CreateIndexBuffer(const IndexBufferData& bufferData) const = 0;
		virtual GPUResource	CreateTexture(const TextureConfig& config) const = 0;
		virtual GPUResource	CreateShader(const ShaderDataGLSL& shaderData) const = 0;
		virtual GPUResource CreateFrameBuffer(const FrameBufferGPUResourceConfig& config) const = 0;
		
		virtual void UploadTextureData(GPUResource& texture, const GPUTextureResourceData& data) const = 0;
	};
}

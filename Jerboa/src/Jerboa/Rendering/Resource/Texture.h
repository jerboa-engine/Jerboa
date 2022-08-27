#pragma once

#include "Jerboa/Resource/Data/TextureData.h"
#include "Jerboa/Rendering/Resource/Internal/GPUResource.h"
#include "Jerboa/Rendering/Resource/Config/TextureConfig.h"

#include "Jerboa/Core/Types.h"

#include <string>
#include <memory>

namespace Jerboa 
{
	class GPUResourceAllocator;

	class Texture2D
	{
	public:
		~Texture2D();
		void Create(const TextureConfig& data, GPUResourceAllocator& allocator);
		void CreateFromTextureData(const TextureData& data, TextureUsage usage, const GPUResourceAllocator& allocator);
		void UploadTextureData(const TextureData& data, const GPUResourceAllocator& allocator);

		const GPUResource&	GetGPUResouce() const { return m_TextureGPUResource; }
		uint32				GetWidth() const { return m_Width; }
		uint32				GetHeight() const { return m_Height; };
		PixelFormat			GetPixelFormat() const { return m_PixelFormat; }
		TextureUsage		GetUsage() const { m_Usage; }

	private:
		GPUResource m_TextureGPUResource;
		uint32		m_Width = 0;
		uint32		m_Height = 0;
		PixelFormat m_PixelFormat = PixelFormat::None;
		TextureUsage m_Usage = TextureUsage::None;
	};
}



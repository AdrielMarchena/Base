#include "TextureSpecifications.h"

#include "utils/base_assert.h"
#include "glad/glad.h"

namespace Base
{
	uint32_t GLSwitch::TextureFilter(GL_TextureFilter filter_type)
	{
		switch (filter_type)
		{
		case GL_TextureFilter::LINEAR:
			return GL_LINEAR;
		case GL_TextureFilter::NEAREST:
			return GL_NEAREST;
		case GL_TextureFilter::LINEAR_MIPMAP_LINEAR:
			return GL_LINEAR_MIPMAP_LINEAR;
		case GL_TextureFilter::NEAREST_MIPMAP_NEAREST:
			return GL_NEAREST_MIPMAP_NEAREST;
		}
		return 0;
	}
	uint32_t GLSwitch::TextureWrap(GL_TextureWrap wrap_type)
	{
		switch (wrap_type)
		{
		case GL_TextureWrap::REPEAT:
			return GL_REPEAT;
		case GL_TextureWrap::MIRROR_REPEAT:
			return GL_MIRRORED_REPEAT;
		case GL_TextureWrap::CLAMP_EDGE:
			return GL_CLAMP_TO_EDGE;
		case GL_TextureWrap::CLAMP_BORDER:
			return GL_CLAMP_TO_BORDER;
		default:
			BASE_CORE_ASSERT(false, "Unknow Texture Wrap");
			break;
		}
		return 0;
	}
	uint32_t GLSwitch::TextureTarget(GL_TextureTarget wrap_type)
	{
		switch (wrap_type)
		{
		case GL_TextureTarget::TEXTURE_2D:
			return GL_TEXTURE_2D;
		case GL_TextureTarget::TEXTURE_3D:
			return GL_TEXTURE_3D;
		default:
			BASE_CORE_ASSERT(false, "Unknow Texture Target");
			break;
		}
		return 0;
	}
}
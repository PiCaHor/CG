#include "TRTexture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

namespace TinyRenderer
{
	//----------------------------------------------TRTexture2D----------------------------------------------

	TRTexture2D::TRTexture2D() :
		m_width(0), m_height(0), m_channel(0), m_pixels(nullptr),
		m_warp_mode(TRTextureWarpMode::TR_REPEAT),
		m_filtering_mode(TRTextureFilterMode::TR_NEAREST) {}

	TRTexture2D::~TRTexture2D() { freeLoadedImage(); }

	void TRTexture2D::setWarpingMode(TRTextureWarpMode mode) { m_warp_mode = mode; }

	void TRTexture2D::setFilteringMode(TRTextureFilterMode mode) { m_filtering_mode = mode; }

	bool TRTexture2D::loadTextureFromFile(
		const std::string &filepath,
		TRTextureWarpMode warpMode,
		TRTextureFilterMode filterMode)
	{
		freeLoadedImage();

		m_warp_mode = warpMode;
		m_filtering_mode = filterMode;

		//Load image from given file using stb_image.h
		//Refs: https://github.com/nothings/stb
		{
			stbi_set_flip_vertically_on_load(true);
			m_pixels = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channel, 0);
		}

		if (m_pixels == nullptr)
		{
			std::cerr << "Failed to load image from " << filepath << std::endl;
			exit(1);
		}

		return true;
	}

	void TRTexture2D::readPixel(int u, int v, unsigned char &r, unsigned char &g, unsigned char &b, unsigned char &a) const
	{
		//Handling out of range situation
		{
			if (u < 0 || u >= m_width)
			{
				switch (m_warp_mode)
				{
				case TRTextureWarpMode::TR_REPEAT:
					u = u > 0 ? (u % m_width) : (m_width - 1 + u % m_width);
					break;
				case TRTextureWarpMode::TR_CLAMP_TO_EDGE:
					u = (u < 0) ? 0 : m_width - 1;
					break;
				default:
					u = (u < 0) ? 0 : m_width - 1;
					break;
				}
			}

			if (v < 0 || v >= m_height)
			{
				switch (m_warp_mode)
				{
				case TRTextureWarpMode::TR_REPEAT:
					v = v > 0 ? (v % m_height) : (m_height - 1 + v % m_height);
					break;
				case TRTextureWarpMode::TR_CLAMP_TO_EDGE:
					v = (v < 0) ? 0 : m_height - 1;
					break;
				default:
					v = (v < 0) ? 0 : m_height - 1;
					break;
				}
			}
		}

		int index = (v * m_height + u) * m_channel;
		r = m_pixels[index + 0];
		g = m_pixels[index + 1];
		b = m_pixels[index + 2];
		a = (m_channel >= 4) ? m_pixels[index + 3] : a;

		return;
	}

	void TRTexture2D::freeLoadedImage()
	{
		if (m_pixels != nullptr)
		{
			stbi_image_free(m_pixels);
		}

		m_pixels = nullptr;
		m_width = m_height = m_channel = 0;
	}

	glm::vec4 TRTexture2D::sample(const glm::vec2 &uv) const
	{
		//Perform sampling procedure
		//Note: return texel that ranges from 0.0f to 1.0f instead of [0,255]
		glm::vec4 texel(1.0f);
		switch (m_filtering_mode)
		{
		case TRTextureFilterMode::TR_NEAREST:
			texel = TRTexture2DSampler::textureSampling_nearest(*this, uv);
			break;
		case TRTextureFilterMode::TR_LINEAR:
			texel = TRTexture2DSampler::textureSampling_bilinear(*this, uv);
			break;
		default:
			break;
		}

		return texel;
	}

	//----------------------------------------------TRTexture2DSampler----------------------------------------------

	glm::vec4 TRTexture2DSampler::textureSampling_nearest(const TRTexture2D &texture, glm::vec2 uv)
	{
		unsigned char r = 255, g = 255, b = 255, a = 255;

		//Task1: Implement nearest sampling algorithm for texture sampling
		// Note: You should use texture.readPixel() to read the pixel, and for instance, 
		//       use texture.readPixel(25,35,r,g,b,a) to read the pixel in (25, 35).
		//       But before that, you need to map uv from [0,1]*[0,1] to [0,width-1]*[0,height-1].
		int x, y; 
		{
			x = (int)(uv.x * texture.m_width + 0.5); 
			y = (int)(uv.y * texture.m_height + 0.5); 
			texture.readPixel(x, y, r, g, b, a); 
		}

		constexpr float denom = 1.0f / 255.0f;
		return glm::vec4(r, g, b, a) * denom;
	}

	glm::vec4 TRTexture2DSampler::textureSampling_bilinear(const TRTexture2D &texture, glm::vec2 uv)
	{
		//Note: Delete this line when you try to implement Task 4. 
		//return textureSampling_nearest(texture, uv);

		//Task4: Implement bilinear sampling algorithm for texture sampling
		// Note: You should use texture.readPixel() to read the pixel, and for instance, 
		//       use texture.readPixel(25,35,r,g,b,a) to read the pixel in (25, 35).


		unsigned char r = 255, g = 255, b = 255, a = 255;
		unsigned char rx1 = 255, gx1 = 255, bx1 = 255, ax1 = 255;
		unsigned char rx2 = 255, gx2 = 255, bx2 = 255, ax2 = 255; 
		unsigned char rt = 255, gt = 255, bt = 255, at = 255;
		double realx, realy; 
		int x, y;
		{
			realx = (uv.x * texture.m_width);
			realy = (uv.y * texture.m_height);
			x = (int)realx; 
			y = (int)realy;
			texture.readPixel(x, y, rt, gt, bt, at);
			rx1 = (realx - x) * rt; gx1 = (realx - x) * gt; bx1 = (realx - x) * bt;ax1 = (realx - x) * at; 
			texture.readPixel(x+1, y, rt, gt, bt, at);
			rx1 += (1 - realx + x) * rt; gx1 += (1 - realx + x) * gt; bx1 += (1 - realx + x) * bt; ax1 += (1 - realx + x) * at;

			texture.readPixel(x, y + 1, rt, gt, bt, at); 
			rx2 = (realx - x) * rt; gx2 = (realx - x) * gt; bx2 = (realx - x) * bt; ax2 = (realx - x) * at;
			texture.readPixel(x + 1, y + 1, rt, gt, bt, at); 
			rx2 += (1 - realx + x) * rt; gx2 += (1 - realx + x) * gt; bx2 += (1 - realx + x) * bt; ax2 += (1 - realx + x) * at;

			r = (realy - y) * rx1 + (1 - realy + y) * rx2; 
			g = (realy - y) * gx1 + (1 - realy + y) * gx2;
			b = (realy - y) * bx1 + (1 - realy + y) * bx2;
			a = (realy - y) * ax1 + (1 - realy + y) * ax2;
		}

		constexpr float denom = 1.0f / 255.0f;
		return glm::vec4(r, g, b, a) * denom;
	}
}
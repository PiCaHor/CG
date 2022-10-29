#include "TRShaderPipeline.h"

#include <algorithm>

namespace TinyRenderer
{
	//----------------------------------------------VertexData----------------------------------------------

	TRShaderPipeline::VertexData TRShaderPipeline::VertexData::lerp(
		const TRShaderPipeline::VertexData &v0,
		const TRShaderPipeline::VertexData &v1,
		float frac)
	{
		//Linear interpolation
		VertexData result;
		result.pos = (1.0f - frac) * v0.pos + frac * v1.pos;
		result.col = (1.0f - frac) * v0.col + frac * v1.col;
		result.nor = (1.0f - frac) * v0.nor + frac * v1.nor;
		result.tex = (1.0f - frac) * v0.tex + frac * v1.tex;
		result.cpos = (1.0f - frac) * v0.cpos + frac * v1.cpos;
		result.spos.x = (1.0f - frac) * v0.spos.x + frac * v1.spos.x;
		result.spos.y = (1.0f - frac) * v0.spos.y + frac * v1.spos.y;

		return result;
	}

	TRShaderPipeline::VertexData TRShaderPipeline::VertexData::barycentricLerp(
		const VertexData &v0, 
		const VertexData &v1, 
		const VertexData &v2,
		glm::vec3 w)
	{
		VertexData result;
		result.pos = w.x * v0.pos + w.y * v1.pos + w.z * v2.pos;
		result.col = w.x * v0.col + w.y * v1.col + w.z * v2.col;
		result.nor = w.x * v0.nor + w.y * v1.nor + w.z * v2.nor;
		result.tex = w.x * v0.tex + w.y * v1.tex + w.z * v2.tex;
		result.cpos = w.x * v0.cpos + w.y * v1.cpos + w.z * v2.cpos;
		result.spos.x = w.x * v0.spos.x + w.y * v1.spos.x + w.z * v2.spos.x;
		result.spos.y = w.x * v0.spos.y + w.y * v1.spos.y + w.z * v2.spos.y;

		return result;
	}

	void TRShaderPipeline::VertexData::prePerspCorrection(VertexData &v)
	{
		//Perspective correction: the world space properties should be multipy by 1/w before rasterization
		//https://zhuanlan.zhihu.com/p/144331875
		//We use pos.w to store 1/w
		v.pos.w = 1.0f / v.cpos.w;
		v.pos = glm::vec4(v.pos.x * v.pos.w, v.pos.y * v.pos.w, v.pos.z * v.pos.w, v.pos.w);
		v.tex = v.tex * v.pos.w;
		v.nor = v.nor * v.pos.w;
		v.col = v.col * v.pos.w;
	}

	void TRShaderPipeline::VertexData::aftPrespCorrection(VertexData &v)
	{
		//Perspective correction: the world space properties should be multipy by w after rasterization
		//https://zhuanlan.zhihu.com/p/144331875
		//We use pos.w to store 1/w
		float w = 1.0f / v.pos.w;
		v.pos = v.pos * w;
		v.tex = v.tex * w;
		v.nor = v.nor * w;
		v.col = v.col * w;
	}

	//----------------------------------------------TRShaderPipeline----------------------------------------------


	void TRShaderPipeline::rasterize_fill_edge_function(
		const VertexData& v0,
		const VertexData& v1,
		const VertexData& v2,
		const unsigned int& screen_width,
		const unsigned int& screen_height,
		std::vector<VertexData>& rasterized_points)
	{
		int x0 = v0.spos.x, x1 = v1.spos.x, x2 = v2.spos.x;
		int y0 = v0.spos.y, y1 = v1.spos.y, y2 = v2.spos.y;

		int rdx = (x0 > x1) ? x0 : x1;
		rdx = x2 > rdx ? x2 : rdx;
		if (rdx > screen_width) rdx = screen_width; 
		int lpy = (y0 > y1) ? y0 : y1;
		lpy = y2 > lpy ? y2 : lpy;
		if (lpy > screen_height) lpy = screen_height; 
		int lpx = (x0 < x1) ? x0 : x1;
		lpx = x2 < lpx ? x2 : lpx;
		if (lpx < 0) lpy = 0; 
		int rdy = (y0 < y1) ? y0 : y1;
		rdy = y2 < rdy ? y2 : rdy;
		if (rdx < 0) rdx = 0; 
		for (int i = lpx; i <= rdx; i++) {
			for (int j = rdy; j <= lpy; j++) {
				glm::vec3 t(i, j, 0);
				glm::vec3 r1 = glm::cross(glm::vec3(t.x - x0, t.y - y0, 0), glm::vec3(v1.spos - v0.spos, 0));
				bool f1 = r1.z >= 0;
				glm::vec3 r2 = glm::cross(glm::vec3(t.x - x1, t.y - y1, 0), glm::vec3(v2.spos - v1.spos, 0));
				bool f2 = r2.z >= 0;
				glm::vec3 r3 = glm::cross(glm::vec3(t.x - x2, t.y - y2, 0), glm::vec3(v0.spos - v2.spos, 0));
				bool f3 = r3.z >= 0;
				if (f1 == f2 && f2 == f3)
				{
					glm::fvec3 sx(0, 0, 0);
					glm::fvec3 sy(0, 0, 0);
					sx.y = x2 - x0;
					sx.x = x1 - x0;
					sx.z = x0 - t.x;
					sy.y = y2 - y0;
					sy.x = y1 - y0;
					sy.z = y0 - t.y;
					glm::vec3 u = glm::cross(sx, sy);
					if (std::abs(u.z) > 1e-2)
					{
						glm::vec3 w(1.0f - (u.x + u.y) / u.z, u.x / u.z, u.y / u.z);
						auto mid = VertexData::barycentricLerp(v0, v1, v2, w);
						//mid.spos = glm::vec4(i, j, 0.0f, 1.0f);
						if (mid.spos.x >= 0 && mid.spos.x < screen_width && mid.spos.y >= 0 && mid.spos.y < screen_height)
						{
							rasterized_points.push_back(mid);
						}
					}
					else {
						glm::vec3 w(-1.0f, 1.0f, 1.0f);
						auto mid = VertexData::barycentricLerp(v0, v1, v2, w);
						//mid.spos = glm::vec4(i, j, 0.0f, 1.0f);
						if (mid.spos.x >= 0 && mid.spos.x < screen_width && mid.spos.y >= 0 && mid.spos.y < screen_height)
						{
							rasterized_points.push_back(mid);
						}
					}
				}

			}
		}
	}

	void TRShaderPipeline::rasterize_wire_aux(
		const VertexData &from,
		const VertexData &to,
		const unsigned int &screen_width,
		const unsigned int &screen_height,
		std::vector<VertexData> &rasterized_points)
	{

		//Task1: Implement Bresenham line rasterization
		// Note: You shold use VertexData::lerp(from, to, weight) for interpolation,
		//       interpolated points should be pushed back to rasterized_points.
		//       Interpolated points shold be discarded if they are outside the window. 
		
		//       from.spos and to.spos are the screen space vertices.

		int dx = to.spos.x - from.spos.x;
		int dy = to.spos.y - from.spos.y; 
		int stepX = 1, stepY = 1; 

		if (dx < 0)
		{
			stepX = -1; 
			dx = -dx; 
		}
		if (dy < 0)
		{
			stepY = -1; 
			dy = -dy; 
		}

		int d2x = 2 * dx, d2y = 2 * dy; 
		int d2y_minus_d2x = d2x - d2y; 
		int sx = from.spos.x; 
		int sy = from.spos.y; 

		VertexData tmp; 
		if (dy <= dx)
		{
			int flag = d2y - dx; 
			for (int i = 0; i <= dx; ++i)
			{
				tmp = VertexData::lerp(from, to, static_cast<double>(i) / dx);
				if (tmp.spos.x <= screen_width && tmp.spos.x >= 0 && tmp.spos.y <= screen_height && tmp.spos.y >= 0)
					rasterized_points.push_back(tmp); 
				sx += stepX; 
				if (flag <= 0)
					flag += d2y; 
				else
				{
					sy += stepY; 
					flag += d2y_minus_d2x; 
				}

			}
		}
		else
		{
			int flag = d2x - dy;
			for (int i = 0; i <= dy; ++i)
			{
				tmp = VertexData::lerp(from, to, static_cast<double>(i) / dy);
				if (tmp.spos.x <= screen_width && tmp.spos.x >= 0 && tmp.spos.y <= screen_height && tmp.spos.y >= 0)
					rasterized_points.push_back(tmp);
				sy += stepY;
				if (flag <= 0)
					flag += d2x;
				else
				{
					sx += stepX;
					flag -= d2y_minus_d2x;
				}

			}
		}


	}

	void TRDefaultShaderPipeline::vertexShader(VertexData &vertex)
	{
		//Local space -> World space -> Camera space -> Project space
		vertex.pos = m_model_matrix * glm::vec4(vertex.pos.x, vertex.pos.y, vertex.pos.z, 1.0f);
		vertex.cpos = m_view_project_matrix * vertex.pos;
	}

	void TRDefaultShaderPipeline::fragmentShader(const VertexData &data, glm::vec4 &fragColor)
	{
		//Just return the color.
		fragColor = glm::vec4(data.tex, 0.0, 1.0f);
	}
}
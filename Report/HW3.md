## Assignment3

#### Task1 

这个任务主要是做一个映射就可以，由于是0-1的范围，所以只需要乘上宽高即可。 

```cpp
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
```

实验结果就是把贴图映射上去，整体的视频结果在task2一起展示

#### Task2

Phong光照模型首先考虑的是衰减，依据教程，由于选择的是点光源，所以有一个衰减的公式，一个一次项，一个二次项，参数根据传入的进行计算。 

环境光就是一个衰减，直接计算光线颜色即可

漫反射的关键就是入射光线和发现的夹角，由于都是单位向量，所以直接点乘即可。然后只需要注意漫反射系数就ok了，然后注意不能小于0

高光部分的是反射光线与视线的夹角，然后注意不能小于0。 

```cpp
	void TRTextureShadingPipeline::fragmentShader(const VertexData &data, glm::vec4 &fragColor)
	{
		//Default color
		fragColor = glm::vec4(m_ke, 1.0f);

		if (m_diffuse_tex_id != -1)
		{
			fragColor = texture2D(m_diffuse_tex_id, data.tex);
		}
	}

	//----------------------------------------------TRPhongShadingPipeline----------------------------------------------

	void TRPhongShadingPipeline::fragmentShader(const VertexData &data, glm::vec4 &fragColor)
	{
		fragColor = glm::vec4(0.0f);

		//Fetch the corresponding color 
		glm::vec3 amb_color, dif_color, spe_color, glow_color;
		amb_color = dif_color = (m_diffuse_tex_id != -1) ? glm::vec3(texture2D(m_diffuse_tex_id, data.tex)) : m_kd;
		spe_color = (m_specular_tex_id != -1) ? glm::vec3(texture2D(m_specular_tex_id, data.tex)) : m_ks;
		glow_color = (m_glow_tex_id != -1) ? glm::vec3(texture2D(m_glow_tex_id, data.tex)) : m_ke;

		//No lighting
		if (!m_lighting_enable)
		{
			fragColor = glm::vec4(glow_color, 1.0f);
			return;
		}

		//Calculate the lighting
		glm::vec3 fragPos = glm::vec3(data.pos);
		glm::vec3 normal = glm::normalize(data.nor);
		glm::vec3 viewDir = glm::normalize(m_viewer_pos - fragPos);
		for (size_t i = 0; i < m_point_lights.size(); ++i)
		{
			const auto &light = m_point_lights[i];
			glm::vec3 lightDir = glm::normalize(light.lightPos - fragPos);

			glm::vec3 ambient, diffuse, specular;
			float attenuation = 1.0f;

			//Task2: Implement phong lighting algorithm
			// Note: The parameters you should use are described as follow:
			//        amb_color: the ambient color of the fragment
			//        dif_color: the diffuse color of the fragment
			//        spe_color: the specular color of the fragment
			//          fragPos: the fragment position in world space
			//           normal: the fragment normal in world space
			//          viewDir: viewing direction in world space
			//             m_kd: diffuse coefficient
			//         lightDir: lighting direction in world space
			// light.lightColor: the ambient, diffuse and specular color of light source
			//      m_shininess: specular hightlight exponent coefficient
			//   light.lightPos: the position of the light source
			//light.attenuation：the attenuation coefficients of the light source (x,y,z) -> (constant,linear,quadratic)
			//  you could use glm::pow(), glm::dot, glm::reflect(), glm::max(), glm::normalize(), glm::length() et al.
			
			{
				float dis = glm::length(light.lightPos - fragPos);
				attenuation = attenuation / (light.attenuation.x + light.attenuation.y * dis + light.attenuation.z * dis * dis);

				ambient = amb_color * light.lightColor;

				diffuse = dif_color * glm::max(0.0f, glm::dot(normal, lightDir)) * m_kd * light.lightColor;
				
				//Phong
				glm::vec3 half = glm::normalize(glm::reflect(lightDir, normal));
				specular = spe_color * glm::pow(glm::max(0.0f, glm::dot(normal, half)), m_shininess) * light.lightColor;

				//Blinn-Phong
				//glm::vec3 half = glm::normalize(lightDir + viewDir);
				//specular = spe_color * glm::pow(glm::max(0.0f, glm::dot(normal, half)), m_shininess) * light.lightColor;
			}
			fragColor.x += (ambient.x + diffuse.x + specular.x) * attenuation;
			fragColor.y += (ambient.y + diffuse.y + specular.y) * attenuation;
			fragColor.z += (ambient.z + diffuse.z + specular.z) * attenuation;
		}

		fragColor = glm::vec4(fragColor.x + glow_color.x, fragColor.y + glow_color.y, fragColor.z + glow_color.z, 1.0f);

		//Tone mapping: HDR -> LDR
		//Refs: https://learnopengl.com/Advanced-Lighting/HDR
		{
			glm::vec3 hdrColor(fragColor);
			fragColor.x = 1.0f - glm::exp(-hdrColor.x * 2.0f);
			fragColor.y = 1.0f - glm::exp(-hdrColor.y * 2.0f);
			fragColor.z = 1.0f - glm::exp(-hdrColor.z * 2.0f);
		}
	}
```

#### Task3：

Blinn-Phong的关键就是高光，变成了视线与入射光线的半向量，由于归一化的函数，直接相加即可。 

```cpp
	void TRTextureShadingPipeline::fragmentShader(const VertexData &data, glm::vec4 &fragColor)
	{
		//Default color
		fragColor = glm::vec4(m_ke, 1.0f);

		if (m_diffuse_tex_id != -1)
		{
			fragColor = texture2D(m_diffuse_tex_id, data.tex);
		}
	}

	//----------------------------------------------TRPhongShadingPipeline----------------------------------------------

	void TRPhongShadingPipeline::fragmentShader(const VertexData &data, glm::vec4 &fragColor)
	{
		fragColor = glm::vec4(0.0f);

		//Fetch the corresponding color 
		glm::vec3 amb_color, dif_color, spe_color, glow_color;
		amb_color = dif_color = (m_diffuse_tex_id != -1) ? glm::vec3(texture2D(m_diffuse_tex_id, data.tex)) : m_kd;
		spe_color = (m_specular_tex_id != -1) ? glm::vec3(texture2D(m_specular_tex_id, data.tex)) : m_ks;
		glow_color = (m_glow_tex_id != -1) ? glm::vec3(texture2D(m_glow_tex_id, data.tex)) : m_ke;

		//No lighting
		if (!m_lighting_enable)
		{
			fragColor = glm::vec4(glow_color, 1.0f);
			return;
		}

		//Calculate the lighting
		glm::vec3 fragPos = glm::vec3(data.pos);
		glm::vec3 normal = glm::normalize(data.nor);
		glm::vec3 viewDir = glm::normalize(m_viewer_pos - fragPos);
		for (size_t i = 0; i < m_point_lights.size(); ++i)
		{
			const auto &light = m_point_lights[i];
			glm::vec3 lightDir = glm::normalize(light.lightPos - fragPos);

			glm::vec3 ambient, diffuse, specular;
			float attenuation = 1.0f;

			//Task2: Implement phong lighting algorithm
			// Note: The parameters you should use are described as follow:
			//        amb_color: the ambient color of the fragment
			//        dif_color: the diffuse color of the fragment
			//        spe_color: the specular color of the fragment
			//          fragPos: the fragment position in world space
			//           normal: the fragment normal in world space
			//          viewDir: viewing direction in world space
			//             m_kd: diffuse coefficient
			//         lightDir: lighting direction in world space
			// light.lightColor: the ambient, diffuse and specular color of light source
			//      m_shininess: specular hightlight exponent coefficient
			//   light.lightPos: the position of the light source
			//light.attenuation：the attenuation coefficients of the light source (x,y,z) -> (constant,linear,quadratic)
			//  you could use glm::pow(), glm::dot, glm::reflect(), glm::max(), glm::normalize(), glm::length() et al.
			
			{
				float dis = glm::length(light.lightPos - fragPos);
				attenuation = attenuation / (light.attenuation.x + light.attenuation.y * dis + light.attenuation.z * dis * dis);

				ambient = amb_color * light.lightColor;

				diffuse = dif_color * glm::max(0.0f, glm::dot(normal, lightDir)) * m_kd * light.lightColor;
				
				//Phong
				//glm::vec3 half = glm::normalize(glm::reflect(lightDir, normal));
				//specular = spe_color * glm::pow(glm::max(0.0f, glm::dot(normal, half)), m_shininess) * light.lightColor;

				//Blinn-Phong
				glm::vec3 half = glm::normalize(lightDir + viewDir);
				specular = spe_color * glm::pow(glm::max(0.0f, glm::dot(normal, half)), m_shininess) * light.lightColor;
			}
			fragColor.x += (ambient.x + diffuse.x + specular.x) * attenuation;
			fragColor.y += (ambient.y + diffuse.y + specular.y) * attenuation;
			fragColor.z += (ambient.z + diffuse.z + specular.z) * attenuation;
		}

		fragColor = glm::vec4(fragColor.x + glow_color.x, fragColor.y + glow_color.y, fragColor.z + glow_color.z, 1.0f);

		//Tone mapping: HDR -> LDR
		//Refs: https://learnopengl.com/Advanced-Lighting/HDR
		{
			glm::vec3 hdrColor(fragColor);
			fragColor.x = 1.0f - glm::exp(-hdrColor.x * 2.0f);
			fragColor.y = 1.0f - glm::exp(-hdrColor.y * 2.0f);
			fragColor.z = 1.0f - glm::exp(-hdrColor.z * 2.0f);
		}
	}
```

效果的差别就是边缘的高光变化更加的平缓

#### Task4

双线性插值的部分就是先对一个方向进行插值，然后再根据另一边的参数进行插值，难度不大

```cpp
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
```


为了控制视频大小，前四个内容的视频会合在一起

Task1:

这部分根据教程的公式推导进行编写即可，主要是要注意边界的控制

代码如下

```cpp
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
```

Task2：

这一部分的就是做一个测试，由于题目的限制，只要有一个点在，就加入考虑。

代码如下：

```cpp
		int cnt = 3; 
		if (v0.cpos.w < m_frustum_near_far.y && v0.cpos.w > m_frustum_near_far.x)
			if (abs(v0.cpos.w) > abs(v0.cpos.x) && abs(v0.cpos.w) > abs(v0.cpos.y) && abs(v0.cpos.w) > abs(v0.cpos.z))
				cnt--; 

		if (v1.cpos.w < m_frustum_near_far.y && v1.cpos.w > m_frustum_near_far.x)
			if (abs(v1.cpos.w) > abs(v1.cpos.x) && abs(v1.cpos.w) > abs(v1.cpos.y) && abs(v1.cpos.w) > abs(v1.cpos.z))
				cnt--;

		if (v2.cpos.w < m_frustum_near_far.y && v2.cpos.w > m_frustum_near_far.x)
			if (abs(v2.cpos.w) > abs(v2.cpos.x) && abs(v2.cpos.w) > abs(v2.cpos.y) && abs(v2.cpos.w) > abs(v2.cpos.z))
				cnt--;
		if (cnt == 3)
			return {}; 
		return { v0, v1, v2 };
```

Task3：

这部分就是叉乘，然后根据对应关系，和法向量点乘判断是不是通向，是同向的话就是背侧。

代码如下：

```cpp
		glm::vec3 x1 = (v1 - v0);
		glm::vec3 x2 = (v2 - v1);
		glm::vec3 n1 = glm::normalize(glm::cross(x1, x2));
		glm::vec3 view = glm::vec3(0, 0, -1);
		if (glm::dot(n1, view) > 0)
			return true; 
		return false;
```

Task4：

这一部分内容比较多，首先是画包围盒，这里直接进行边界判定，然后就是依次访问这个包围盒里的每一个像素，用三次to_left 操作判断是不是在三角形内，然后对于在三角形内的点用叉乘求得中心坐标，然后插值得到对应的点

代码如下：

```cpp
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
```

前四个部分的实验效果见视频1，对比结果，背面剔除的效果可以加快渲染，同时使一些看不见的三角形正常消失，可以看到如果放大的话，随着旋转，#CulledFaces不断变化，当复杂面转过去的时候就会变得很大。 

然后在拉伸镜头的过程中，如果拉的非常大，#clipedfaces会变得很大，此前由于没有添加边界，会直接卡住，后面加上边界就正常了

Task5：

加入一行判断即可。 效果见视频2

```cpp
if (m_backBuffer->readDepth(points.spos.x, points.spos.y) < points.cpos.z) continue;
```

实现成功

Task6：

体会与感悟主要是集中在task4，前面三个任务和task5进行的还是比较轻松的，在教程的帮助下都比较容易的解决了，但是在task4中，特别是重心坐标推算的部分，开始时由于想直接写导出公式，导致整个内容实现起来非常的冗杂。




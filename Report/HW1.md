## Assignment 1

**Task 1**、实现观察矩阵（View Matrix）的计算

观察矩阵如图：

![1](C:\Users\Administrator\Desktop\2\1.png)

代码：

```
	glm::mat4 TRRenderer::calcViewMatrix(glm::vec3 camera, glm::vec3 target, glm::vec3 worldUp)
	{
		//Setup view matrix (world space -> camera space)
		glm::mat4 vMat = glm::mat4(1.0f);

		//Task 1: Implement the calculation of view matrix, and then set it to vMat
		//  Note: You can use any glm function (such as glm::normalize, glm::cross, glm::dot) except glm::lookAt

		glm::vec3 cameraDir = glm::normalize(camera - target); 
		glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDir, worldUp)); // There have some problem maybe
		glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDir, cameraRight)); 

		

		//set translation matrix
		vMat[3][0] = -camera.x; 
		vMat[3][1] = -camera.y;
		vMat[3][2] = -camera.z;

		//set rotation matrix
		glm::mat4 rMat = glm::mat4(1.0f); 
		rMat[0][0] = cameraRight.x; rMat[1][0] = cameraRight.y; rMat[2][0] = cameraRight.z; rMat[3][0] = 0;
		rMat[0][1] = cameraUp.x; rMat[1][1] = cameraUp.y; rMat[2][1] = cameraUp.z; rMat[3][1] = 0;
		rMat[0][2] = cameraDir.x; rMat[1][2] = cameraDir.y; rMat[2][2] = cameraDir.z; rMat[3][2] = 0;
		rMat[0][3] = 0; rMat[1][3] = 0; rMat[2][3] = 0; rMat[3][3] = 1.0f;

		//get LookAt Matrix
		vMat = rMat * vMat; 

		return vMat;
	}
```

分析：

先通过叉乘获得相机的三位左边，这里同库函数有一点点的不一样，这里的Right是z轴叉up，但是出来的是倒立的图像，通过老师ppt和换算这里反一下。 

**Task 2**、实现透视投影矩阵（Project Matrix）的计算

透视矩阵如图：

![2](C:\Users\Administrator\Desktop\2\2.png)

![3](C:\Users\Administrator\Desktop\2\3.png)

代码：

```cpp
	glm::mat4 TRRenderer::calcPerspProjectMatrix(float fovy, float aspect, float near, float far)
	{
		//Setup perspective matrix (camera space -> clip space)
		glm::mat4 pMat = glm::mat4(1.0f);

		//Task 2: Implement the calculation of perspective matrix, and then set it to pMat
		//  Note: You can use any math function (such as std::tan) except glm::perspective
		
		pMat[0][0] = 1/(aspect * std::tan(glm::radians(fovy/2))); pMat[1][0] = 0; pMat[2][0] = 0; pMat[3][0] = 0;
		pMat[0][1] = 0; pMat[1][1] = 1/(std::tan(glm::radians(fovy / 2))); pMat[2][1] = 0; pMat[3][1] = 0;
		pMat[0][2] = 0; pMat[1][2] = 0; pMat[2][2] = (far + near) / (near - far); pMat[3][2] = (2 * far * near) / (near - far);
		pMat[0][3] = 0; pMat[1][3] = 0; pMat[2][3] = -1.0f; pMat[3][3] = 0;

		return pMat;
	}
```

分析：

这里非常的简单就是对对应的矩阵进行赋值，不过要换算一下fov一系列的

**Task3**、实现视口变换矩阵（Viewport Matrix）的计算

代码：

```cpp
	glm::mat4 TRRenderer::calcViewPortMatrix(int width, int height)
	{
		//Setup viewport matrix (ndc space -> screen space)
		glm::mat4 vpMat = glm::mat4(1.0f);

		//Task 3: Implement the calculation of viewport matrix, and then set it to vpMat
		vpMat[0][0] = width / 2; vpMat[1][0] = 0; vpMat[2][0] = 0; vpMat[3][0] = width / 2;
		vpMat[0][1] = 0; vpMat[1][1] = height / 2; vpMat[2][1] = 0; vpMat[3][1] = height / 2;
		vpMat[0][2] = 0; vpMat[1][2] = 0; vpMat[2][2] = 1; vpMat[3][2] = 0;
		vpMat[0][3] = 0; vpMat[1][3] = 0; vpMat[2][3] = 0; vpMat[3][3] = 1;

		return vpMat;
	}
```

分析：

该部分就是用于将原来的1x1x1转换到分辨率的位置，直接切换即可



以上三部分实现完成后，效果参考video1.mp4



**Task4**、程序默认物体绕着y轴不停地旋转，请你在 main.cpp 文件中稍微修改一下代码，使得物体

分别绕x轴和z轴旋转。

代码：

```cpp
			//Rotation
			{
				//Task 4: Make the model rotate around x axis and z axis, respectively 

				// rotate around y axis
				model_mat = glm::rotate(model_mat, (float)deltaTime * 0.001f, glm::vec3(0, 1, 0));
				// rotate around x axis
				// model_mat = glm::rotate(model_mat, (float)deltaTime * 0.001f, glm::vec3(1, 0, 0));

				// rotate around z axis
				// model_mat = glm::rotate(model_mat, (float)deltaTime * 0.001f, glm::vec3(0, 0, 1));
			}
```

分析：

这里仿照y轴的进行转换即可，比较简单，效果参考video2_x.mp4, video2_z.mp4



**Task5**、仔细体会使物体随着时间的推进不断绕$y$轴旋转的代码，现在要你用 glm::scale 函数实现

物体不停地放大、缩小、放大的循环动画，物体先放大至$2.0$倍、然后缩小至原来的大小，然后再放

大至$2.0$，依次循环下去，要求缩放速度适中，不要太快也不要太慢。贴出你的效果，说说你是怎么

实现的。

代码：

```cpp
				cnt += 0.5f; 
				if (cnt > 1440.0f) cnt -= 1440.0f; 
				glm::vec3 scale_vec = glm::vec3(0.5 * std::sin(glm::radians(cnt)) + 1.5,
												0.5 * std::sin(glm::radians(cnt)) + 1.5,
												0.5 * std::sin(glm::radians(cnt)) + 1.5);

				model_mat = glm::scale(glm::mat4(1.0f), scale_vec);
```

分析：

这里采用了一个计数器循环来控制速度，要注意的是一定要加载一个单位阵上，不然会直接过大炸裂，效果参考video3.mp4

**Task6**、现在要求你实现正交投影矩阵的计算

正交矩阵如图：

![4](C:\Users\Administrator\Desktop\2\4.png)

代码：

```cpp
	glm::mat4 TRRenderer::calcOrthoProjectMatrix(float left, float right, float bottom, float top, float near, float far)
	{
		//Setup orthogonal matrix (camera space -> homogeneous space)
		glm::mat4 pMat = glm::mat4(1.0f);

		//Task 6: Implement the calculation of orthogonal projection, and then set it to pMat
		pMat[0][0] = 2 / (right - left); pMat[1][0] = 0; pMat[2][0] = 0; pMat[3][0] = 0;
		pMat[0][1] = 0; pMat[1][1] = 2 / (top - bottom); pMat[2][1] = 0; pMat[3][1] = 0;
		pMat[0][2] = 0; pMat[1][2] = 0; pMat[2][2] = 2 / (near - far); pMat[3][2] = (far + near) / (near - far);
		pMat[0][3] = 0; pMat[1][3] = 0; pMat[2][3] = 0; pMat[3][3] = 1;
		
		return pMat;
	}
```

分析：

这部分也是对矩阵赋值即可，比透视矩阵简单很多，就是对原来矩阵的一个缩放

**Task7**、实现了正交投影计算后，在 main.cpp 的如下代码中，分别尝试调用透视投影和正交投影函

数，通过滚动鼠标的滚轮来拉近或拉远摄像机的位置，仔细体会这两种投影的差别.

代码：

```cpp
	// Task 6: try it with different kind of projection
	{
		renderer->setProjectMatrix(TRRenderer::calcPerspProjectMatrix(45.0f, static_cast<float>(width) / height, 0.1f, 10.0f));
		//renderer->setProjectMatrix(TRRenderer::calcOrthoProjectMatrix(-2.0f, +2.0f, -2.0f, +2.0f, 0.1f, 10.0f));
	}
```

分析：

对于透视矩阵而言，会有明显的近大远小的感觉，而正交阵不会有相应的，但是随着正交阵变远也会逐步的消失。 效果参考video4.mp4

**Task8**、完成上述的编程实践之后，请你思考并回答以下问题：

1. 请简述正交投影和透视投影的区别。

   正交投影就是一个缩放的过程，将其放置到一个1x1x1的立方体中，而透视投影是变化成一个视锥体，会有近大远小的效果，更符合人眼的视觉感受

2. 从物体局部空间的顶点的顶点到最终的屏幕空间上的顶点，需要经历哪几个空间的坐标系？裁剪空间下的顶点的$w$值是哪个空间坐标下的$z$值？它有什么空间意义？

   需要经历局部空间，世界空间，观察空间，裁剪空间，屏幕空间

   是观察空间下的z值，表示深度的概念，是转换成ndc坐标的一个重要参数。是透视法的实现核心

3. 经过投影变换之后，几何顶点的坐标值是被直接变换到了NDC坐标（即$xyz$的值全部都在

$[-1,1]$范围内）吗？透视除法（Perspective Division）是什么？为什么要有这么一个过程？

​		不是，是先通过几何关系映射到近平面，在通过透视除法转换到NDC坐标。 	

​		透视除法指的是将视野（平截头体）内所有的东西都变形，然后挤压到一个小立方体里面。

​		这个过程是为了保存深度信息，是的深度测试和裁剪能够比较容易的进行。
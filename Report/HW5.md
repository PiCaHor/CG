## Assignment 5

### Task1：

实现非常的简单，主要的任务在于预处理号所有的阶乘参数，减小计算量

```c++
Point2D BezierCurve::implementTask1(const std::vector<Point2D> &points, const double &t) const
{
	//Task1: implement Bezier curve generation algorithm accroding to the definition
	int n = points.size() - 1; 
	double i_grad = 1; //save i!
	double t_grad = 1; // save t^i
	double t_ngrad = 1; //save (1-t)^(n-i)
	double n_grad = 1; 
	for (int i = 1; i < points.size(); ++i)
	{
		n_grad *= i;
		t_ngrad *= (1 - t);
	}
	double n_i_grad = n_grad; //save (n-i)!

	double B = n_grad / (i_grad * n_i_grad) * t_grad * t_ngrad; 
	Point2D res = points[0] * B; 
	for (int i = 1; i < points.size(); ++i)
	{
		i_grad *= i; 
		n_i_grad /= (n - i + 1); 
		t_grad *= t; 
		t_ngrad /= (1 - t); 
		B = n_grad / (i_grad * n_i_grad) * t_grad * t_ngrad;
		res += points[i] * B; 
	}
	return res; 

}
```

结果演示：

![image-20221202113847508](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20221202113847508.png)

### Task2

核心内容就是对点做一个整合即可，难度不大。 直接利用插值函数

```c++
Point2D BezierCurve::implementTask2(const std::vector<Point2D> &points, const double &t) const
{
	//Task2: implement de Casteljau algorithm for Bezier curve
	// Note: you should use Point2D::lerp().
	std::vector<Point2D> tmp = points; 
	int cnt = tmp.size(); 
	while (cnt != 1)
	{
		for (int i = 0; i < cnt - 1; ++i)
			tmp[i] = Point2D::lerp(tmp[i], tmp[i + 1], t);
		cnt--; 
	}
	return tmp[0];
	
}
```

结果演示:

![image-20221202113935687](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20221202113935687.png)

### Task3

我的理解贝塞尔曲线就是用折线控制曲线生成

不足：

计算量与浮点计算会存在的精度问题，会对结果造成比较大的影响。 

不能做局部的修改

在控制点很多时，很难控制曲线。
# HW0

Q1:

OpenGL 是图形化接口的一种规范，具体的实现是根据不同的厂商如英伟达，amd等显卡厂商封装起来的代码库，opengl中的很多的接口都是根据CG的原理去实现的。 

Q2:

结果如图所示

![1](C:\Users\Administrator\Desktop\1.png)

Q3

延拓坐标后如图

![2](C:\Users\Administrator\Desktop\2.png)

用GL_MIRRORED_REPEAT后如图：

![3](C:\Users\Administrator\Desktop\3.png)

用GL_CLAMP_TO_EDGE 如图

![4](C:\Users\Administrator\Desktop\4.png)

Q4：

原图输出：

![5](C:\Users\Administrator\Desktop\5.png)

扩大FOV到85![6](C:\Users\Administrator\Desktop\6.png)

缩小**aspect-ratio** 到1/2

![7](C:\Users\Administrator\Desktop\7.png)

分析：

对比图一图二，可以看出FOV改变的是视野的纵深感，也就是相机位置的张角，他不会改变图形的比例但是会在相同的分辨率下给出更广的视野

对比图一图三可以发现，对aspect-ratio的改变，会改变图片的宽高比，图片相对原图会有拉伸的感觉。 

Q5：

结果以展示在图片窗口标题
### Assignment 4

1.

结果截图：

![task1](C:\Users\Administrator\Desktop\新建文件夹\task1.png)

这一部分的编写没有遇到比较大的问题，主要就是理解了对射线的定义然后考虑颜色的线性插值即可。 

2.

结果截图

![task2](C:\Users\Administrator\Desktop\新建文件夹\task2.png)

中间的过程主要就是生成红色的球，然后采样点进行追踪，然后为了抗锯齿进行超采样取平均的过程

3.

漫反射材质结果如图

![task3](C:\Users\Administrator\Desktop\新建文件夹\task3.png)

这里的渲染其实有一点点疑惑，这里在光线发射的过程中有三种反射的随机向量，这里理解上其实有一点疑惑，但是基于先验的感觉，如果折射值很大就会很黑。 

金属材质的渲染：

![task3_2](C:\Users\Administrator\Desktop\新建文件夹\task3_2.png)

这部分的代码比较简单，主要是添加的镜面反射的相关内容。

电解质材质的渲染

![task3_3](C:\Users\Administrator\Desktop\新建文件夹\task3_3.png)

这里教程中教的有一点模糊，特别是反射定律的推导那边，后面主要是通过网上搜索找到的解释。 

4.

对焦模糊的效果

![task4](C:\Users\Administrator\Desktop\新建文件夹\task4.png)

这部分主要就是对焦点公式的理解，然后加上一个对应的模糊参数即可

5.

这一部分没有引入新的东西，是最后对于之前所有东西的运用结合，渲染时间较长

采用的参数： 1600宽 aspect_ratio = 3/2 sample次数为500

最后渲染时间为3600s多一点 1h左右

![task5](C:\Users\Administrator\Desktop\新建文件夹\task5.png)

6&8.

下面的部分是学习next week这本书的内容，包括不限于BVH树

这是开始时的对于加上动态模糊的部分效果展示

![task8](C:\Users\Administrator\Desktop\新建文件夹\task8.png)

加上bv优化后，速度明显变快

预计加速其实可以达到5倍左右，这次渲染的在原本的基础上还加上了动态模糊

![task6](C:\Users\Administrator\Desktop\新建文件夹\task6.png)

下面我们加上材质祥光的属性，输出的渲染效果如下图：

![task8_2](C:\Users\Administrator\Desktop\新建文件夹\task8_2.png)

perlin噪声的效果图：

![task8_3](C:\Users\Administrator\Desktop\新建文件夹\task8_3.png)

为了改进这种方格状，加入插值模糊和一些平滑操作

效果如图：

![task8_4](C:\Users\Administrator\Desktop\新建文件夹\task8_4.png)

继续我们加上噪音的湍流，然后改进采样频率

![task8_5](C:\Users\Administrator\Desktop\新建文件夹\task8_5.png)

为了让这个纹理更加的优美，通过一个正弦波来调整噪声的波动，这样可以形成一个类似大理石纹理的效果。

![task8_6](C:\Users\Administrator\Desktop\新建文件夹\task8_6.png)

下面是通过stb_image加载模型贴图作为纹理，这一部分直接调用对应的库并与原本的进行结合就可以完成了

效果如图

![task8_7](C:\Users\Administrator\Desktop\新建文件夹\task8_7.png)

接下来的章节主要是对于光照的设计，这里公式的推导从教程里没太看懂，后面补档了Games101去了解了相关的内容：

![task8_8](C:\Users\Administrator\Desktop\新建文件夹\task8_8.png)

接下来将建立康奈尔盒子，在建立过程中，开始时相机的位置一直有问题，所以最终呈现的效果和教程有一点点的出入，整体的效果会更加远

![task8_9](C:\Users\Administrator\Desktop\新建文件夹\task8_9.png)

接下来时雾化的过程

![task8_10](C:\Users\Administrator\Desktop\新建文件夹\task8_10.png)

最终效果呈现：

这部分渲染时间非常的长，本机用时9h

![task8_11](C:\Users\Administrator\Desktop\新建文件夹\task8_11.png)

7.

光追的效果非常的有意思，跟着两本教材一点点去做，最终呈现的效果也是满足感爆棚。不过教材有一些部分讲解的还是有一点模糊，通过网课和一些博客的指导逐渐去搞懂他。这次作业做过来感受是最深的，最后的效果也是非常的出人意料，真实感非常的强

dizuo
=====
本科、研究生期间的所有opengl代码集。
从google code迁移：https://code.google.com/p/dizuo/
> 所有代码都是基于glut框架，测试demo居多。

dizuo / dizuo / DesignPattern
---
> 本科的一些学习code：设计模式，事件模型等。
> 四叉树遍历查询demo：http://blog.csdn.net/ryfdizuo/article/details/4100689

![四叉树查询](/quad_tree.jpg)

dizuo / Opengl Csg Test
---
>   基于CSG树的3D boolean运算。

![csg](/csg.gif)

dizuo / dizuo / OGL
---
>   大三opengl课程作业，实现了一个场景管理的demo，研究生虚拟现实课程时做过一些升级，主要feature如下：
* 3ds、obj加载
* tga, pcx, bmp纹理加载
* 场景管理树
* 简单地形绘制，加载。不支持LOD
* MD2动画播放
* log、config 脚本简单实现
* 场景雾简单使用
* 实现四种camera模式：walk，drive，flight，trackball

![csg](/ogl.jpeg)

dizuo / dizuo / myFigure
---
>   本科毕业设计，实现了一个骨骼动画播放器：
*   基于opengl自己实现简单控件：imageButton，fontButton，进度条等控件。
*   加载*.bvh动画文件；
*   动画暂停、快进、快退；
*   支持照相机 pan、zoom、track模式
*   track模式下可以编辑骨架，修改bvh文件。

![动画播放器](/bvh_player.jpeg)

> 类似角色动画文章，参考：http://blog.csdn.net/ryfdizuo/article/details/7368057   

dizuo / dizuo / Simple Model Engine
---
> 研一期间实现的一个简单的建模库，进行了一些简单的尝试：
* 曲线：B样条
* 基本3D物体：cylinder，sphere，box. 
* 放样 lofter
* 挤出 extrude
* 车削加工 lathe
* 所有物体都支持纹理
* 模型动画器：扭曲 twist，波浪 wave。

> 图片来自csdn 空间，建模库截图如下：
<img src="http://img.my.csdn.net/uploads/201402/22/1393032590_2456.jpg" alt="建模库截图" width="50%" />

dizuo / opengl play ground / VL_Camera
---
>   该工程通过矩形变换的方式，提供了三种照相机操纵器：
*   move（pan），
*   zoom（scale），
*   trackball（rotate）。

dizuo / opengl play ground / gtl
---
>   geometry template library. 增加了一些opengl函数
    
dizuo / opengl play ground / QEasingCurve 
--- 
>   可视化了QT中各种动画差值曲线
>   截图：

![qeasingcurve](/ease_curve.gif)

dizuo / opengl play ground / glut_bass
>   声音与图形互动：http://blog.csdn.net/ryfdizuo/article/details/5987246

>   提前声音振幅驱动轮胎缩放，截图：

![声音可视化](/bass.gif)

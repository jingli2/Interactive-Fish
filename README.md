Interactive-Fish
================

For 48-780 Final Project


A projecting drawing of fish which is interactive to people's behavior.

陈明琦--鱼群在没有人的情况下的行为。转身，鱼游动时的摆动。

黄悉阳--鱼在有人的情况下1.人大幅度动作时候鱼被人吓跑2.人站着不动时候鱼围在人身边

闫辰羽--人点击屏幕代表喂食，屏幕中出现一块食物，鱼群过来争抢吃掉

李璟--绘制程序能够调用的鱼和背景

张洋--Kinect代替鼠标，组装，调试

需要统一的变量： 鱼是一个类。成员变量中的 X, Y, Angle 记录一条鱼当前在屏幕中的位置以及鱼头的方向。


Hi, 鱼和背景画好了！

1.我只写了一个cpp文件 和5个 stl文件
  cpp文件：drawfish2d.cpp里面包含了StringParser Mesh 等等类。Mesh 就是所画的图形。
  stl文件：lotusAndBuds.stl  ChineseFish.stl lotusLeaf.stl stem.stl, ripple.stl, dragonFly.stl 是图像的点的坐标

2.Mesh 里有 readFromFile("filename") 这个filename有五种，ChineseFish.stl, lotusAndBuds.stl读的是什么，就会画什么。
  文件路径需要改成你的电脑你的stl文件的路径。
  
3.仔细看main里面画鱼的方法。


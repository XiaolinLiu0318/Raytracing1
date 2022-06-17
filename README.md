# Raytracing1
整体是按照Ray tracing in one weekend 和Rayring tracing in next week写的，对其中的代码做了一点小优化
Done:
1.使用Opencv来弹出窗口显示图片，不用再阴间的生成ppm格式图片，再使用莫名其妙的图片打开器来查看图片了。
2.对于矩形那块做了统一，使得xy,xz,yz三种矩形生成格式统一。
3.增加了对物体进行x轴旋转和z轴旋转，并与原文里的y轴旋转做了统一。


TO DO：
1.拟使用mipmap
2.对光源采样
3.联合双边滤波

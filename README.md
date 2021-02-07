# CV-HW
ZJU CV Homework

## HW1:制作个人视频
对输入的一个彩色视频与五张以上照片，用OpenCV实现以下功能或要求：
1. 命令行格式: “xxx.exe 放视频与照片的文件夹路径” ,（例如 MyMakeVideo.exe C:\input ）【假设该文件夹下面只有一个avi视频文件与若干jpg文件】；
2. 将输入的视频与照片处理成同样长宽后，合在一起生成一个视频；
3. 这个新视频中，编程生成一个片头，然后按幻灯片形式播放这些输入照片，最后按视频原来速度播放输入的视频；
4. 新视频中要在底部打上含自己学号与姓名等信息的字幕；
5. 有能力的同学，可以编程实现镜头切换效果；（已实现）
6. 视频文件无需上载 ，但在实验报告里必须贴图展示输入输出效果

## HW2:椭圆拟合
调用CvBox2D cvFitEllipse2( const Cvarr* points )实现椭圆拟合

## HW3:Eigenface人脸识别
自己写代码实现 Eigenface 人脸识别的训练与识别过程：
1. 假设每张人脸图像只有一张人脸，且两只眼睛位置已知（即可人工标注给出）。每张图像的眼睛位置存在相应目录下的一个与图像文件名相同但后缀名为 txt 的文本文件里，文本文件中用一行、以空格分隔的4个数字表示，分别对应于两只眼睛中心在图像中的位置；
2. 实现两个程序过程（两个执行文件），分别对应训练与识别；
3. 自己构建一个人脸库（至少 40 人，包括自己），课程主页提供一个人脸库可选用；【可任意使用钉钉群内给出的三个数据集，其中BioFaceDatabase内.eye文件包含对应图片眼睛位置；Caltec Database内ImageData.mat包含人脸bounding box位置；JAFFE无额外标注】
4. 不能直接调用 OpenCV 里面与 Eigenface 相关的一些函数，特征值与特征向量求解函数可以调用；只能用 C/C++/Python，不能用其他编程语言；GUI只能用 OpenCV 自带的 HighGUI，不能用QT或其他的；平台可以用 Win/Linux/MacOS，建议 Win 优先；
5. 训练程序格式大致为：“mytrain.exe <能量百分比> <model文件名> <其他参数>…”，用能量百分比决定取多少个特征脸，将训练结果输出保存到 model 文件中。同时将前 10 个特征脸拼成一张图像，然后显示出来；
6. 识别程序格式大致为：“mytest.exe <人脸图像文件名> <model文件名> <其他参数>…”，将 model 文件装载进来后，对输入的人脸图像进行识别，并将识别结果叠加在输入的人脸图像上显示出来，同时显示人脸库中跟该人脸图像最相似的图像。

## HW4:相机定标与鸟瞰图变换
1. 参考Learning OpenCV示例18-1，利用棋盘格图像进行相机定标，将参数写入xml文件保存。棋盘格图像见群文件Learning OpenCV/LearningOpenCV_Code/LearningOpenCV_Code/calibration
2. 参考示例19-1，根据求得的内参实现鸟瞰图（俯视）转换，测试图片见群文件Learning OpenCV/LearningOpenCV_Code/LearningOpenCV_Code/birdseye
参考资料:
Learning-OpenCV-3 Example: https://github.com/oreillymedia/Learning-OpenCV-3_examples

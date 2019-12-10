编译osg：
1、下载源码;
2、创建build，在build中执行cmake ..
3、make -j4多线程make
4、安装：make install
5、将其路径写入，LD_LIBRARY_PATH；如/usr/local/lib64:/usr/local/lib
6、创建qt项目，修改项目LD_LIBRARY_PATH，
并在项目的pro文件中添加动态库LIBS +=/usr/local/lib64/xxx.so，以及头文件INCLUDEPATH += /usr/local/include/

qt与osg相结合，添加O3DWidget文件
编译osgEarth，将其与qt相结合，显示三维球组件。

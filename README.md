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

在qt中开发时需要将所需要的dll文件考在debug或release文件夹下,还需要将编译结果中bin下的osgPlugins文件夹复制到运行目录中，否则会显示错误reading file simple.earth file not handled。

- 1.自己编译的osg加载模型没有贴纹理,出现
`Warning: TexGen::apply(State&) - not supported.
Warning: Material::apply(State&) - not supported.
Warning: detected OpenGL error 'invalid enumerant' at after RenderBin::draw(..)`
警告,然后采用官方提供的编译好的osg,加载模型可以正常显示,可能自己编译的有点问题.

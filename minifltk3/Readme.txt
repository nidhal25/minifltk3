改进了中文显示速度慢的问题

windows下用vs2008编译：
添加minifltk下所有的.cxx文件，去掉platform_xxx.cxx，将minifltk加入搜索路径

osx下用xcode编译：
添加minifltk下所有.cxx cocoaXXX.mm文件，去掉platform_xxx.cxx，将minifltk加入搜索路径，将build settings下面的Objective-C automatic reference counting改成No

2014.05.10
内置了jpeg和png库，不再需要附带第三方库

若需要修改windows下程序的图标，添加vs_res.h和vs_res.rc，并修改app.ico

修正ImageGIF，可以使用动画

若需要使用GLWindow，添加extra_gl目录，并添加相应的库，在windows下添加opengl32.lib和glu32.lib

删除style.h和style.cxx，因为style和scheme有重复，而且造成类的结构不够清晰，但是目前scheme还不能及时更换

添加flsleep，编辑器添加行号

用codeblock编译，windows平台，和vs基本一样，除了要link2个库：libuuid.a和libole32.a

linux下用gcc编译：
参看os/linux/Makefile，在linux下必须有x11和xft(x freetype library)
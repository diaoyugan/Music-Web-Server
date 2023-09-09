# 一个简单的放歌网页和服务端（算是吧）

需要C++ REST SDK 2.10 推荐使用VCPKG安装


## 编译完成后

没有向局域或公网广播的功能（但你可以自己改代码）

链接格式
http://localhost:8081/html?song=歌曲对照表名称

例如将这段地址输入浏览器（推荐Microsoft Edge或Google Chrome 其他浏览器可能没有播放器控件）
http://localhost:8081/html?song=song3
这会播放x64\Debug\music文件夹里的song3.mp3

x64\Debug路径下的music_map.txt是歌曲对照表
格式为 链接关键词=歌曲路径 参考现有例子 允许使用绝对路径和相对路径
例如song=.\\music\\song1.mp3
所以要播放song1就需要输入这个链接
http://localhost:8081/html?song=song

windows系统下务必使用两个反斜线(\\)否则崩溃
绝对路径例子 "C:\\Users\\95741\\Desktop\\NFC-Music-Service\\x64\\Debug\\music\\song3.mp3"
相对路径例子 .\\music\\song3.mp3
相对地址必须要加前面的句号（.）否则无法正确寻找路径

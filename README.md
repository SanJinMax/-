本人新手, 代码很垃圾, 逻辑不清晰, 大佬勿喷
主要使用功能为 6.异或加减&左旋&异或
1 2 3 4 功能可用于新手学习或分析分区时的辅助功能。
5 功能可以无视，用处不大。 簇扇区数输入十进制， 文件位置输入十六进制，具体文件位置输入值为目录表里面的，反正没图不好解释怎么用
6 功能最常用，简单易懂 无输入上限，输入多少字节都可以。
7 8 9 功能无用，你也用不会。


联系QQ: 1762202060

1.1版本
	基本功能：
		遍历：FAT32、NTFS、exFAT、EXT
		计算：对FAT32文件位置矫正
		脚本：异或解密、左旋解密
	日期：2021-10-24
1.2版本
	优化左旋解密：主体Levo函数分解成，设置密文，设置明文，左旋解密 三部分。
	新增输出文件内容，类似winhex中十六进制数据区。（有待优化，有待新增右侧内容）
	新增对jpg图片文件进行简单判断是否为异或解密或者左旋解密。（有待优化）
	日期：2021-10-26
1.2.1版本
	更改了左旋和异或输出的 "------" 个数 （狗头.jpg)
	由于以前异或和左旋输出结果时 0-F 只显示 0-F ，优化为 0-F 显示为 00-0F
	优化jpg解密，明文从 FF D8 FF D9 更改为 FF D8 FF E0 D9 FF
	将jpg解密封装为函数。
	日期：2021-10-27
1.3版本
	将各种文件的解密函数打包，并只用选择8就可以直接判断文件类型，然后进行文件简单解密。
	文件解密包含：.jpg .png .bmp .gif .doc .ppt .xls .docx .pptx .xlsx
	日期：2021-10-29
1.4版本
	新增对jpg固定结构破坏的修复，只支持windows自带的画图软件制作的jpg文件。
	新增对doc头部SAT等值破坏的修复，算法有错，只支持某些doc。
	日期：2021-11-02
1.5版本
	新增单异或，将异或加减、左旋、单异或 合并为一个功能
	优化异或逻辑，优化设置密文明文代码。
	修复输入退出编号时的bug。
	修复doc一个无关紧要的bug（因为修复doc头部这个功能，基本上没用用处）
	日期：2021-11-03
// 



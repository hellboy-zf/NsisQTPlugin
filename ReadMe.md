# 介绍
使用Qt作为界面库

---

# 依赖项

**1. NSIS**

从[https://nsis.sourceforge.io/Download](https://nsis.sourceforge.io/Download) 下载NSIS并安装

添加环境变量`NSIS_DIR`为NSIS安装目录。

**2. Python**

之所以需要安装Python，主要是为了执行`NsisScriptGenerate.py`脚本。

将`Python.exe`所在目录添加到`Path`环境变量。

**3. Qt**

因为插件默认使用Qt作为界面库，所以需要安装Qt。

Qt安装包会默认将安装目录添加`QTDIR`环境变量，如果没有自动添加，则需要手动添加。

---

# 开始使用

**编译QTSetupPlugin

1.安装完上面依赖项之后，编译 `QTSetupPlugin\QTSetupPlugin.sln` 工程，`Visual Studio`生成后事件会自动将目标文件(`Debug`版：`QTSetupPlugin_d.dll`，`Release`版：`QTSetupPlugin.dll`)拷贝到NSIS插件目录(`NSIS_DIR\Plugins\x86-unicode`)。



2.用QT工具生成dll发布所需文件(注：需手动生成)。`Package\Debug和Package\Release`分别是`QTSetupPlugin_d.dll`和`QTSetupPlugin.dll`运行所需文件。(QT命令: `windeployqt QTSetupPlugin.dll`)



3.将需要打包的文件目录在`build_setup`中设置，其它参数在`nsis_setup_template.nsi`中设置，然后运行`build-setup.bat`生成安装包。



**说明：**

```txt
Package                		-- QTSetupPlugin_d.dll或QTSetupPlugin.dll QT的打包文件
SystemDll       			-- 放置VC++运行时库文件（Debug版和Release版），Qt界面库默认采用MD模式编译，运行时需要依赖VC++运行时库
nsis_setup_template.nsi 	-- NSIS模板文件，文件名的_template后缀固定的
DeleteUselessDependFile.py	-- 会删除Package中不需要的依赖文件
NsisScriptGenerate.py		-- 会根据该模板生成 nsis_setup.nsi
build_setup _debug.bat 		-- 生成Debug版的安装包
build-setup.bat 			-- 生成Release版的安装包
```


# PCL + OpenCV + VTK Demo Project

这是一个基于 C++ / CMake / VS Code 的点云处理与图像可视化**学习演示工程**，用于验证
**PCL（点云）+ OpenCV（图像）+ VTK（3D 渲染）** 这条工具链在 Windows 下能否完整打通。

> 💡 本项目为学习和演示目的设计，不含业务逻辑，重点在于展示「如何手工接入三大第三方库」。

---

## 🧩 功能简介

程序启动后依次执行 4 个互相独立的演示：

| 顺序 | 入口函数 | 演示内容 | 涉及库 |
|:---:|---|---|---|
| 1 | `myfunc()` | 调用 `add()` / `diff()` 两个自定义函数并打印结果 | —（自建模块） |
| 2 | `opencv_demo()` | 以灰度方式读取 `data/17.jpg` 并弹窗显示 | OpenCV |
| 3 | `edge_demo()` | Canny 边缘检测，带**阈值滑动条**可实时调节 | OpenCV |
| 4 | `pcl_demo()` | 生成 1000 个随机彩色点云，弹出 3D 交互窗口 | PCL + VTK |

其它特性：

- ✅ Debug / Release 双配置构建，输出统一到 `bin/`
- ✅ 通过 CMake 编译宏控制 Debug 专属输出
- ✅ `src/sum`、`src/diff` 用于验证「多子目录源文件自动收集」

---

## 🛠️ 技术栈

| 组件 | 版本 |
|------|------|
| C++ | C++17 |
| CMake | v3.16+ |
| PCL | 1.15.1 |
| OpenCV | 4.9.0 |
| VTK | 9.4（随 PCL 分发，位于 `PCL_ROOT/3rdParty/VTK`） |
| 编译器 | MSVC (Visual Studio) / x64 |
| 生成器 | Ninja |
| IDE | VS Code + CMake Tools |

---

## 📁 工程目录结构

```text
project_01/
├── CMakeLists.txt          # 唯一的构建脚本（手工指定三方库路径，不使用 find_package）
├── README.md               # 本说明文档
├── data/
│   └── 17.jpg              # 唯一测试素材（OpenCV 两个 demo 共用）
├── bin/                    # 可执行文件输出目录（HelloDemo.exe / .pdb）
├── build/                  # CMake 构建中间文件
│   ├── Debug/              # 对应 CMAKE_BUILD_TYPE=Debug
│   └── Release/            # 对应 CMAKE_BUILD_TYPE=Release
└── src/
    ├── demo.cpp            # ★ 唯一程序入口 main()，串起全部演示
    ├── sum/                # add(a, b)
    │   ├── sum.h
    │   └── sum.cpp
    ├── diff/               # diff(a, b)
    │   ├── diff.h
    │   └── diff.cpp
    └── edge/               # edge_demo()：OpenCV Canny + 阈值滑动条
        ├── edge.h
        └── edge.cpp
```

> ⚠️ 程序入口是 **`src/demo.cpp`**，工程中**没有** `main.cpp`。

---

## 🔧 构建

### 方式一：VS Code + Kit（推荐，日常开发用这个）

用 VS Code 打开工程，安装 **CMake Tools** 扩展，然后：

1. `Ctrl+Shift+P` → **CMake: Select a Kit** → 选 `Visual Studio ... Release - x64`（或 amd64）
2. `Ctrl+Shift+P` → **CMake: Configure**
3. **F7** 构建，**F5** 调试

`.vscode/settings.json` 已把构建目录配置为按构建类型分目录：

```json
"cmake.buildDirectory": "${workspaceFolder}/build/${buildType}",
"cmake.generator": "Ninja"
```

> **为什么必须用 Kit，而不是 CMakePresets？**
> MSVC 编译除了 `cl.exe` 在 PATH，还需要 `INCLUDE` / `LIB`（指向 MSVC STL 和
> Windows SDK）。**Kit 会自动注入这一整套环境**，而 CMakePresets 在 Windows 上
> 无法加载 `vcvars64.bat`——只在预设里写 `cl.exe` 绝对路径是不够的，configure 会在
> 编译器 ABI 检测阶段失败。此外，预设模式会**完全绕过 Kit**，容易让人误以为工程
> "只能用某一种生成器"。所以本工程**不使用 CMakePresets**。

### 方式二：命令行

需要先进入 VS 开发者环境（否则 `cl.exe` 找不到标准库头文件，报 `C1034`）：

```powershell
# 先加载 MSVC 环境（路径按你的 VS 安装调整）
& "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"

# Debug
cmake -S . -B build/Debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build/Debug

# Release
cmake -S . -B build/Release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/Release
```

> 注意：本机 PATH 中没有 `ninja`，命令行下需指定 VS 自带的 ninja：
> `-DCMAKE_MAKE_PROGRAM="C:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/Ninja/ninja.exe"`
> （VS Code 里由 CMake Tools 自动解析，无需手动指定。）

### 方式三：Visual Studio 工程（仅用于在 VS 里断点调试）

在 VS Code 里运行任务 **「生成 Visual Studio 解决方案 (Debug)」**
（`Ctrl+Shift+P` → `Tasks: Run Task`），等价于：

```powershell
cmake -S . -B build/vs -G "Visual Studio 18 2026" -A x64 -DCMAKE_BUILD_TYPE=Debug
```

然后打开 `build/vs/HelloDemo.slnx`（CMake 4.x 对 VS 18 生成的是 XML 格式的 `.slnx`，
不是传统 `.sln`），**在 VS 里必须选 `Debug` 配置**。

> ⚠️ 不要用 CMake Tools 去构建这个 `build/vs` 目录：CMake Tools 会传
> `--target all`，而 MSBuild 会去找 `all.vcxproj`（实际文件名是 `ALL_BUILD.vcxproj`），
> 于是报 `MSB1009: 项目文件不存在`。要构建它请在 Visual Studio 里按 F7，或直接用
> `cmake --build build/vs --config Debug --target HelloDemo`。

### 修改第三方库位置

库路径是**可缓存的 CMake 变量**，不必改源码，重新 configure 时传入即可：

```powershell
cmake -S . -B build/Debug -G Ninja -DCMAKE_BUILD_TYPE=Debug `
      -DPCL_ROOT="D:/OpenSource/PCL1151" `
      -DOPENCV_ROOT="D:/OpenSource/OPENCV490"
```

---

## ▶️ 运行

生成的 `HelloDemo.exe` 位于 `bin/`，而图片是按**相对路径 `../data/17.jpg`** 加载的，
因此**必须从 `bin/` 目录启动**：

```powershell
cd bin
.\HelloDemo.exe
```

### 运行前提：DLL 必须在 PATH 中

工程链接的是 OpenCV / PCL / VTK 的导入库，运行时仍需要这些库的 **DLL**。
把以下目录加入 `PATH`（否则会提示「找不到 xxx.dll」）：

```text
D:\OpenSource\OPENCV490\opencv\build\x64\vc16\bin
D:\OpenSource\PCL1151\bin
D:\OpenSource\PCL1151\3rdParty\VTK\bin
```

### 交互说明

| 步骤 | 操作 |
|---|---|
| `opencv_demo()` | 弹出灰度图窗口，**按任意键**关闭 |
| `edge_demo()` | 弹出 `Original` 与 `Canny Edges` 两个窗口，拖动 `High Threshold` 滑动条（0~500）可实时观察边缘变化，低阈值自动取高阈值的一半；**按任意键**关闭 |
| `pcl_demo()` | 弹出黑色背景 3D 点云窗口，含 XYZ 坐标轴，鼠标可旋转/缩放；**按 `q` 或关闭窗口**退出 |

---

## 🧱 CMakeLists.txt 关键设计

| 设计点 | 位置 | 说明 |
|---|---|---|
| 源文件收集 | `file(GLOB_RECURSE ...)` | 自动抓取 `src/**/*.cpp`。**新增 .cpp 后必须重新 configure**，否则不会参与编译 |
| 依赖定位 | `PCL_ROOT` / `OPENCV_ROOT` 缓存变量 | 硬编码绝对路径 + `CACHE PATH`，不走 `find_package` |
| 头文件路径 | `target_include_directories` | 手工列出 PCL、Boost、Eigen3、FLANN、Qhull、VTK、OpenCV 的 include 目录 |
| Debug/Release 判定 | 生成器表达式 `$<$<CONFIG:Debug>:...>` | 用于**编译选项与宏**，多配置生成器下同样正确 |
| 库名后缀 | configure 期 `if(CMAKE_BUILD_TYPE ...)` | 库名必须在 configure 阶段写死，属 `CMAKE_BUILD_TYPE` 的合法用途；多配置生成器下做了兜底推断 |
| 库链接 | 全路径 `.lib` | 直接链接完整路径，绕开 `link_directories` |

Debug 与 Release 的库变体对照：

| 库 | Debug | Release |
|---|---|---|
| OpenCV | `opencv_world490d.lib` | `opencv_world490.lib` |
| PCL | `pcl_commond.lib` … | `pcl_common.lib` … |
| VTK | `vtkCommonCore-9.4-gd.lib` … | `vtkCommonCore-9.4.lib` … |

调试宏 `MY_TEST_SHOW_PRINTF` 仅在 Debug 配置下由 CMake 定义，`demo.cpp` 中通过
`#ifdef` 判断，用于验证宏按配置生效。

---

## ⚠️ 已知注意事项

1. **`/arch:AVX2` 是无条件开启的**（见 `CMakeLists.txt` 编译选项）。目标 CPU 必须支持
   AVX2，否则程序会在运行时直接崩溃。换机器编译前请确认。
2. **图片是相对路径加载**，务必从 `bin/` 目录运行，见上文「运行」。
3. **新增源文件后需重新 configure**：`file(GLOB_RECURSE)` 的结果在 configure 阶段确定，
   只 build 不会发现新文件。
4. **不要并发构建同一个 build 目录**：Ninja 会用 `.ninja_lock` 保护构建目录，同一目录下
   同时跑两个构建（例如命令行与 VS Code 同时触发）会互相阻塞。
5. **命令行构建前必须先加载 MSVC 环境**（`vcvars64.bat`），否则 `cl.exe` 找不到标准库
   头文件，报 `fatal error C1034: iostream: 不包括路径集`。VS Code 里由 Kit 自动处理。
6. **生成 Visual Studio 工程后，不要用 CMake Tools 去构建 `build/vs`**：CMake Tools 会传
   `--target all`，MSBuild 会去找 `all.vcxproj`（实际是 `ALL_BUILD.vcxproj`），报
   `MSB1009: 项目文件不存在`。请在 Visual Studio 里构建，或显式指定
   `cmake --build build/vs --config Debug --target HelloDemo`。
7. 工程根目录的 `*.log` 已在 `.gitignore` 中忽略。

# LiteEngine

基于Vulkan的渲染器，还在锐意制作中

### 环境依赖

- Visual Studio 2022
- Vulkan

### 拉取

```bash
git clone https://github.com/Reuben-Sun/LiteEngine.git
cd LiteEngine
git submodule init
git submodule update
```



### 构建

1. 运行脚本`cmake_generate.bat`，在build文件夹中会生成VS工程
2. 用VS2022打开`.sln`工程，设置`Client`为目标，构建运行

### 开发进度

#### 2024.2.6

- 搭建ImGui Editor框架
- 基础Vulkan渲染
- 接入简单刚体物理系统

![24_2_6](Doc/24_2_6.png)

#### 2024.2.28

- 添加简单光照计算
- 迭代材质系统、绑定系统
- 辅助线绘制物理BV

![24_2_28](Doc/24_2_28.png)

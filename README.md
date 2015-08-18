# 语音聊天小工具

## 实现功能

通过麦克风采集声音并实时传输，接收后实时播放。

## 使用环境

Linux(非虚拟机)下运行

## 使用方法

### 安装 alsa 环境(ubuntu)
    sudo apt-get install libasound2-dev alsa-oss alsa-utils

### 编译代码
    git clone https://github.com/chenshuchao/c_chat.git && cd c_chat
    make

### 启动
    ./ser
    ./cli


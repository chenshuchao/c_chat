# 语音聊天小工具

## 使用环境

Ubuntu14(非虚拟机环境)下运行

## 使用方法

### 安装 alsa 环境
    sudo apt-get install libasound2-dev alsa-oss alsa-utils

### 编译代码
    git clone https://github.com/chenshuchao/c_chat.git
    cd c_chat/src
    g++ src/lib/utils.c src/lib/sound.c src/lib/cache.c src/client/client.c src/client/main.c -I ./src -lasound -pthread -o client
    g++ src/lib/utils.c src/lib/sound.c src/lib/cache.c src/server/server.c src/server/main.c -I ./src -lasound -pthread -o server

### 启动
    ./server
    ./client


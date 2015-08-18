# 语音聊天小工具

## 使用环境

Ubuntu(非虚拟机)下运行

## 使用方法

### 安装 alsa 环境
    sudo apt-get install libasound2-dev alsa-oss alsa-utils

### 编译代码
    git clone https://github.com/chenshuchao/c_chat.git && cd c_chat
    g++ lib/utils.c lib/sound.c lib/cache.c client/client.c client/main.c -I ./ -lasound -pthread -o cli
    g++ lib/utils.c lib/sound.c lib/cache.c server/server.c server/main.c -I ./ -lasound -pthread -o ser

### 启动
    ./ser
    ./cli


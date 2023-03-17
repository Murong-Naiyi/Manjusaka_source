cmake_minimum_required(VERSION 3.14)

# 项目信息
project(Manjusaka)

# 配置环境
set(CMAKE_SYSTEM_NAME "Linux")
set(CMAKE_SYSTEM_PROCESSOR "aarch64")
set(TZ "Asia/Shanghai")
execute_process(COMMAND ln -snf /usr/share/zoneinfo/${TZ} /etc/localtime)
execute_process(COMMAND echo ${TZ} > /etc/timezone)
# 设置编译链接参数
set(CMAKE_C_FLAGS "-g0 -O0 -W -Wall -Wno-format-truncation -Werror -fvisibility=hidden -flto -fno-omit-frame-pointer -fdata-sections -ffunction-sections")
set(CMAKE_EXE_LINKER_FLAGS "-g0 -O0 -static -fvisibility=hidden -flto -ffixed-x18 -Wl,-O0,--strip-all,--hash-style=both,--gc-sections,--exclude-libs,ALL")

add_executable(Manjusaka_Resolution ./Source/Manjusaka_Dpi.cpp)

target_link_libraries(Manjusaka_Resolution PRIVATE pthread)


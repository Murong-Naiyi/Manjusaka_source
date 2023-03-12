#!/bin/sh


# 获取ksud版本号，并根据版本号设置模块路径
S=$(/data/adb/ksud -V | awk '/ksud/{gsub("ksud ", ""); print substr($0,1,4)}')
Module_Path="/data/adb/ksu/modules"
[ "$S" = "v0.4" ] && Module_Path="/data/adb/modules"

# 设置相关文件的路径
thermal_path="$Module_Path/Manjusaka_thermal"

# 删除Qiu目录及其子目录和文件，然后创建一个空的Qiu文件
rm -rf "$thermal_path/Qiu"
mkdir -p "$thermal_path/Qiu"
touch "$thermal_path/Qiu/Manjusaka"

# 将Qiu文件的完整路径保存到变量$Qiu中
Qiu="$thermal_path/Qiu/Manjusaka"

# 查找需要处理的文件，并对每个文件执行相应的操作
find / \( \
    -path "/mnt" -o \
    -path "/data" -o \
    -path "/proc" -o \
    -path "/dev" -o \
    -path "/storage" -o \
    -path "/data_mirror" -o \
    -path "/apex" -o \
    -path "/sys" -o \
    -path "/acct" \
\) -prune -o -iname "*thermal*" -type f ! -name "*.so" ! -name "*"@"*" ! -name "*hardware*" -print0 | while IFS= read -r -d '' Manjusaka; do
    case "$Manjusaka" in
        *.so|*@|*hardware)
            continue
            ;;
    esac
    /data/ManjusakaTool/mount --make-slave --rbind --size "$Qiu" "$Manjusaka"
    chmod 000 "$Manjusaka"
    chown -h adb.adb "$Manjusaka"
done

# 删除/data/vendor/thermal目录下的所有文件，并创建空文件
if [ -d "/data/vendor/thermal" ]; then
    rm -rf "/data/vendor/thermal/"
    mkdir -p "/data/vendor/thermal"
    chmod 000 "/data/vendor/thermal"
fi

# 删除/dev/socket/目录下相关的文件
find /dev/socket/ -iname "*thermal*" -o -iname "thermal*" -type f -exec rm -f {} +

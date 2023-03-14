#!/system/bin/sh
Conf_path="/data/media/0/Android/Manjusaka/Crond定时清理/Manjusaka_Clear.conf"
if [ ! -f $Conf_path ]; then
mkdir -p "/data/media/0/Android/Manjusaka/Crond定时清理"

    echo "####################
####################
####################
# Author:Manjusaka(酷安@曼珠沙华Y)
# Group:647299031       
# QQ:898780441          
# Github:https://github.com/ManjusakaY
####################
####################
####################


# 是否启用空文件夹扫描检查 
# 默认开启
Folder_check=true

# 检查的目录
# 默认为/data/media/0
Dir_chrck=/data/media/0

# 是否检查子目录
# 默认关闭
Subdirectory_check=false
# 开启后将则将递归检查该目录下的所有子目录
# 关闭后则只检查该目录

# 是否启用黑名单检查
# 默认开启
Blacklist_check=true




" > $Conf_path
    exit 255
else
    source $Conf_path
fi

removeEmptyDirs () {
    for dir in "$1"/*; do
        if [ -d "$dir" ]; then
            if [ "$(command ls -A "$dir")" ]; then
                if [ "${Subdirectory_check}" = true ]; then
                    removeEmptyDirs "$dir"
                fi
            else
                echo "$(date +"%Y-%m-%d %H:%M:%S") 删除空文件夹: $dir" >> /data/media/0/Android/Manjusaka/Crond定时清理/Clear.log
                command rmdir "$dir"
            fi
        fi
    done
}
Blacklist () {
 if [ ! -f /data/media/0/Android/Manjusaka/Crond定时清理/Manjusaka_Blacklist.conf ]; then
    mkdir -p /data/media/0/Android/Manjusaka/Crond定时清理
    echo "#没有白名单保护，请勿手残
#文件夹结尾加/，不加/代表文件
/data/media/0/.*
/data/media/0/Android/Manjusaka/blacklist.conf
/data/media/0/Android/Manjusaka/Clear.log
/data/media/0/fvlog.txt
/data/media/0/Alarms/
/data/media/0/Audiobooks/
/data/media/0/com.tencent.mobileqq/
/data/media/0/com.quark.browser/
/data/media/0/iApp/
/data/media/0/libs/
/data/media/0/mfcache/
/data/media/0/mipush/
/data/media/0/Notifications/
/data/media/0/OSSLog/
/data/media/0/qqstory/
/data/media/0/qsvf/
/data/media/0/Ringtones/
/data/media/0/tbs/
/data/media/0/Recordings/
/data/media/0/Podcasts/


" > /data/media/0/Android/Manjusaka/Crond定时清理/Manjusaka_Blacklist.conf
 fi

# 定义黑名单文件路径和IFS变量
BLACKLIST_PATH="/data/media/0/Android/Manjusaka/Crond定时清理/Manjusaka_Blacklist.conf"
local IFS=$'\n'

# 将黑名单文件内容读入变量qiule，并去除注释行
qiule=""
for bl in $(cat $BLACKLIST_PATH | grep -v '#'); do
    qiule="$qiule"$'\n'"$bl"
done

# 定义删除文件或目录的函数
function delete_file_or_directory() {
    local path=$1
    if [[ -d "$path" ]]; then
        # 排除当前目录、父目录和链接目录，并删除目录
        case $path in
            *'/.') continue ;;
            *'/./') continue ;;
            *'/..') continue ;;
            *'/../') continue ;;
        esac
        rm -rf "$path" && {
            let DIR++
            echo "$(date +"%Y-%m-%d %H:%M:%S") 删除文件夹 $path" >> /data/media/0/Android/Manjusaka/Crond定时清理/Clear.log
        }
    fi
    if [[ -f "$path" ]]; then
        # 删除文件
        rm -rf "$path" && {
            let FILE++
            echo "$(date +"%Y-%m-%d %H:%M:%S") 删除文件 $path" >> /data/media/0/Android/Manjusaka/Crond定时清理/Clear.log
        }
    fi
}

# 循环遍历黑名单列表，逐一删除文件或目录
for item in $(echo "$qiule" | grep -v '*'); do
    delete_file_or_directory "$item"
done
}

if [ "${Blacklist_check}" = true ]; then
    Blacklist
fi

if [ "${Folder_check}" = true ]; then
    removeEmptyDirs "$Dir_chrck"
fi
chmod -R 755 "/data/media/0/Android/Manjusaka/Crond定时清理"
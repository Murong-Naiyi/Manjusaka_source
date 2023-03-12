#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <sys/stat.h>
// 配置文件路径
std::string Manjusaka_conf_path = "/data/media/0/Android/Manjusaka/Manjusaka.conf";

void check_loop(std::string Set_Time, std::string Creen_script, std::string Screen_script) {
    auto check = [&]() {
        std::string Qiule;
        FILE *fp;
        char buffer[1024];
        fp = popen("dumpsys window policy | awk -F= '/mInputRestricted/ {print $2}'", "r");
        fgets(buffer, sizeof(buffer), fp);
        pclose(fp);
        Qiule = buffer;
        Qiule.erase(Qiule.find_last_not_of("\n") + 1);

        static std::string Manjusaka = "false";
        if (Qiule != Manjusaka) {
            Manjusaka = Qiule;
            if (Qiule == "true") {
                std::this_thread::sleep_for(std::chrono::seconds(std::stoi(Set_Time)));
                system(Creen_script.c_str());
            } else {
                system(Screen_script.c_str());
            }
        }
    };
    while (true) {
        check();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    std::string Set_Time, Creen_script, Screen_script;
    {
        std::ifstream infile(Manjusaka_conf_path);
        if (!infile.good()) {
            // 创建默认配置文件
            mkdir("/data/media/0/Android/Manjusaka", 0777);
            std::ofstream outfile(Manjusaka_conf_path);
            outfile << "# Author:Manjusaka(酷安@曼珠沙华Y)" << std::endl;
            outfile << "# Group:647299031" << std::endl;
            outfile << "# QQ:898780441" << std::endl;
            outfile << "# 设置屏幕关闭多长时间执行" << std::endl;
            outfile << "Set_Time=1" << std::endl;
            outfile << "# 屏幕关闭后执行的脚本" << std::endl;
            outfile << "Creen_script=/data/off.sh" << std::endl;
            outfile << "# 屏幕开启后执行的脚本" << std::endl;
            outfile << "Screen_script=/data/on.sh" << std::endl;
            outfile.close();
        }

        std::ifstream fconf(Manjusaka_conf_path);
        std::string line;
        while (std::getline(fconf, line)) {
            if (line.find("Set_Time=") != std::string::npos) {
                Set_Time = line.substr(line.find("=") + 1);
            } else if (line.find("Creen_script=") != std::string::npos) {
                Creen_script = line.substr(line.find("=") + 1);
            } else if (line.find("Screen_script=") != std::string::npos) {
                Screen_script = line.substr(line.find("=") + 1);
            }
        }
        fconf.close();
    }
    // 设置配置文件权限为755
    system(("chmod 755 " + Manjusaka_conf_path).c_str());
    std::thread t(check_loop, Set_Time, Creen_script, Screen_script);

    // 减少 CPU 占用率
    std::this_thread::sleep_for(std::chrono::seconds(10));

    t.join();

    return 0;
}

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <sys/stat.h>

// 配置文件路径
std::string Manjusaka_conf_path = "./_純白恋歌.cblg";
void check_screen(std::string Set_Time, std::string Creen_script, std::string Screen_script, std::string Screen_switch) {
    auto check = [&]() {
        if (Screen_switch != "Y") return; // 当 Screen_switch=N 时，退出函数
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

void check_battery(std::string Set_Time_battery, std::string Charge_script, std::string Poweroff_script, std::string battery_switch) {
    auto check2 = [&]() {
        if (battery_switch != "Y") return; // 当 battery_switch=N 时，退出函数
        std::string Qiule2;
        FILE *fp;
        char buffer[1024];
        fp = popen("dumpsys battery | grep 'AC powered' | awk '{print $3}'", "r");
        fgets(buffer, sizeof(buffer), fp);
        pclose(fp);
        Qiule2 = buffer;
        Qiule2.erase(Qiule2.find_last_not_of("\n") + 1);

        static std::string Manjusaka2 = "false";
        if (Qiule2 != Manjusaka2) {
            Manjusaka2 = Qiule2;
            if (Qiule2 == "true") {
                std::this_thread::sleep_for(std::chrono::seconds(std::stoi(Set_Time_battery)));
                system(Charge_script.c_str());
            } else {
                system(Poweroff_script.c_str());
            }
        }
    };
    while (true) {
        check2();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
int main() {
    std::string Set_Time, Creen_script, Screen_script, Screen_switch;
    std::string Set_Time_battery, Charge_script, Poweroff_script, battery_switch; // 添加变量
    {
        std::ifstream infile(Manjusaka_conf_path);
        if (!infile.good()) {
            // 创建默认配置文件
            std::ofstream outfile(Manjusaka_conf_path);
            outfile << "Screen_switch=Y" << std::endl;
            outfile << "Set_Time=10" << std::endl;
            outfile << "Creen_script=./doze.cblg" << std::endl;
            outfile << "Screen_script=./undoze.cblg" << std::endl;
            outfile << "battery_switch=N" << std::endl;
            outfile << "Set_Time_battery=1" << std::endl;
            outfile << "Charge_script=/dev/null" << std::endl;
            outfile << "Poweroff_script=/dev/null" << std::endl;
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
            } else if (line.find("Screen_switch=") != std::string::npos) {
                Screen_switch = line.substr(line.find("=") + 1);
            } else if (line.find("Set_Time_battery=") != std::string::npos) { // 读取电池检测相关配置项
                Set_Time_battery = line.substr(line.find("=") + 1);
            } else if (line.find("Charge_script=") != std::string::npos) {
                Charge_script = line.substr(line.find("=") + 1);
            } else if (line.find("Poweroff_script=") != std::string::npos) {
                Poweroff_script = line.substr(line.find("=") + 1);
            } else if (line.find("battery_switch=") != std::string::npos) {
                battery_switch = line.substr(line.find("=") + 1);
            }
        }
        fconf.close();
    }
    system(("chmod 777 " + Manjusaka_conf_path).c_str());
    std::thread t(check_screen, Set_Time, Creen_script, Screen_script, Screen_switch);
    std::thread t2(check_battery, Set_Time_battery, Charge_script, Poweroff_script, battery_switch); // 添加电池检测线程
    std::this_thread::sleep_for(std::chrono::seconds(10));
    t.join();
    t2.join();
    return 0;
}

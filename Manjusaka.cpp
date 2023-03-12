#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>

// 配置文件路径
std::string Manjusaka_conf_path = "/data/Manjusaka.conf";

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

// 检测配置文件

int main() {
    std::ifstream infile(Manjusaka_conf_path);
    if (!infile.good()) {
        std::ofstream outfile(Manjusaka_conf_path);
        outfile << "Set_Time=1" << std::endl;
        outfile << "Creen_script=/data/off.sh" << std::endl;
        outfile << "Screen_script=/data/on.sh" << std::endl;
        outfile.close();
    }

    std::ifstream fconf(Manjusaka_conf_path);
    std::string line;
    std::string Set_Time, Creen_script, Screen_script;
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

    std::thread t(check_loop, Set_Time, Creen_script, Screen_script);

    // 让主线程等待一段时间，不再进行循环检测，以减少 CPU 占用率
    std::this_thread::sleep_for(std::chrono::seconds(10));

    t.join();

    return 0;
}
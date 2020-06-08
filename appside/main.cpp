#include "libremotehaptics.h"
#include <iostream>
#include <thread>

int main(){
    std::cout << "Hello world\n";
    Libremotehaptics rh;

    rh.init(1000);
    Libremotehaptics::Vector3d v;

    std::cout << "Opened remotehaptics!\n";

    while(true){
        rh.getPosition(v);

        Libremotehaptics::Vector3d f;
        f.x = 0;//-200*v.x;
        f.y = 0;//-200*v.y;
        f.z = 1;//-200*v.z;
        rh.setForce(f);

        std::cout << "Pos: " << v.x << " " << v.y << " " << v.z <<"\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}

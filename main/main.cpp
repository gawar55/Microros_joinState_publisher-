#include <stdio.h>
#include <urosHandler.hpp>
#include <pub.hpp>
#include <stdio.h>
#include <esp_log.h>

#define LOG "MAIN"

uros_master_node* master_node = 0;

extern "C" void app_main(void)
{
    master_node = new uros_master_node("master_node");

    std::vector<joint> joints(3);
    joints[0] = {"joint1", new double(1.0), new double(5.0)};
    joints[1] = {"joint2", new double(2.0), new double(3.0)};
    joints[2] = {"joint3", new double(4.0), new double(7.0)};

    master_node->add_urosElement({
        // new pub(name,position,velocity,size)
        new pub(joints) 
    });
}
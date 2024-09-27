#include "pub.hpp"
#include <esp_log.h>
#include <rclc/publisher.h>
#include <rmw_microros/rmw_microros.h>
// #include <std_msgs/msg/int32.h>
#include <sensor_msgs/msg/joint_state.h>
#include "rosidl_runtime_c/string_functions.h"
#include "rosidl_runtime_c/primitives_sequence_functions.h"
#include <cstring>

#define LOG "MAIN"
const rosidl_message_type_support_t *pub_type_support = ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, JointState);
// int pub::size = 0; // Define the static int variable
char **pub::arr_name = nullptr;
double *pub::arr_position = nullptr; // Define the static pointer to arr_position
double *pub::arr_velocity = nullptr; // Define the static pointer to arr_velocity
pub *pub::publish = 0;
pub *pub::active = nullptr;
const unsigned int timer_period = RCL_MS_TO_NS(1000);
pub::pub(std::vector<joint>& p_joint)
// pub::pub(char **arr_name, double *arr_position, double *arr_velocity, int size)
{
    // pub::arr_name = arr_name;
    // pub::arr_position = arr_position;
    // pub::arr_velocity = arr_velocity;
    // pub::size = size;
    int size = p_joint.size();
    pub::arr_name = new char*[size];
    pub::arr_position = new double[size];
    pub::arr_velocity = new double[size];

   for (int i = 0; i < size; ++i) {
        pub::arr_name[i] = strdup(p_joint[i].name);
        pub::arr_position[i] = *p_joint[i].position;
        pub::arr_velocity[i] = *p_joint[i].velocity;
    }


    publish = this;
    active = this;

    sensor_msgs__msg__JointState__init(&joint_state_msg);
    rosidl_runtime_c__double__Sequence__init(&joint_state_msg.position, size);
    rosidl_runtime_c__double__Sequence__init(&joint_state_msg.velocity, size);
    rosidl_runtime_c__String__Sequence__init(&joint_state_msg.name, size);

    for (int i = 0; i < size; ++i) {
        joint_state_msg.position.data[i] = *p_joint[i].position;
        joint_state_msg.velocity.data[i] = *p_joint[i].velocity;
        rosidl_runtime_c__String__assign(&joint_state_msg.name.data[i], p_joint[i].name);
    }
}

void pub::init()
{
    rcl_ret_t ret = rclc_publisher_init_default(
        &publisher,       
        node,             
        pub_type_support, 
        "my_joint_states" 
    );

    
    if (ret != RCL_RET_OK)
    {
        ESP_LOGE(LOG, "Failed to initialize publisher: %s", rcl_get_error_string().str);
        rcl_reset_error();
        return;            
    }
    ESP_LOGI(LOG, "Publisher initialized successfully.");

    if (rclc_timer_init_default(&timer, support, timer_period, pub::pub_callback) != RCL_RET_OK)
    {
        ESP_LOGE(LOG, "Failed to initialize timer");
    }

    if (rclc_executor_add_timer(exec, &timer) != RCL_RET_OK)
    {
        ESP_LOGE(LOG, "Failed to add timer to executor");
    }
}

void pub::pub_callback(rcl_timer_s *time, int64_t num)
{
    // sensor_msgs__msg__JointState joint_state_msg;
    // sensor_msgs__msg__JointState__init(&joint_state_msg);
    // rosidl_runtime_c__String__Sequence__init(&joint_state_msg.name, size);
    // rosidl_runtime_c__double__Sequence__init(&joint_state_msg.position, size);
    // rosidl_runtime_c__double__Sequence__init(&joint_state_msg.velocity, size);
    // const char *joint_name = "joint_1";
    // for (int i = 0; i < size; i++)
    // {
    //     joint_state_msg.position.data[i] = arr_position[i];
    //     ESP_LOGI("MAIN", "Enter");
    //     // joint_state_msg.position.size = i+1;
    //     // joint_state_msg.position.capacity = i+1;

    //     joint_state_msg.velocity.data[i] = arr_velocity[i];
    //     // double vel = 5.0;
    //     // joint_state_msg.velocity.size = i+1;
    //     // joint_state_msg.velocity.capacity = i+1;

    //     rosidl_runtime_c__String__assign(&joint_state_msg.name.data[i], arr_name[i]);
    //     // joint_state_msg.name.size = i+1;
    //     // joint_state_msg.name.capacity = i+1;
    // }

    rcl_publish(&publish->publisher, &active->joint_state_msg, NULL);
}

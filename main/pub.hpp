#include <urosElement.hpp>
#include <sensor_msgs/msg/joint_state.h>
#include <vector>


// #define MAX_JOINTS (2)

class joint {

public:
    const char* name;
    double *position = NULL;
    double *velocity = NULL;
};


class pub : public urosElement {

public:
    pub(std::vector<joint>& joint);
    // pub(char** arr_name,double* arr_position,double* arr_velocity,int size);

    void init();

    static void pub_callback(rcl_timer_s* time, int64_t num);

    sensor_msgs__msg__JointState joint_state_msg;

private:

    rcl_timer_t timer;
    rcl_publisher_t publisher;
    static pub* publish;
    // static int size;
    static double* arr_position;
    static double* arr_velocity;
    static char** arr_name;
    static pub* active;
};
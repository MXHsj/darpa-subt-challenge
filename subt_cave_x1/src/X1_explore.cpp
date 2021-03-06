#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>
#include <vector>
#include <math.h>

geometry_msgs::Twist vel_command;
float last_ang_vel = 0.0;

std::vector<float> lidar_range;
std::vector<float> lidar_angle;

int num_sector = 180;

std::vector<float> calc_magnitude()
{
    std::vector<float> magnitude;
    float c = 1.0;
    float a = 5.0;
    float b = 1.0;
    for (int i = 0; i < lidar_range.size(); i++)
    {
        magnitude.push_back(c * c * (a - b * (lidar_range[i])));
    }
    return magnitude;
}

std::vector<int> calc_histogram(std::vector<float> mag)
{
    std::vector<float> histogram;
    std::vector<int> hist_bin;
    int sector_interval = round(mag.size() / num_sector);
    float threshold = 4.8;
    for (int i = 0; i < num_sector; i++)
    {
        float hist = 0;
        for (int j = 0; j < sector_interval; j++)
        {
            hist += mag[i * sector_interval + j];
        }
        histogram.push_back(hist);
    }
    for (int i = 0; i < histogram.size(); i++)
    {
        if (histogram[i] > threshold)
            hist_bin.push_back(1);
        else
            hist_bin.push_back(0);
    }
    return hist_bin;
}

int select_sector(std::vector<int> hist_bin)
{
    int curr_len = 0;
    int threshold = 1;
    bool isValley = false;
    std::vector<int> candidates;
    int mid = round(hist_bin.size() / 2);
    int valley_idx = mid;
    // find & store all admissible valleys
    for (int i = 0; i < hist_bin.size(); i++)
    {
        if (hist_bin[i] == 0 && !isValley)
        {
            isValley = true;
        }
        if ((hist_bin[i] == 1 && isValley) || (i == hist_bin.size() - 1))
        {
            isValley = false;
            if (curr_len >= threshold)
            {
                candidates.push_back(i - round(curr_len / 2));
            }
            curr_len = 0;
        }
        if (isValley)
        {
            curr_len++;
        }
    }
    // find the valley needs minimum turning
    int dist = 1000;
    for (int i = 0; i < candidates.size(); i++)
    {
        if (abs(candidates[i] - mid) < dist)
        {
            valley_idx = candidates[i];
            dist = abs(candidates[i] - mid);
        }
    }

    return valley_idx;
}

void vel_control(int valley_idx)
{
    float kp = 0.008;
    float error = valley_idx - num_sector / 2;
    // float ang_vel = 0.125 * kp * error + (1 - 0.125) * last_ang_vel; // IIR filter
    float ang_vel = kp * error;
    vel_command.linear.x = 0.4;
    vel_command.angular.z = ang_vel;
    last_ang_vel = ang_vel;
}

void vfh()
{
    std::vector<float> mag = calc_magnitude();
    std::vector<int> hist_bin = calc_histogram(mag);
    int valley_idx = select_sector(hist_bin);
    vel_control(valley_idx);

    // for debugging
    std::cout << "histogram:\n";
    for (int i = 0; i < hist_bin.size(); i++)
        std::cout << hist_bin[i];
    std::cout << "\nvalley_idx: " << valley_idx << " angular vel:" << vel_command.angular.z << std::endl;
}

void view_lidar()
{
    std::cout << "START ANGLES" << std::endl;
    for (int i = 0; i < lidar_angle.size(); i++)
    {
        std::cout << lidar_angle[i] << " ";
    }
    std::cout << "END ANGLES" << std::endl;

    std::cout << "START RANGES" << std::endl;
    for (int i = 0; i < lidar_range.size(); i++)
    {
        std::cout << lidar_range[i] << " ";
    }
    std::cout << "END RANGES" << std::endl;

    std::cout << "total lasers:" << lidar_range.size() << std::endl;
}

void lidar_callback(const sensor_msgs::LaserScan::ConstPtr &msg)
{
    lidar_range = msg->ranges;
    float beams = msg->angle_min;
    while (beams <= msg->angle_max)
    {
        lidar_angle.push_back(beams);
        beams += msg->angle_increment;
    }
    // view_lidar();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "explorer");

    ros::NodeHandle n;
    ros::Subscriber lidar_sub = n.subscribe("/X1/front_scan", 1000, lidar_callback);
    ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("X1/cmd_vel", 10);

    ROS_INFO("start exploration");

    ros::Rate loop_rate(5);
    while (n.ok())
    {
        ros::spinOnce();
        vfh();
        vel_pub.publish(vel_command);
        loop_rate.sleep();
    }

    // stop robot
    for (int i = 0; i < 10; i++)
    {
        vel_command.linear.x = 0.0;
        vel_command.angular.z = 0.0;
        vel_pub.publish(vel_command);
    }

    return 0;
}
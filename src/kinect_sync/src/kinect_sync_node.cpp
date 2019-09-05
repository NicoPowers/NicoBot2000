#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

// using namespace message_filters;

class Node
{
 public:
  Node()
  {
    sub_1_.subscribe(nh_, "/kinect2/sd/image_depth", 1);
    sub_2_.subscribe(nh_, "/kinect2/sd/camera_info", 1);
    sync_.reset(new Sync(MySyncPolicy(10), sub_1_, sub_2_));
    sync_->registerCallback(boost::bind(&Node::callback, this, _1, _2));
  }

  void callback(const sensor_msgs::ImageConstPtr &in1, const sensor_msgs::CameraInfoConstPtr &in2)
  {
    ROS_INFO("Synchronization successful");
  }

 private:
  ros::NodeHandle nh_;
  message_filters::Subscriber<sensor_msgs::Image> sub_1_;
  message_filters::Subscriber<sensor_msgs::CameraInfo> sub_2_;

  typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::CameraInfo> MySyncPolicy;
  typedef message_filters::Synchronizer<MySyncPolicy> Sync;
  boost::shared_ptr<Sync> sync_;
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "synchronizer");

  Node synchronizer;

  ros::spin();
}
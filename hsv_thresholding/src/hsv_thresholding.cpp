//include the necessary files

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;

static const std::string OPENCV_WINDOW = "Image window";
static const std::string THRESH_WIN = "Thresholded_Window";
// create a Mat data type to store the thresholded image
Mat frame_threshold;
//create a named window to show the thresholded image

    static int hue_low_slider=0;
   static int hue_high_slider=101;
   static int saturation_low_slider=82;
   static int saturation_high_slider=255;
   static int value_low_slider=100;
  static  int value_high_slider=255;



void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    int i=0;
    cout<<"Here for the"<<i++<<" th time\n";
    cv_bridge::CvImagePtr cv_ptr;
    cout<<"bridge declaration done\n";
    // use a try handling mechanism for better error handling
    try
    {
      //use cv_bridge to convert the images from ROS to Open CV type
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    cout<<"bridging done\n";
    // Show the Original image
    cout<<"Showing the original image\n";
    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    /// Initialize values of the differrent sliders

   //Create 6 trackbars on the OPENCV_WINDOW window
   

   // Actual Tresholding is done here on the original mage and is stored in  the mat object we created
    inRange(cv_ptr->image,Scalar(hue_low_slider,saturation_low_slider,value_low_slider), Scalar(hue_high_slider,saturation_high_slider,value_high_slider),frame_threshold);
    //Show the Thresholded Image
    imshow(THRESH_WIN,frame_threshold);
    waitKey(1);

  // Now that Trackbars are created let's use it
  // Use an infinite while loop to instataneously threshold the image when and as the trackbar values change
  //If " q " is pressed then exit
  
 /* while((char)waitKey(1)=='q'){
    
    // Actual Tresholding is done here on the original mage and is stored in  the mat object we created
    inRange(cv_ptr->image,Scalar(hue_low_slider,saturation_low_slider,value_low_slider), Scalar(hue_high_slider,saturation_high_slider,value_high_slider),frame_threshold);
    //Show the Thresholded Image
    imshow(THRESH_WIN,frame_threshold);
  }*/
}
int main(int argc, char** argv)
{
    //Mat image1=imread("/home/sangeet/fw.png");
    cout<<"I am Here at the beginning\n";
    ros::init(argc, argv, "hsv_thresholding_node");
    cout<<"I am initialised\n";

    ros::NodeHandle nh ;
    cout<<"Node Handle created\n";
    image_transport::ImageTransport it(nh);
    cout<<"Image Transport done\n";
    cout<<"Lets make a call to the Call Back Function\n";
    cout<<"Create 6 taskbars\n";   
    //SHowing this here because I got some error

    //cv::imshow(OPENCV_WINDOW, image1);

    namedWindow(OPENCV_WINDOW, 1);
    cv::createTrackbar( "h_low", OPENCV_WINDOW, &hue_low_slider, 179);
    cv::createTrackbar( "h_high", OPENCV_WINDOW, &hue_high_slider, 179);
    cv::createTrackbar( "s_low", OPENCV_WINDOW, &saturation_low_slider, 255);
    cv::createTrackbar( "s_high", OPENCV_WINDOW, &saturation_high_slider, 255);
    cv::createTrackbar( "v_low", OPENCV_WINDOW, &value_low_slider, 255);
    cv::createTrackbar( "v_high", OPENCV_WINDOW, &value_high_slider, 255);
    image_transport::Subscriber sub = it.subscribe("/kraken/front_camera", 1, imageCb);
    image_transport::Publisher pub = it.advertise("/image_converter/output_video",1);
    ros::spin();
    return 0;

}

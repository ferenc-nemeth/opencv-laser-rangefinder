/**
 * @file    laser.cpp
 * @author  Ferenc Nemeth
 * @date    18 Sept 2018
 * @brief   Demonstration software of a laser rangefinder.
 *          This project shows how to measure distance with a laser pointer and a webcamera.
 *
 *          Copyright (c) 2018 Ferenc Nemeth - https://github.com/ferenc-nemeth/
 */ 

#include <iostream>
#include <math.h>
#include "opencv2/opencv.hpp"

int32_t main(void)
{
  /* Magic constants. If you want to make your own version, you have to modify these values for your needs. */
  static constexpr int32_t cam_number     = 1;                 /**< The number of the camera, the 0 is the built in my computer. */
  static constexpr int32_t cam_width      = 640;               /**< Width of the video's resolution. */
  static constexpr int32_t cam_height     = 480;               /**< Height of the video's resolution. */
  static constexpr int32_t threshold_min  = 245;               /**< Minimum value of the binary threshold. */
  static constexpr int32_t threshold_max  = 255;               /**< Maximum value of the binary threshold. */

  /* Look-up table for linear interpolation. If you want to make your own version, you have to re-measure these values. */
  static std::vector<double> pixel  = {42.0, 94.0, 122.0, 139.0, 150.0, 157.0, 163.0, 168.0, 171.0};  /**< Measured values of pixels. */
  static std::vector<double> cm     = {20.0, 30.0, 40.0,  50.0,  60.0,  70.0,  80.0,  90.0,  100.0};  /**< Measured values of centimeters. */
  
  /* Initialize the video formats. */
  cv::Mat video;
  cv::Mat video_gray;
  cv::Mat video_black_white;

  /* Start the video capture with a prescribed resolution. */
  cv::VideoCapture cap(cam_number);
  cap.set(cv::CAP_PROP_FRAME_WIDTH, cam_width);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, cam_height);

  /* If there is a error while opening, abort. */
  if(!cap.isOpened()) 
  {
    std::cerr << "No camera detected!\n";
		return -1;
  }

  /* Create the window. */
  namedWindow("Laser rangefinder", cv::WINDOW_AUTOSIZE);

  while(true)
  {    
    try
    {
      /* Vectors for contours and hierarchy. */
      std::vector<std::vector<cv::Point>> contours;
      std::vector<cv::Vec4i> hierarchy;

      /* Get a new frame from the camera, convert it to grayscale, then make into black&white with binary threshold. */
      cap >> video;
      cv::cvtColor(video, video_gray, cv::COLOR_RGB2GRAY);
      cv::threshold(video_gray, video_black_white, threshold_min, threshold_max, cv::THRESH_BINARY);

      /* Get contours with full hierararchy and simple approximation. */
      cv::findContours(video_black_white, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0)); 

      /* If there are no contours, skip everything, otherwise there would be an exception. */    
      if (contours.size())
      {
        /* Get moments. */
        cv::Moments m = cv::moments(contours[0]);
        /* Protection from divison by zero. */
        if (m.m00 > 0.0)
        {
          /* Calculate the x,y coordinates of the laser point. */
          double coord_x = m.m10 / m.m00;
          double coord_y = m.m01 / m.m00;

          /* Make sure, that we are in the look-up table's range. */
          if ((coord_y > pixel[0]) && (coord_y < pixel[pixel.size()-1]))
          {
            /* Find the place of the coordinate in the look-up table. */
            uint32_t i = 0;
            while (coord_y > pixel[i+1])
            {
              i++;
            }
            /* Calculate the value with linear interpolation. */
            double distance = cm[i] + ((coord_y - pixel[i]) * (cm[i+1] - cm[i]) / (pixel[i+1] - pixel[i]));

            std::cout << "X: "<< coord_x << "\tY: " << coord_y << "\tDistance: " << distance << "\n";

            /* Draw a circle on the laser and put a text with the distance on it. */
            cv::circle(video, cv::Point(coord_x, coord_y), 5, cv::Scalar(0,0,0), 1, 8);
            cv::putText(video, std::to_string(distance), cv::Point(coord_x, coord_y), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 0.5, cv::Scalar(255,0,0), 1);
          }
        }
      }
      /* Show the picture. */
      imshow("Laser rangefinder", video);

      /* Press any key to exit. */
      if(cv::waitKey(30) >= 0)
      {
        break;
      }
    }
    /* Write out if there is an error. */
    catch (std::exception& e)
    {  
      std::cerr << e.what() << "\n";
    }
  }
  return 0;
}


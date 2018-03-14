// cutVideo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <boost/algorithm/string.hpp>

int main(int argc, char *argv[])
{
	if (argc < 4)
	{
		std::cout << "wrong parameter count" << std::endl;
	}
	std::string videoPath = argv[1];
	std::string startTime = argv[2];
	std::string endTime = argv[3];
	if (!boost::filesystem::exists(videoPath))
	{
		std::cout << "video path doesn't exist" << std::endl;
		return 1;
	}

	cv::VideoCapture capture;
	capture.open(videoPath);
	if (!capture.isOpened())
	{
		std::cout << "cannot open " << videoPath << std::endl;
		return 1;
	}

	int fps = (int)capture.get(CV_CAP_PROP_FPS);
	int frameW = (int)capture.get(CV_CAP_PROP_FRAME_WIDTH);
	int frameH = (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	
	std::vector<std::string> vStart;
	std::vector<std::string> vStop;
	boost::algorithm::split(vStart, startTime, boost::is_any_of(":"));
	boost::algorithm::split(vStop, endTime, boost::is_any_of(":"));
	if (vStart.size() != 3)
	{
		std::cout << "invalid start time: " << startTime << std::endl;
	}
	if (vStop.size() != 3)
	{
		std::cout << "invalid stop time: " << endTime << std::endl;
	}

	int startSeconds = std::stoi(vStart[0]) * 3600 + std::stoi(vStart[1]) * 60 + std::stoi(vStart[2]);
	int stopSeconds = std::stoi(vStop[0]) * 3600 + std::stoi(vStop[1]) * 60 + std::stoi(vStop[2]);
	
	int startFrameNum = startSeconds*fps;
	int stopFrameNum = stopSeconds*fps;

	capture.set(CV_CAP_PROP_POS_FRAMES, startFrameNum);
	cv::Mat image;
	
	int f = startFrameNum;
	boost::filesystem::path p(videoPath);
	std::string fileName = p.stem().string();
	
	fileName += "_cut.av8";
	std::string outPath = p.parent_path().append(fileName).string();
	cv::VideoWriter out(outPath, CV_FOURCC('M', 'P', '4', '2'), fps,  cv::Size(frameW, frameH), true);
	while (capture.read(image))
	{
		out.write(image);

		++f;
		if (f > stopFrameNum)
		{
			break;
		}
	}
	out.release();
	std::cout << "Done" << std::endl;
    return 0;
}


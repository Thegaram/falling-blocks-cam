#ifndef FACEDETECTOR_H_DEFINED
#define FACEDETECTOR_H_DEFINED

#include <opencv2/core/core.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

class FaceDetector {
private:
	
	cv::CascadeClassifier faceCascade;
	std::string cascadeFileName = "lbpcascade_frontalface.xml";
	
	std::string windowName;

	int camNumber;
	cv::VideoCapture camera;

	cv::Mat frame;

public:

	FaceDetector(std::string name = "FaceDetector", int camNum = 0) {
		windowName = name;
		camNumber = camNum;
	}
	void load();
	float detect();
	void read();
	//std::string getWindowName() { return windowName; }
	//cv::Mat detectedFrame() { return frame; }

};

#endif // FACEDETECTOR_H_DEFINED
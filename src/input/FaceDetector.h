#ifndef FACEDETECTOR_H_DEFINED
#define FACEDETECTOR_H_DEFINED

#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

class FaceDetector {
private:

	cv::CascadeClassifier faceCascade;
	cv::CascadeClassifier smileCascade;

	//std::string cascadeFileName = "lbpcascade_frontalface.xml";
	std::string cascadeFileName = "assets/xmls/haarcascade_frontalface_alt.xml";
	std::string smileCascadeFileName = "assets/xmls/haarcascade_smile.xml";

	std::string windowName;

	int camNumber;
	cv::VideoCapture camera;

	cv::Mat frame;

public:

	double headPosX;
	double headPosY;

	bool isSmiling;

	FaceDetector(std::string name = "FaceDetector", int camNum = 0) {
		windowName = name;
		camNumber = camNum;
		headPosX = 0.5;
		headPosY = 0.5;
		isSmiling = false;
	}
	void load();
	void detect();
	void read();
	//std::string getWindowName() { return windowName; }
	//cv::Mat detectedFrame() { return frame; }

};

#endif // FACEDETECTOR_H_DEFINED
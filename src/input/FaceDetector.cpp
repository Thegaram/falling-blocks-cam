#include "FaceDetector.h"
#include <string>


/**
* Open camera stream 
* Load pretrained XML file
*/
void FaceDetector::load() {
	
	camera.open(camNumber);
	try {
		faceCascade.load(cascadeFileName);
	}
	catch(cv::Exception e){}
	if (faceCascade.empty()) {
		std::cerr << "ERROR: Couldn't load Face Detector: ";
		std::cerr << cascadeFileName << std::endl;
		exit(1);
	}
}

/**
* Detecting and showing the image
*/
float FaceDetector::detect() {
	float xCoord = 0.5f;
	std::vector<cv::Rect> faces;
	cv::Mat grayFrame;

	// Grayscale color conversion:
	if (frame.channels() == 3) {
		cvtColor(frame, grayFrame, CV_BGR2GRAY);
	}
	else if (frame.channels() == 4) {
		cvtColor(frame, grayFrame, CV_BGRA2GRAY);
	}
	else {
		grayFrame = frame;
	}
		
	// Shrinking the camera image to increase speed:
	/*const int DETECTION_WIDTH = 320;
	cv::Mat smallImg;
	float scale = frame.cols / (float)DETECTION_WIDTH;
	if (frame.cols > DETECTION_WIDTH) {
		int scaledHeight = cvRound(frame.rows / scale);
		cv::resize(frame, smallImg, cv::Size(DETECTION_WIDTH, scaledHeight));
	}
	else {
		smallImg = frame;
	}*/

	// Histogram equalization:
	equalizeHist(grayFrame, grayFrame);   				

	// Detecting the face:												
	faceCascade.detectMultiScale(grayFrame, faces, 1.1f, 4, cv::CASCADE_SCALE_IMAGE, cv::Size(80, 80));



	// Iterate over all of the faces
	for (size_t i = 0; i < faces.size(); i++) {

		// Find center of faces
		cv::Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);

		// Draw ellipse around face
		cv::ellipse(frame, center, cv::Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, cv::Scalar(0, 255, 255), 4, 8, 0);
		xCoord = (float)center.x / camera.get(CV_CAP_PROP_FRAME_WIDTH);

		//cv::imshow(detector.getWindowName(), detector.detectedFrame());
		
	}

	cv::imshow(windowName, frame);
	cv::waitKey(5);
	return xCoord;
}

/**
* Reading the actual camera image
*/
void FaceDetector::read() {
	camera.read(frame);
}
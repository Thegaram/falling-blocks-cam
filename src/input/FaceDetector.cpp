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
	const int DETECTION_WIDTH = 320;
	cv::Mat smallImg;
	float scale = grayFrame.cols / (float)DETECTION_WIDTH;
	if (grayFrame.cols > DETECTION_WIDTH) {
		int scaledHeight = cvRound(frame.rows / scale);
		cv::resize(grayFrame, smallImg, cv::Size(DETECTION_WIDTH, scaledHeight));
	}
	else {
		smallImg = frame;
	}

	// Histogram equalization:
	equalizeHist(smallImg, smallImg);

	// Detecting the face:
	faceCascade.detectMultiScale(smallImg, faces, 1.1f, 4, cv::CASCADE_SCALE_IMAGE, cv::Size(40, 40));



	// Iterate over all of the faces
	for (size_t i = 0; i < faces.size(); i++) {

		// Find center of faces
		cv::Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);

		// Draw ellipse around face
		cv::ellipse(smallImg, center, cv::Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, cv::Scalar(0, 255, 255), 4, 8, 0);
		headPosX = (double)center.x / DETECTION_WIDTH;

		//cv::imshow(detector.getWindowName(), detector.detectedFrame());

	}

	cv::imshow(windowName, smallImg);
	cv::waitKey(5);
	return headPosX;
}

/**
* Reading the actual camera image
*/
void FaceDetector::read() {
	cv::Mat temp;
	camera.read(temp);
	cv::flip(temp, frame, 1);
}
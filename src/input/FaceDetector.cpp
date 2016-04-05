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
		smileCascade.load(smileCascadeFileName);
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
void FaceDetector::detect() {
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
	int scaledHeight = frame.rows;
	cv::Mat smallImg;
	float scale = grayFrame.cols / (float)DETECTION_WIDTH;
	if (grayFrame.cols > DETECTION_WIDTH) {
		scaledHeight = cvRound(frame.rows / scale);
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
		// headPosX = (double)center.x / DETECTION_WIDTH;
		headPosX = ((double)center.x / DETECTION_WIDTH - 0.25) / 0.5;
		headPosY = (double)center.y / scaledHeight;

		// detect smile
        std::vector<cv::Rect> smiles;
		cv::Mat smallImgROI = smallImg(faces[i]);
		smileCascade.detectMultiScale(smallImgROI, smiles, 1.1, 20, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(20, 20));

		if (smiles.size() > 0)
		{
			isSmiling = true;

			// Draw ellipse around smile
			cv::Point center2(faces[i].x + smiles[0].x + smiles[0].width / 2, faces[i].y + smiles[0].y + smiles[0].height / 2);
			cv::ellipse(smallImg, center2, cv::Size(smiles[0].width / 2, smiles[0].height / 2), 0, 0, 360, cv::Scalar(0, 255, 255), 4, 8, 0);
		}
		else
		{
			isSmiling = false;
		}
	}

	cv::Mat outputImage;
	cv::resize(smallImg, outputImage, cv::Size(1.5 * DETECTION_WIDTH, 1.5 * scaledHeight));
	cv::imshow(windowName, outputImage);


	cv::waitKey(5);
}

/**
* Reading the actual camera image
*/
void FaceDetector::read() {
	cv::Mat temp;
	camera.read(temp);
	cv::flip(temp, frame, 1);
}

void FaceDetector::cleanup() {
	cv::destroyWindow(windowName);
}
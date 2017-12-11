// 171211 single camera calibration
// by Junchao
#include "SigCameraCalibration.h"

// record pictures of chessboard
const string fileListPath = "..\\chessboards\\PicturePathList.cof";

bool SigCameraCalibration::getPicturePath()
{
	ifstream inputStream;
	inputStream.open(fileListPath, ios::in);
	if (!inputStream.is_open()) return false;

	string sPicPath = "";
	while (inputStream >> sPicPath) {
		vecStrPicPath.push_back(sPicPath);
	}
	inputStream.close();
	return true;
}

int SigCameraCalibration::addChessboardPoints(Size& boardSize) {
	// record success detect chessboard
	int successes = 0;
	vector<Point2f> imageCorners;
	vector<Point3f> worldCorners;

	for (int i = 0; i < boardSize.height; ++i) {
		for (int j = 0; j < boardSize.width; ++j) {
			worldCorners.push_back(Point3f(i, j, 0.0f));
		}
	}
	for (int i = 0; i < vecStrPicPath.size(); ++i) {
		Mat image;
		image = imread(vecStrPicPath[i], 0);
		bool isDetectdChessboard = findChessboardCorners(image, boardSize, imageCorners);

		if (isDetectdChessboard && imageCorners.size() == boardSize.area()) {
			cornerSubPix(image, imageCorners,
				Size(5, 5),
				Size(-1, -1),
				TermCriteria(cv::TermCriteria::MAX_ITER +
					cv::TermCriteria::EPS,
					30,
					0.1)
			);
			addPoints(imageCorners, worldCorners);
			successes++;

		}
		// show result
		drawChessboardCorners(image, boardSize, imageCorners, isDetectdChessboard);
		imshow("chessboard detect", image);
		waitKey(100);
	}
	return successes;
}

void SigCameraCalibration::addPoints(vector<Point2f>& imageCorners,
	vector<Point3f>& worldCorners) {
	imagePoints.push_back(imageCorners);
	worldPoints.push_back(worldCorners);
}

double SigCameraCalibration::calibrate(Size& imageSize, bool isRestoreToFile) {
	vector<Mat> rvecs, tvecs;

	isMustInitUndist = true;
	double val = calibrateCamera(worldPoints,
		imagePoints,
		imageSize,
		cameraMatrix,
		distParam,
		rvecs, tvecs,
		flag);
	if (isRestoreToFile) {
		FileStorage output("camera_para.xml", FileStorage::WRITE);
		output << "cameraMatrix" << getCameraMatrix();
		output << "DistPara" << getDistParam();
		output.release();
	}
	return val;
}
Mat SigCameraCalibration::remap(const Mat& image)
{
	Mat unDistort;
	if (isMustInitUndist) {
		initUndistortRectifyMap(cameraMatrix,
			distParam,
			Mat(),
			Mat(),
			image.size(),
			CV_32FC1,
			map1, map2);
		isMustInitUndist = false;
	}
	cv::remap(image, unDistort, map1, map2, cv::INTER_LINEAR);
	return unDistort;
}
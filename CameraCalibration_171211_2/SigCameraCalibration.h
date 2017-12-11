#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

class SigCameraCalibration {
private:
	// variable
	vector<string> vecStrPicPath;
	// world and image coordinate
	vector<vector<Point3f>> worldPoints;
	vector<vector<Point2f>> imagePoints;
	// variable
	Mat cameraMatrix;  // camera matrix
	Mat distParam;     // distortion matrix
	int flag;          // specify how calibration is done
	bool isMustInitUndist;
	Mat map1, map2;    // used in undistort
public:
	// cosntructor
	SigCameraCalibration() : flag(0) {
		// picture path initial
		vecStrPicPath.reserve(10);
		// must init undistort
		isMustInitUndist = true;
	}
	// destructor
	~SigCameraCalibration() {}
	// function
	// read picture path from chessboards\\PicturePathList.cof
	// exist or not exist this file
	bool getPicturePath();
	// find chessboard points restore image points and world points
	int addChessboardPoints(Size& boardSize);
	// add world and image coordinate
	void addPoints(vector<Point2f>& imageCorners, vector<Point3f>& worldCorners);
	// camera calibration
	double calibrate(Size& imageSize, bool isRestoreToFile);
	// image remap
	Mat remap(const Mat& image);
	// read camera metrix and distort param
	void loadCamMetrix(bool isNeedLoad) {
		if (isNeedLoad) {
			FileStorage input("camera_para.xml", FileStorage::READ);
			if (!input.isOpened()) {
				cout << "file is not exist." << endl;
				return;
			}
			input["cameraMatrix"] >> cameraMatrix;
			input["DistPara"] >> distParam;
			input.release();
		}
	}
	Mat getCameraMatrix() { return cameraMatrix; }
	Mat getDistParam() { return distParam; }
	string getPicturePath(int i) { return vecStrPicPath[i]; }
};
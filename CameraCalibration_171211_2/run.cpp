#include "SigCameraCalibration.h"

int main(void) {
	bool isNeedUnDistort = false;     // whether to undistort
	bool isNeedLoad = true;           // from file to load
	int  selectUndistort = 1;         // select picture to deal
	SigCameraCalibration camCal;
	camCal.getPicturePath();
	if (isNeedUnDistort) {
		double dChessPic = camCal.addChessboardPoints(Size(6, 4));
		camCal.calibrate(Size(320, 240), true/*store parameter to file*/);
		cout << "detect chess numbers:" << dChessPic << endl;
		cout << "camera matrix:\n" << camCal.getCameraMatrix() << endl;
		cout << "distortion parameter:\n" << camCal.getDistParam() << endl;
	}
	camCal.loadCamMetrix(isNeedLoad);
	Mat unDistort = camCal.remap(imread(camCal.getPicturePath(selectUndistort)));
	imshow("distort image", unDistort);
	waitKey(0);
	return 0;
}
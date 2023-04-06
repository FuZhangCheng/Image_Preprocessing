#include	<iostream>
#include	<opencv2/core/core.hpp>
#include	<opencv2/highgui/highgui.hpp>
#include	"Supp.h"

using namespace std;
using namespace cv;

int main() {
	Mat i(25, 25, CV_8U);

	int const	noOfImagePerCol = 16, noOfImagePerRow = 16;
	Mat			largeWin, win[noOfImagePerRow * noOfImagePerCol], legend[noOfImagePerRow * noOfImagePerCol];

	createWindowPartition(i, largeWin, win, legend, noOfImagePerCol, noOfImagePerRow, 1, 1);

	for (int n = 0; n < 256; n++) {
		i = n;
		i.copyTo(win[n]);
	}

	imshow("images", largeWin);

	waitKey(0);
	destroyAllWindows();
	return 0;

}

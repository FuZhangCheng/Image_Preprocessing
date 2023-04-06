#include	<iostream>
#include	<opencv2/core/core.hpp>
#include	<opencv2/highgui/highgui.hpp>
#include	"Supp.h"

using namespace std;
using namespace cv;

int main() {
	// image is the original image
	// dst is the blurred image
	Mat		image, dst;

	image = imread("Inputs/Images/Moon.jpg", IMREAD_GRAYSCALE);

	GaussianBlur(image, dst, Size(5, 5), 1);

	// img is the result of (original image - blurred image)
	Mat img = (image - dst);

	int const	noOfImagePerCol = 2, noOfImagePerRow = 2;
	Mat			largeWin, win[noOfImagePerRow * noOfImagePerCol], legend[noOfImagePerRow * noOfImagePerCol];

	createWindowPartition(image, largeWin, win, legend, noOfImagePerCol, noOfImagePerRow);

	image.copyTo(win[0]);
	putText(legend[0], "Original", Point(5, 11), 1, 1, Scalar(250, 250, 250), 1);

	dst.copyTo(win[1]);
	putText(legend[1], "Blurred", Point(5, 11), 1, 1, Scalar(250, 250, 250), 1);

	img.copyTo(win[2]);
	putText(legend[2], "Mask", Point(5, 11), 1, 1, Scalar(250, 250, 250), 1);

	double c = 15;
	Mat unsharp_mask = (image + c * img); //This is the result of image after unsharp masking

	unsharp_mask.copyTo(win[3]);
	putText(legend[3], "Result", Point(5, 11), 1, 1, Scalar(250, 250, 250), 1);

	imshow("Unsharp mask", largeWin);
	
	waitKey(0);

	destroyAllWindows();
	return 0;

}

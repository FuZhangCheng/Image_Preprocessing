#include	<iostream>
#include	<opencv2/core/core.hpp>
#include	<opencv2/highgui/highgui.hpp>
#include	"Supp.h"

using namespace std;
using namespace cv;

void swapQuadrants(Mat magI) {
	// rearrange the quadrants of spectrum image so that the origin is at the image center
	int		cx = magI.cols / 2;
	int		cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat		tmp;                        // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);
}

int main() {
	Mat image = imread("Inputs/Images/SaturnRing.jpg", IMREAD_GRAYSCALE);
	if (image.empty()) {
		cout << "Cannot open image" << endl;
		return 1;
	}

	Mat padded;
	int m = getOptimalDFTSize(image.rows);
	int n = getOptimalDFTSize(image.cols);
	cout << "Original rows: " << image.rows << endl;
	cout << "Optimal rows: " << m << endl;
	cout << "Original columns: " << image.cols << endl;
	cout << "Optimal columns: " << n << endl;
	copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat		planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat		complexI, tmp2, tmp, magI;

	int const	noOfImagePerCol = 1, noOfImagePerRow = 4;
	Mat			largeWin, win[noOfImagePerRow * noOfImagePerCol], legend[noOfImagePerRow * noOfImagePerCol];
	createWindowPartition(padded, largeWin, win, legend, noOfImagePerCol, noOfImagePerRow);

	padded.copyTo(win[0]); // create display window 1
	putText(legend[0], "Padded original", Point(5, 11), 1, 1, Scalar(250, 250, 250), 1);

	// Perform DFT operation
	merge(planes, 2, complexI);
	dft(complexI, complexI);
	split(complexI, planes);

	// Draw spectrum of image
	magnitude(planes[0], planes[1], magI);

	magI = magI + 1;
	log(magI, magI);

	tmp = convertGrayFloat2GrayImage(magI);
	swapQuadrants(tmp);

	tmp.copyTo(win[1]); // create display window 1
	putText(legend[1], "Spectrum", Point(5, 11), 1, 1, Scalar(250, 250, 250), 1);

	// Draw H Filtering mask to multiply with F image
	double space = 5;
	Mat H(m, n, CV_32F, Scalar::all(255));
	line(H, Point(m / 2, 0), Point(m / 2, n / 2 - space), Scalar(0, 0, 0), 1, LINE_8);
	line(H, Point(m / 2, n), Point(m / 2, n / 2 + space), Scalar(0, 0, 0), 1, LINE_8);

	tmp = convertGrayFloat2GrayImage(H);
	tmp.copyTo(win[2]); // create display window 1
	putText(legend[2], "Filter Mask", Point(5, 11), 1, 1, Scalar(250, 250, 250), 1);

	// Perform multiplication of F image and H filtering mask
	swapQuadrants(planes[0]);
	swapQuadrants(planes[1]);
	multiply(H, planes[0], planes[0]);
	multiply(H, planes[1], planes[1]);
	swapQuadrants(planes[0]);
	swapQuadrants(planes[1]);

	// Perform inverse DFT operation
	merge(planes, 2, complexI); // create back the complex image
	dft(complexI, complexI, DFT_SCALE | DFT_INVERSE); // do inverse
	split(complexI, planes);

	tmp = convertGrayFloat2GrayImage(planes[0]);
	tmp.copyTo(win[3]); // create display window 1
	putText(legend[3], "Result", Point(5, 11), 1, 1, Scalar(250, 250, 250), 1);

	imshow("DFT", largeWin);

	waitKey(0);

	destroyAllWindows();
	return 0;

}
#include	<iostream>
#include	<opencv2/core/core.hpp>
#include	<opencv2/highgui/highgui.hpp>
#include	"Supp.h"

using namespace std;
using namespace cv;

int main() {
	String input_filename = "Inputs/Videos/Burglary006_x264.mp4";
	String output_filename = "out.mp4";
	Mat				inputImage, input, grayscale; // declare an image object to store each frame grabbed from a video
	VideoCapture	inputVideo(input_filename); // Open a video file for reading
	VideoWriter		outputVideo;  // create an output object
	int				width = (int)inputVideo.get(CAP_PROP_FRAME_WIDTH), // get video width & height
		height = (int)inputVideo.get(CAP_PROP_FRAME_HEIGHT);
	double			fps = inputVideo.get(CAP_PROP_FPS); // get frame rate per second
	int				totalFrame = inputVideo.get(CAP_PROP_FRAME_COUNT);

	//To create a 1 * 2 window partition for 2 sub-images.
	int const	noOfImagePerCol = 1, noOfImagePerRow = 3;
	Mat			largeWin, win[noOfImagePerRow * noOfImagePerCol], legend[noOfImagePerRow * noOfImagePerCol];

	if (!inputVideo.isOpened()) { // Check if video file can be opened
		cout << "Could not open video file for reading!" << endl;
		return -1;
	}

	// set output video properties
	// The line below creates a Size object to record the size of each image of a video
	Size	S = Size(width * noOfImagePerRow, height * noOfImagePerCol);

	// The line below open a video file for writing according to the input parameters.
	// DIV3 is the DivX MPEG-4 compression code to set the compression type of the video file.
	// see http://www.fourcc.org/codecs.php for fourcc. 
	outputVideo.open(output_filename, cv::VideoWriter::fourcc('D', 'I', 'V', '3'), fps, S, true);

	if (!outputVideo.isOpened()) {
		cout << "Could not open the output video for write: " << endl;
		return -1;
	}

	// Extact the first frame of the video
	Mat background;
	inputVideo >> background;
	if (background.empty()) {
		cout << "No background image!";
		return -1;
	}
	cvtColor(background, input, COLOR_BGR2GRAY);
	cvtColor(input, background, COLOR_GRAY2BGR);

	for(int n = 1; n < totalFrame; n++) { // Extract all frames to write into a new video file

		inputVideo >> inputImage; // Read one frame/image at a time

		if (inputImage.empty()) {
			cout << " < < <  Game over!  > > > ";
			break;
		}

		createWindowPartition(inputImage, largeWin, win, legend, noOfImagePerCol, noOfImagePerRow);
		background.copyTo(win[0]);
		putText(legend[0], "Original RGB color format", Point(5, 11), 1, 1, Scalar(250, 250, 250), 1);

		cvtColor(inputImage, input, COLOR_BGR2GRAY); //
		cvtColor(input, win[1], COLOR_GRAY2BGR); // convert colored video into grayscale video
		putText(legend[1], "Gray scale", Point(5, 11), 1, 1, Scalar(250, 250, 250), 1);

		win[2] = abs(win[1] - win[0]);
		putText(legend[2], "Result", Point(5, 11), 1, 1, Scalar(250, 250, 250), 1);

		imshow("Video", largeWin); // display the read video image
		outputVideo << largeWin; // write image into new video file
		waitKey(1);
	}
	cout << endl << "\nFinished writing" << endl;
	waitKey(0);
	destroyAllWindows();
	return 0;
}
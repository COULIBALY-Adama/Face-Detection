////****************************************************************************************************////
////                          Face detection and recognition  								    		////
////          This program was implemented based on the tutorial at:              					    ////
////     	http://docs.opencv.org/2.4/modules/contrib/doc/facerec/facerec_tutorial.html                ////
////																									////
////             Author: COULIBALY ADama                             									////
////																									////
////             Compilation: 1- make      										        				////
////						  2. ./face_detection image_name.jpg		 								////
////						    													    			    ////
////																									////
////		     Description: This program can detect faces in images        	    					////
////                                   																	////
////****************************************************************************************************////

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

void detectAndDisplay(Mat frame);

//Global variable
String face_cascade_name =
		"/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
String eyes_cascade_name =
		"/usr/share/opencv/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
String nose_cascade_name =
		"/usr/share/opencv/haarcascades/haarcascade_mcs_nose.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
CascadeClassifier nose_cascade;

String window_name = "Capture - Face detection";

//Main function
int main(int argc, const char *argv[]) {

	Mat frame;
	string chemin_image = string(argv[1]);

	//Loading descriptors
	if (!face_cascade.load(face_cascade_name)) {
		printf("--(!)Error loading face cascade\n");
		return -1;
	};
	if (!eyes_cascade.load(eyes_cascade_name)) {
		printf("--(!)Error loading eyes cascade\n");
		return -1;
	};
	if (!nose_cascade.load(nose_cascade_name)) {
		printf("--(!)Error loading eyes cascade\n");
		return -1;
	};

	//Image reading
	frame = imread(chemin_image, 1);

	//Face detection
	detectAndDisplay(frame);

	waitKey(0);

	return 0;
}

// Detecting faces in the test imag
void detectAndDisplay(Mat frame) {

	Size eye_size;
	Size nose_size;
	double echelle;
	int nb_voisins;

	//images resizing
	if ((frame.cols > frame.rows) && (frame.cols * frame.rows > 1200000)) {
		resize(frame, frame, Size(1200, 1000), 1.0, 1.0, CV_INTER_AREA);
		eye_size = Size(5, 5);
		nose_size = Size(15, 15);
		echelle = 1.1;
		nb_voisins = 2;
	}
	if ((frame.cols > frame.rows) && (frame.cols * frame.rows < 1200000)) {
		resize(frame, frame, Size(1200, 1000), 1.0, 1.0, CV_INTER_LINEAR);
		eye_size = Size(5, 5);
		nose_size = Size(15, 15);
		echelle = 1.1;
		nb_voisins = 2;
	}
	if ((frame.cols <= frame.rows) && (frame.cols * frame.rows > 55000)) {
		resize(frame, frame, Size(220, 250), 1.0, 1.0, CV_INTER_AREA);
		eye_size = Size(30, 30);
		nose_size = Size(50, 50);
		echelle = 1.01;
		nb_voisins = 1;
	}
	if ((frame.cols <= frame.rows) && (frame.cols * frame.rows < 55000)) {
		resize(frame, frame, Size(220, 250), 1.0, 1.0, CV_INTER_LINEAR);
		eye_size = Size(30, 30);
		nose_size = Size(50, 50);
		echelle = 1.01;
		nb_voisins = 1;
	}
	std::vector<Rect> faces;
	Mat frame_gray;

	//Converting to Grayscale Image
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

	//Original image display
	imshow("image originale", frame);

	//-- Face detection
	face_cascade.detectMultiScale(frame_gray, faces, echelle, nb_voisins, 0,
			Size(30, 30), Size(200, 200));

	vector<int> faces_index;

	//Face discrimination
	for (size_t i = 0; i < faces.size(); i++) {

		vector<Rect> eyes;
		Mat face_region = frame_gray(faces[i]);

		// Detection of eyes in the considered face
		eyes_cascade.detectMultiScale(face_region, eyes, 1.1, 3, 0, eye_size);


		vector<Rect> nose;

		// Detection of the nose in the face considered
		nose_cascade.detectMultiScale(face_region, nose, 1.1, 3, 0, nose_size);

		//Critère d'acceptation
		if (eyes.size() + nose.size() >= 1) {
			faces_index.push_back(i);
			Point pt1(faces[i].x, faces[i].y); // Display detected faces on main window - live stream from camera
			Point pt2((faces[i].x + faces[i].height),
					(faces[i].y + faces[i].width));
			rectangle(frame, pt1, pt2, Scalar(255, 0, 255), 2, 8, 0);

//			for (size_t j = 0; j < eyes.size(); j++) {
//
//				Point pt3(eyes[j].x + faces[i].x, eyes[j].y + faces[i].y); // Display detected faces on main window - live stream from camera
//				Point pt4((eyes[j].x + faces[i].x + eyes[j].height),
//						(eyes[j].y + faces[i].y + eyes[j].width));
//				rectangle(frame, pt3, pt4, Scalar(0, 0, 255), 2, 8, 0);
//			}
//
//			for (size_t j = 0; j < nose.size(); j++) {
//
//				Point pt7(nose[j].x + faces[i].x, nose[j].y + faces[i].y); // Display detected faces on main window - live stream from camera
//				Point pt8((nose[j].x + faces[i].x + nose[j].height),
//						(nose[j].y + faces[i].y + nose[j].width));
//				rectangle(frame, pt7, pt8, Scalar(0, 255, 255), 2, 8, 0);
//			}

		}
	}

	imshow(window_name, frame);
}

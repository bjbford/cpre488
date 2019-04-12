///////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       //
// File name: main.cpp                                                                   //
//                                                                                       //
// Description: Track CPRE 488 Quad by tracking a 3-ball triangle pattern                //
// placed on top of the quad                                                             //
//                                                                                       //
// Author: Phillip Jones (phjones@iastate.edu)                                           //
//                                                                                       //
// Set path to dynamically linked libraries: export LD_LIBRARY_PATH=/usr/local/lib       //
//                                                                                       //
// Compile command line:                                                                 //
// gcc -L/usr/local/lib -o GPS_server -lopencv_core -lopencv_imgproc -lopencv_highgui    //
// -lopencv_objdetect Fruit.cpp pseudoGPS.cpp main.cpp                                   // 
//                                                                                       //
//  Run command usage: GPS_server <UDP_SERVER_PORT>                                      //
//                                                                                       //
//  Run command example: GPS_server 4560                                                 //
//                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////


//Base object tracker: Written by  Kyle Hounslow 2013

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.


#include <sstream>
#include <string>
#include <iostream>

#include <vector>
#include <math.h>
#include "Fruit.h"
#include "pseudoGPS.h"
 
#include <stdlib.h> 
#include <errno.h>  
#include <sys/types.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h> 


using namespace cv;

// Made global so can remember last value
pseudoGPS quadGPS;    // Temp store quad tracking info to send to requeseter

//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;
//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame/
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 10*10;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";

// Constants for computing estimated Z, based on tracking system mesurments
const double CUBE_CONST = 0.0;
const double SQ_CONST = -0.0034;
const double LIN_CONST = 2.4135;
const double OFFSET_CONST = -354.05;  // with incorrect parimeter, -266.05

// Constant for computing x_offset and x_pix_per_cm: used to compute X and Y estimate 
//PHJ: need to update comment to cm_per_pix
const double X_SQ_OFF_CONST = 0.0;
const double X_LIN_OFF_CONST = 0.4062;
const double X_OFF_CONST = -0.5335;

const double X_PIX_SQ_CONST = 0.0;
const double X_PIX_LIN_CONST = -0.0013;
const double X_PIX_CONST = 0.3343;

const double Y_LIN_OFF_CONST = 0.2638;
const double Y_OFF_CONST = -0.129;

const double Y_PIX_SQ_CONST = 0.0;
const double Y_PIX_LIN_CONST = -0.0011;
const double Y_PIX_CONST = 0.338;

#define PI 3.14159265

void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed


}


// Used to sort objects by size of Area (phjones)
struct by_Area {
    bool operator()(Fruit const &a, Fruit const &b) const {
        return a.Area > b.Area;
    }
};

string intToString(int number){


	std::stringstream ss;
	ss << number;
	return ss.str();
}
void createTrackbars(){
	//create window for trackbars


	namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN", H_MIN);
	sprintf( TrackbarName, "H_MAX", H_MAX);
	sprintf( TrackbarName, "S_MIN", S_MIN);
	sprintf( TrackbarName, "S_MAX", S_MAX);
	sprintf( TrackbarName, "V_MIN", V_MIN);
	sprintf( TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->
	createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );


}
void drawObject(vector<Fruit> theFruits,Mat &frame){

  for(unsigned int i = 0; i< theFruits.size(); i++)
  {
	cv::circle(frame,cv::Point(theFruits.at(i).getxPos(),theFruits.at(i).getyPos()),10,cv::Scalar(0,0,255));
	cv::putText(frame,intToString(theFruits.at(i).getxPos())+ " , " + intToString(-(theFruits.at(i).getyPos()-480)),cv::Point(theFruits.at(i).getxPos(),theFruits.at(i).getyPos()+20),1,1,Scalar(0,255,0));
  }
}
void morphOps(Mat &thresh){

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);


	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);

}


// Determine Front, Left, and Right tracking ball of the quad
void computeFLR(Fruit *front_ball, Fruit *left_ball, Fruit *right_ball, Fruit temp_ball1, Fruit temp_ball2, Fruit temp_ball3)
{

  if(temp_ball1.getyPos() > temp_ball2.getyPos())
  {
    if(temp_ball1.getyPos() > temp_ball3.getyPos())
    {
      // Ball 1 is front
      front_ball->setxPos(temp_ball1.getxPos());
      front_ball->setyPos(temp_ball1.getyPos());

      // Determine left and right ball
      if(temp_ball2.getxPos() < temp_ball3.getxPos())
      {
        left_ball->setxPos(temp_ball2.getxPos()); //Ball 2 left
        left_ball->setyPos(temp_ball2.getyPos());

        right_ball->setxPos(temp_ball3.getxPos()); // Ball 3 right
        right_ball->setyPos(temp_ball3.getyPos());
      }
      else
      {
        left_ball->setxPos(temp_ball3.getxPos()); //Ball 3 left
        left_ball->setyPos(temp_ball3.getyPos());

        right_ball->setxPos(temp_ball2.getxPos()); // Ball 2 right
        right_ball->setyPos(temp_ball2.getyPos());
      }
    }
    else
    {
      // Ball 3 is front
      front_ball->setxPos(temp_ball3.getxPos());
      front_ball->setyPos(temp_ball3.getyPos());

      // Determine left and right ball
      if(temp_ball1.getxPos() < temp_ball2.getxPos())
      {
        left_ball->setxPos(temp_ball1.getxPos()); //Ball 1 left
        left_ball->setyPos(temp_ball1.getyPos());

        right_ball->setxPos(temp_ball2.getxPos()); // Ball 2 right
        right_ball->setyPos(temp_ball2.getyPos());
      }
      else
      {
        left_ball->setxPos(temp_ball2.getxPos()); //Ball 2 left
        left_ball->setyPos(temp_ball2.getyPos());

        right_ball->setxPos(temp_ball1.getxPos()); // Ball 1 right
        right_ball->setyPos(temp_ball1.getyPos());
      }
    }
  }
  else
  {
    if(temp_ball2.getyPos() > temp_ball3.getyPos())
    {
      // Ball 2 is front
      front_ball->setxPos(temp_ball2.getxPos());
      front_ball->setyPos(temp_ball2.getyPos());

      // Determine left and right ball
      if(temp_ball1.getxPos() < temp_ball3.getxPos())
      {
        left_ball->setxPos(temp_ball1.getxPos()); //Ball 1 left
        left_ball->setyPos(temp_ball1.getyPos());

        right_ball->setxPos(temp_ball3.getxPos()); // Ball 3 right
        right_ball->setyPos(temp_ball3.getyPos());
      }
      else
      {
        left_ball->setxPos(temp_ball3.getxPos()); //Ball 3 left
        left_ball->setyPos(temp_ball3.getyPos());

        right_ball->setxPos(temp_ball1.getxPos()); // Ball 1 right
        right_ball->setyPos(temp_ball1.getyPos());
      }
    }
    else
    {
      // Ball 3 is front
      front_ball->setxPos(temp_ball3.getxPos());
      front_ball->setyPos(temp_ball3.getyPos());

      // Determine left and right ball
      if(temp_ball1.getxPos() < temp_ball2.getxPos())
      {
        left_ball->setxPos(temp_ball1.getxPos()); //Ball 1 left
        left_ball->setyPos(temp_ball1.getyPos());

        right_ball->setxPos(temp_ball2.getxPos()); // Ball 2 right
        right_ball->setyPos(temp_ball2.getyPos());
      }
      else
      {
        left_ball->setxPos(temp_ball2.getxPos()); //Ball 2 left
        left_ball->setyPos(temp_ball2.getyPos());

        right_ball->setxPos(temp_ball1.getxPos()); // Ball 1 right
        right_ball->setyPos(temp_ball1.getyPos());
      }
    }
  }

}


// Estimate Z postion of quad (in cm)
// Input: perimeter of triangle (in pixels)
// Assuming: close to linear relation between y (i.e. est_z), and x (i.e. perimeter)
// y = mx + b  (b is 0 in this case since starting from 0 cm)
//              m is slope of cm vs pixel line
// Not linear!!  Used Excel to curve fit a polynomial
double est_z(double quad_perimeter)
{
  double est_z;

  // Constants from camera tracking setup measurements.
  // Used 4-5 measurment and Excel to fit 3rd degree polynomial
  est_z =  CUBE_CONST*pow(quad_perimeter,3.0) + SQ_CONST*pow(quad_perimeter, 2.0)
            + LIN_CONST*quad_perimeter + OFFSET_CONST;

  return est_z;
}


// Compute X, Y in cm, based on calibration measurments of tracking system
// Equation found using Excel curve fitting with 4-5 measured point
void est_x_y(pseudoGPS *quadGPS, double est_z, double quad_center_x, double quad_center_y)
{
double x_est;  // Estimate of actaul X in cm
double x_off;  // As a function of est_z, compute x distance from 0 for a x = 0 pixel
double x_pix_per_cm; // As a function of est_z, compute the pixels per cm in the x direction
double y_est;  // Estimate of actual Y in cm
double y_off;  // As a function of est_z, compute y distance from 0 for a y = 0 pixel
double y_pix_per_cm; // As a function of est_z, compute the pixels per cm in the y direction


  // Math to translate raw x,y pix values and estimate z to estimate actual x and y.
  x_off = X_SQ_OFF_CONST*pow(est_z,2.0) + X_LIN_OFF_CONST*est_z + X_OFF_CONST;
  x_pix_per_cm = X_PIX_SQ_CONST*pow(est_z,2.0) + X_PIX_LIN_CONST*est_z + X_PIX_CONST;
  x_est = x_off + quad_center_x*x_pix_per_cm;  // cm_per_pix (need to update varible name through out)
  quadGPS->setxPos(x_est);

  y_off = Y_LIN_OFF_CONST*est_z + Y_OFF_CONST;
  y_pix_per_cm = Y_PIX_SQ_CONST*pow(est_z,2.0) + Y_PIX_LIN_CONST*est_z + Y_PIX_CONST;
  y_est = y_off + quad_center_y*y_pix_per_cm;  // cm_per_pix (need to up date varilbe name)
  quadGPS->setyPos(y_est);
}



// computeQuadGPS
// transform X,Y,Z from pixels to cm
// compute direction in degrees
void computeQuadGPS(pseudoGPS *quadGPS, Fruit front_ball, Fruit left_ball, Fruit right_ball)
{
double dist_L_R;  // Distance between left and right ball (in pixels)
double dist_L_F;  // Distance between left and front ball  (in pixels)
double dist_R_F;  // Distance between right and front ball (in pixels)
double quad_perimeter; // Perimeter of quad (in pixels)
double quad_center_x;    // Center of three tracking balls (in pixels)
double quad_center_y;    // Center of three tracking balls (in pixels)
double quad_dir;

    // Compute triangle perimeter, this will be used to determine Z
    // Assuming "close" to linear scaling
    dist_L_R = sqrt(pow(right_ball.getyPos()-left_ball.getyPos(), 2.0) +
                    pow(right_ball.getxPos()-left_ball.getxPos(), 2.0));

    dist_L_F = sqrt(pow(front_ball.getyPos()-left_ball.getyPos(), 2.0) +
                    pow(front_ball.getxPos()-left_ball.getxPos(), 2.0));

    dist_R_F = sqrt(pow(front_ball.getyPos()-right_ball.getyPos(), 2.0) +
                    pow(front_ball.getxPos()-right_ball.getxPos(), 2.0));

    quad_perimeter = dist_L_R + dist_L_F + dist_R_F;

    // Computer direction (Note will go to infinite for 90 or -90 degrees)
    // atan2 returns result in radii, so convert to degrees
    quad_dir = atan2 ((right_ball.getyPos()-left_ball.getyPos()),right_ball.getxPos()-left_ball.getxPos())
               * 180 / PI;

    printf("P=%6.2f, H=%6.2f, D=%6.2f", quad_perimeter, est_z(quad_perimeter), quad_dir);

    // Formula for computing the center of a triangle
    // (center_x, center_y) = virtex_X + 2/3 (mid_oppX - virtex_X), virtex_Y + 2/3 (mid_oppY - virtex_Y)
    quad_center_x = front_ball.getxPos() +
                    (2.0/3.0)*(((right_ball.getxPos()+left_ball.getxPos())/2.0) - front_ball.getxPos());

    quad_center_y = front_ball.getyPos() +
                    (2.0/3.0)*(((right_ball.getyPos()+left_ball.getyPos())/2.0) - front_ball.getyPos());

    quadGPS->setzPos(est_z(quad_perimeter)); // Est Z based on camera calibration

    // Transform X/Y center in pixels to cm with use of estimated Z based on camera calibrations
    est_x_y(quadGPS, quadGPS->getzPos(),quad_center_x, quad_center_y);
    quadGPS->setdirPos(quad_dir);

    printf("P=%6.2f, Z=%6.2f, D=%6.2f, CX=%6.2f, CY=%6.2f, X=%6.2f, Y=%6.2f", quad_perimeter, est_z(quad_perimeter),
            quad_dir, quad_center_x, quad_center_y, quadGPS->getxPos(), quadGPS->getyPos());

}




void trackFilteredObject(Mat threshold,Mat HSV, Mat &cameraFeed){

    vector<Fruit> apples;  // Hold detected objects
    Fruit temp_ball1, temp_ball2, temp_ball3; // Temp store balls to track quad
    Fruit front_ball, left_ball, right_ball;  // Temp store balls to track quad
//    pseudoGPS quadGPS;    // Temp store quad tracking info to send to requeseter

	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if(area>MIN_OBJECT_AREA){

                    Fruit apple;

			        apple.setxPos(moment.m10/area);
					apple.setyPos(moment.m01/area);
					apple.setArea(area);

                     apples.push_back(apple);

					objectFound = true;

				}else objectFound = false;

			}

			// CPRE 488 (phjones): move 3 largest objects to the front of the vector
            std::sort(apples.begin(), apples.end(), by_Area());

			printf("Obsj=%llu ", apples.size());
			for (int index = 0; index < apples.size(); index++)
            {
              printf("S=%6.2f: ", apples[index].Area);
            }


            // Find and store Front, Left and Right object (Assume the 3 largest object are the tracking balls)
            // make sure we have detected at least 3 balls
            if(apples.size() > 2)
            {
              // First Flip y-coord by 180 degrees for more intutitive coord system
              temp_ball1.setxPos(apples[0].getxPos());
              temp_ball1.setyPos(-(apples[0].getyPos()- 480));
              temp_ball1.setArea(apples[0].getArea());

              temp_ball2.setxPos(apples[1].getxPos());
              temp_ball2.setyPos(-(apples[1].getyPos()- 480));
              temp_ball2.setArea(apples[1].getArea());

              temp_ball3.setxPos(apples[2].getxPos());
              temp_ball3.setyPos(-(apples[2].getyPos()- 480));
              temp_ball3.setArea(apples[2].getArea());

              computeFLR(&front_ball, &left_ball, &right_ball, temp_ball1, temp_ball2, temp_ball3);

			  // Compute X, Y, Z and direction  (i.e. pseudo GPS information) of quad
              computeQuadGPS(&quadGPS, front_ball, left_ball, right_ball);
              printf("\r");
            }

            // Send pseudoGPS information if a request has been made
            // If a ball gets lost, just send last computed pseudo GPS information
            // thus QuadGPS has been made global
            //sendGPS(QuadGPS);

			//Let Developer see what objects found
			if(objectFound ==true){
				//draw object location on screen
				drawObject(apples,cameraFeed);}

		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}

  // COM9
int cport_nr_GPS = 8;   // /dev/ttyS8 (COM9 on windows)
int bdrate   = 115200;  /* 115200 baud */

unsigned char send_GPS_buf[4096];
unsigned char get_GPS_buf[4096] = {0};
float GPS_float_buf[4] = {0};
char *send_GPS_char_ptr = (char *)GPS_float_buf;

// Server communication variables
#define ECHOMAX 255     /* Longest string to echo */
int sock;                        /* Socket */
struct sockaddr_in echoServAddr; /* Local address */
struct sockaddr_in echoClntAddr; /* Client address */
char echoBuffer[ECHOMAX];        /* Buffer for echo string */
unsigned short echoServPort;     /* Server port */
int cliLen;                      /* Length of incoming message */
int recvMsgSize;                 /* Size of received message */


// General variables
int n =0;
int my_count = 0;

int main(int argc, char* argv[])
{
	//if we would like to calibrate our filter values, set to true.
	bool calibrationMode = true;

	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	Mat threshold;
	Mat HSV;

	if(calibrationMode){
		//create slider bars for HSV filtering
		createTrackbars();
	}
	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop


    // Set up server port
    if (argc != 2)         /* Test for correct number of parameters */
    {
        fprintf(stderr,"Usage:  %s <UDP SERVER PORT>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  /* first arg:  Local port */

 
    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
     printf("Socket failed");
     exit(1);
    }

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
 

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
    {
     printf("Bind failed");
     exit(1);
    }


    // Track objects
	while(1){
		//store image to matrix
		capture.read(cameraFeed);
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed,HSV,CV_BGR2HSV);

		if(calibrationMode==true){
		//if in calibration mode, we track objects based on the HSV slider values.
		cvtColor(cameraFeed,HSV,CV_BGR2HSV);
		inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
		morphOps(threshold);
		imshow(windowName2,threshold);
		trackFilteredObject(threshold,HSV,cameraFeed);
		}
		else
        {
          Fruit apple, bannana, cherry;

          // Set thresholds
          apple.setHSVmin(Scalar(40,61,63));
          apple.setHSVmax(Scalar(255,255,255));

		  cvtColor(cameraFeed,HSV,CV_BGR2HSV);
		  inRange(HSV,apple.getHSVmin(),apple.getHSVmax(),threshold);
		  morphOps(threshold);
		  trackFilteredObject(threshold,HSV,cameraFeed);
		}

		//show frames
		//imshow(windowName2,threshold);

		imshow(windowName,cameraFeed);
		//imshow(windowName1,HSV);


        //Get GPS request
        /* Set the size of the in-out parameter */
        cliLen = sizeof(echoClntAddr);

        /* Block until receive message from a client */
        if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0,
            (struct sockaddr *) &echoClntAddr, (socklen_t*) &cliLen)) < 0)
        {
           printf("Received from failed");
           exit(1);
        }

        printf("Handling client %d %s: %c\n", my_count, inet_ntoa(echoClntAddr.sin_addr), echoBuffer[0]);
        my_count++;

        /* Send GPS info back to the client */
        GPS_float_buf[0] = quadGPS.getxPos();
        GPS_float_buf[1] = quadGPS.getyPos();
        GPS_float_buf[2] = quadGPS.getzPos();
        GPS_float_buf[3] = quadGPS.getdirPos();


        if (sendto(sock, send_GPS_char_ptr, 16, 0, (struct sockaddr *) &echoClntAddr,
                    sizeof(echoClntAddr)) != 16)
        {
          printf("Sent inccorrect number of bytes");
          exit(1);
        }


		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(10);
	}

	return 0;
}

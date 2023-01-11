#include <opencv2/aruco.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_250);
    Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();


    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Nie można otworzyć kamery." << endl;
        return 1;
    }

    while (true) {

        Mat frame;
        cap >> frame;
        if (frame.empty()) {
            cout << "Nie można załadować klatki z kamery." << endl;
            break;
        }

        vector<vector<Point2f> > markerCorners, rejectedCandidates;
        vector<int> markerIds;
        aruco::detectMarkers(frame, dictionary, markerCorners, markerIds, detectorParams, rejectedCandidates);

        if (markerIds.size() >= 2) {
            Point2f firstMarkerCenter, secondMarkerCenter;

            // Obliczenie środka pierwszego i drugiego markeru
            for (int i = 0; i < markerCorners.size(); i++) {
                if (i == 0) {
                    firstMarkerCenter = (markerCorners[i][0] + markerCorners[i][2]) / 2;
                } else {
                    secondMarkerCenter = (markerCorners[i][0] + markerCorners[i][2]) / 2;
                }
            }


            double distance = norm(firstMarkerCenter - secondMarkerCenter);

            putText(frame, format("Distance: %.2f", distance), Point(10, 50), FONT_HERSHEY_SIMPLEX, 1,
                    Scalar(0, 0, 255), 2);
        }


        aruco::drawDetectedMarkers(frame, markerCorners, markerIds);
        imshow("frame", frame);

        if (waitKey(1) == 27) break; // if 'esc' pressed

    }
}
#include <opencv2/aruco.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

const float MARKER_MM = 70;

vector<Point2f> fromPixelToMiliMeters(vector<Point2f> original) {
    vector<Point2f> result;
    double distance = norm(original[0] - original[2]);
    double desiredDistance = MARKER_MM;
    double scale = desiredDistance / distance;

    for (int i = 0; i < 4; i++) {
        result.emplace_back(original[i] * scale);
    }
    return result;
}

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
            vector<Point2f> firstMarkerCorners = fromPixelToMiliMeters(markerCorners[0]);
            vector<Point2f> secondMarkerCorners = fromPixelToMiliMeters(markerCorners[1]);
            double distance = norm(firstMarkerCorners[0] - secondMarkerCorners[0]);


            putText(frame, format("Distance: %.2f", distance), Point(10, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
        }

        aruco::drawDetectedMarkers(frame, markerCorners, markerIds);
        imshow("frame", frame);

        if (waitKey(1) == 27) break;

    }
    return 0;
}
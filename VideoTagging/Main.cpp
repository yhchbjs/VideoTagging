#include <cstdio>
#include <cstdlib>
#include <string>
#include "opencv2\opencv.hpp"
using namespace std;
using namespace cv;

int frameN, segmentN;
vector<Mat> frames;
vector<int> sframe;
vector<int> eframe;
vector<int> labels;
int main()
{
	string videoFile = "./video/video.mp4";
	string segmentFile = "./segment/segment.txt";
	string labelFile = "./label/label.txt";

	printf("load segment start\n");
	FILE *segmentPtr = fopen(segmentFile.c_str(), "rt");
	fscanf(segmentPtr, "%d", &segmentN);
	sframe.resize(segmentN);
	eframe.resize(segmentN);
	for (int i = 0; i < segmentN; i++)
	{
		fscanf(segmentPtr, "%d %d", &sframe[i], &eframe[i]);
	}
	fclose(segmentPtr);
	printf("load segment end\n");

	printf("load video start\n");
	VideoCapture videoPtr;
	videoPtr.open(videoFile);
	double fps = videoPtr.get(CAP_PROP_FPS);
	int delay = (int)(1000 / fps);
	while (true)
	{
		Mat frame;
		if (!videoPtr.read(frame))
		{
			break;
		}
		frames.push_back(frame);
	}
	frameN = (int)frames.size();
	videoPtr.release();
	printf("load video end\n");

	printf("show info start\n");
	printf("videoName = %s\n", videoFile.c_str());
	printf("segmentN  = %d\n", segmentN);
	printf("frameN    = %d\n", frameN);
	printf("show info end\n");

	printf("label segment start\n");
	namedWindow(videoFile);
	labels.resize(segmentN);
	for (int i = 0; i < segmentN; i++)
	{
		printf("seg %d, frame %d-%d", i, sframe[i], eframe[i]);
		int index = sframe[i];
		while (true)
		{
			imshow(videoFile, frames[index]);
			int key = waitKey(delay);
			if (key != -1)
			{
				if (key == '-')
				{
					labels[i] = -1;
					printf(", label -1\n");
					break;
				}
				if (key == '=')
				{
					labels[i] = 1;
					printf(", label +1\n");
					break;
				}
			}
			index++;
			if (index == eframe[i])
			{
				index = sframe[i];
			}
		}
	}
	destroyWindow(videoFile);
	printf("label segment end\n");

	printf("save label start\n");
	FILE *labelPtr = fopen(labelFile.c_str(), "wt");
	fprintf(labelPtr, "%d\n", segmentN);
	for (int i = 0; i < segmentN; i++)
	{
		fprintf(labelPtr, "%d %d %d\n", sframe[i], eframe[i], labels[i]);
	}
	fclose(labelPtr);
	printf("save label end\n");
}
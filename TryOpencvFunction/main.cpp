#include<opencv2/opencv.hpp>
#include<vector>
#include<stack>
#include<queue>
#include<algorithm>

bool pointChanged = false;
cv::Point p;
std::string path = "C:/New folder/amg.jpg";
//cv::Mat img = cv::imread(path);


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		//std::cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
		p.x = x;
		p.y = y;
		pointChanged = true;
	}
	else if (event == cv::EVENT_RBUTTONDOWN)
	{
		//cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		p.x = x;
		p.y = y;
		pointChanged = true;

	}

}

bool checkNeighb(const cv::Point main, cv::Mat& used, const cv::Mat& img, const cv::Point& next)
{
	cv::Rect r(0, 0, img.cols, img.rows);
	double distance = std::sqrt(pow(img.at<cv::Vec3b>(main)[0] - img.at<cv::Vec3b>(next)[0], 2) + pow(img.at<cv::Vec3b>(main)[1] - img.at<cv::Vec3b>(next)[1], 2) + pow(img.at<cv::Vec3b>(main)[2] - img.at<cv::Vec3b>(next)[2], 2));
	auto res = !used.at<bool>(next) && r.contains(next) && distance <= 10;
	if(r.contains(next))
		used.at<bool>(next) = true;
	return  res;
}

void dfs(const cv::Mat& img, const cv::Point& startCord)
{
	auto vis = img.clone();
	std::queue<cv::Point> qu;
	qu.push(startCord);
	cv::Mat_<bool> used(img.rows, img.cols);
	used.setTo(false);
	int kk = 0;
	while (!qu.empty())
	{
		kk++;
		auto& p = qu.front();

		vis.at<cv::Vec3b>(p) = cv::Vec3b(255, 255, 255);
		if (kk % 5 == 0)
		{
			cv::imshow("img", vis);
			cv::waitKey(1);
		}
		used.at<bool>(p) = true;
		qu.pop();

		if (checkNeighb(p, used, img, p + cv::Point(0, 1)))
		{
			qu.push(p + cv::Point(0, 1));
		}
		if (checkNeighb(p, used, img, p + cv::Point(0, -1)))
		{
			qu.push(p + cv::Point(0, -1));
		}
		if (checkNeighb(p, used, img, p + cv::Point(1, 0)))
		{
			qu.push(p + cv::Point(1, 0));
		}
		if (checkNeighb(p, used, img, p + cv::Point(-1, 0)))
		{
			qu.push(p + cv::Point(-1, 0));
		}
	}
}

int main()
{

	cv::namedWindow("My Window", 1);
	cv::setMouseCallback("My Window", CallBackFunc, NULL);
	cv::Mat img = cv::imread(path);
	imshow("My Window", img);
	while (true)
	{
		if (pointChanged)
		{
			std::cout << p << std::endl;
			pointChanged = false;
			dfs(img, p);
			
		}
		cv::waitKey(1);
	}
	return 0;

}
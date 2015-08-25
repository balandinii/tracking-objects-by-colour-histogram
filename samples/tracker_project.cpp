#include <tracker.hpp>
#include <vector>
#include <opencv2/opencv.hpp>

class Trackerproject : public Tracker
{
 public:
    virtual ~Trackerproject() {}

    virtual bool init( const cv::Mat& frame, const cv::Rect& initial_position );
    virtual bool track( const cv::Mat& frame, cv::Rect& new_position );

 private:
    cv::Rect position_;
};

cv::Mat previous_frame;

bool Trackerproject::init( const cv::Mat& frame, const cv::Rect& initial_position )
{
    position_ = initial_position;
	previous_frame=frame;
	return true;
}

bool Trackerproject::track( const cv::Mat& frame, cv::Rect& new_position )
{
	cv::Mat for_hist(previous_frame,position_);
	int histSize[3] = {8, 8, 8};
    float range[2] = {0, 256};
    const float * ranges[3] = {range, range, range};
    int channels[3] = {0, 1, 2};
    cv::Mat old_hist;
    calcHist(&for_hist, 1, channels, cv::Mat(), old_hist, 3, histSize, ranges);

	std::vector<long> possible_dy;
	std::vector<long> possible_dx;
	std::vector<double> ver;

	possible_dx.push_back(0);possible_dy.push_back(10);
	possible_dx.push_back(10);possible_dy.push_back(0);
	possible_dx.push_back(0);possible_dy.push_back(-10);
	possible_dx.push_back(-10);possible_dy.push_back(0);
	
	for(long i=0;i<possible_dx.size();i++) ver.push_back(0);

	for(long i=0;i<possible_dx.size();i++){
										 cv::Rect position1;position1=position_;
										 cv::Mat hist1;
										 position1.x+=possible_dx[i];
										 position1.y+=possible_dy[i];
										 cv::Mat for_hist(previous_frame,position1);
										 int histSize[3] = {8, 8, 8};
										 float range[2] = {0, 256};
										 const float * ranges[3] = {range, range, range};
										 int channels[3] = {0, 1, 2};
										 calcHist(&for_hist, 1, channels, cv::Mat(), hist1, 3, histSize, ranges);
										 double ver1=compareHist(old_hist,hist1,CV_COMP_BHATTACHARYYA);
										 ver[i]=ver1;
										 }

	double norm;
	norm=0;
	for(long i=0;i<ver.size();i++){norm+=ver[i];}
	for(long i=0;i<ver.size();i++){ver[i]/=norm;}

	double dx;double dy;dx=0;dy=0;

	for(long i=0;i<ver.size();i++){
								dx+=(possible_dx[i]*ver[i]);
								dy+=(possible_dy[i]*ver[i]);
								}

	position_.x+=dx;
	position_.y+=dy;

    new_position = position_;
	return true;
}

cv::Ptr<Tracker> createTrackerproject()
{
    return cv::Ptr<Tracker>(new Trackerproject());
}

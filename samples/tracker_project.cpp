#include <tracker.hpp>

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
	//cv::Mat mask(frame.rows,frame.cols,CV_8UC1);
	//for(int i=0;i<frame.rows;i++)for(int j=0;j<frame.cols;j++) mask.ptr(i,j)=0;
	cv::Mat for_hist(previous_frame,position_);
	 int histSize[3] = {8, 8, 8};
    float range[2] = {0, 256};
    const float * ranges[3] = {range, range, range};
    int channels[3] = {0, 1, 2};
    cv::Mat hist;
    calcHist(&for_hist, 1, channels, cv::Mat(), hist, 3, histSize, ranges);

	cv::Rect position1;cv::Mat hist1;
	cv::Rect position2;cv::Mat hist2;
	cv::Rect position3;cv::Mat hist3;
	cv::Rect position4;cv::Mat hist4;
	position1=position_;position1.x+=10;
		position2=position_;position2.x-=10;
			position3=position_;position3.y+=10;
				position4=position_;position4.y-=10;
	{
			cv::Mat for_hist(previous_frame,position1);
	 int histSize[3] = {8, 8, 8};
    float range[2] = {0, 256};
    const float * ranges[3] = {range, range, range};
    int channels[3] = {0, 1, 2};
    calcHist(&for_hist, 1, channels, cv::Mat(), hist1, 3, histSize, ranges);
	}
		{
			cv::Mat for_hist(previous_frame,position2);
	 int histSize[3] = {8, 8, 8};
    float range[2] = {0, 256};
    const float * ranges[3] = {range, range, range};
    int channels[3] = {0, 1, 2};
    calcHist(&for_hist, 1, channels, cv::Mat(), hist2, 3, histSize, ranges);
	}
			{
			cv::Mat for_hist(previous_frame,position3);
	 int histSize[3] = {8, 8, 8};
    float range[2] = {0, 256};
    const float * ranges[3] = {range, range, range};
    int channels[3] = {0, 1, 2};
    calcHist(&for_hist, 1, channels, cv::Mat(), hist3, 3, histSize, ranges);
	}
				{
			cv::Mat for_hist(previous_frame,position4);
	 int histSize[3] = {8, 8, 8};
    float range[2] = {0, 256};
    const float * ranges[3] = {range, range, range};
    int channels[3] = {0, 1, 2};
    calcHist(&for_hist, 1, channels, cv::Mat(), hist4, 3, histSize, ranges);
	}
	double dx;double dy;
	double ver1;double ver2;double ver3;double ver4;
	ver1=compareHist(hist,hist1,CV_COMP_BHATTACHARYYA);
		ver2=compareHist(hist,hist2,CV_COMP_BHATTACHARYYA);
			ver3=compareHist(hist,hist3,CV_COMP_BHATTACHARYYA);
				ver4=compareHist(hist,hist4,CV_COMP_BHATTACHARYYA);

	double norm;norm=ver1+ver2+ver3+ver4;
	ver1/=norm;
		ver2/=norm;	ver3/=norm;	ver4/=norm;

	dx=10*ver1-10*ver2;
	dy=10*ver3-10*ver4;

	position_.x+=dx;
	position_.y+=dy;

    new_position = position_;
	return true;
}

cv::Ptr<Tracker> createTrackerproject()
{
    return cv::Ptr<Tracker>(new Trackerproject());
}

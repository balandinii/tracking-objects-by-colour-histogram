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
	double xxx;
	double yyy;
	//xxx и yyy - текущие положения верхнего левого угла трекингового окна
};

cv::Mat previous_frame;

bool Trackerproject::init( const cv::Mat& frame, const cv::Rect& initial_position )
{
    position_ = initial_position;
	previous_frame=frame;
	xxx=position_.x;
	yyy=position_.y;
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

	for(long i=-30;i<40;i+=10)for(long j=-30;j<40;j+=10){
														if((i==0)&&(j==0)) continue;
														possible_dx.push_back(i);possible_dy.push_back(j);
														}

//	std::cout<<possible_dx.size()<<"  "<<possible_dy.size()<<"  ";
	
	for(long i=0;i<possible_dx.size();i++) ver.push_back(0);

	//std::cout<<ver.size()<<"\n";

//	for(long i=0;i<possible_dx.size();i++){
		//std::cout<<possible_dx[i]<<"  "<<possible_dy[i]<<"\n";}

	for(long i=0;i<possible_dx.size();i++){
										 cv::Rect position1;position1=position_;
										 cv::Mat hist1;
										 position1.x+=possible_dx[i];
										 position1.y+=possible_dy[i];
										 {
										 //отслеживаем случай, когда прямоугольник position1 выходит за границы кадра
										 cv::Point p1(1,1);
										 cv::Point p2((frame.cols-2),(frame.rows-2));
										 cv::Rect tmp(p1,p2);
										 bool vyx=false;
										 if(position1.x<tmp.x) vyx=true;
										 if(position1.y<tmp.y) vyx=true;
										 if((position1.x+position1.width)>(tmp.x+tmp.width)) vyx=true;
										 if((position1.y+position1.height)>(tmp.y+tmp.height)) vyx=true;
										 if(vyx){ver[i]=-5.0;continue;}
										 }
										 cv::Mat for_hist(frame,position1);
										 int histSize[3] = {8, 8, 8};
										 float range[2] = {0, 256};
										 const float * ranges[3] = {range, range, range};
										 int channels[3] = {0, 1, 2};
										 calcHist(&for_hist, 1, channels, cv::Mat(), hist1, 3, histSize, ranges);
										 double ver1=compareHist(old_hist,hist1,CV_COMP_CORREL);
										 ver[i]=ver1;
										 }

	{
	double min=1.0e40;
	for(int i=0;i<ver.size();i++){if((ver[i]>0)&&(ver[i]<min)){min=ver[i];}}
	for(int i=0;i<ver.size();i++) if (ver[i]<0) ver[i]=min;
	}

	double norm;
	norm=0;
	for(long i=0;i<ver.size();i++){norm+=ver[i];}
	for(long i=0;i<ver.size();i++){ver[i]/=norm;}

	double dx;double dy;dx=0;dy=0;

		//for(long i=0;i<possible_dx.size();i++){
		//std::cout<<possible_dx[i]<<"  "<<possible_dy[i]<<"\n";}

	for(long i=0;i<ver.size();i++){
								dx+=(possible_dx[i]*ver[i]);
								dy+=(possible_dy[i]*ver[i]);
								//std::cout<<ver[i]<<"\n";
								}

	//double dl=sqrt(dx*dx+dy*dy);
	//if(dl>0.001){dx/=dl;dy/=dl;}
	dx*=17.0;dy*=17.0;

	xxx+=dx;
	yyy+=dy;
	position_.x=xxx;
	position_.y=yyy;
	//std::cout<<xxx<<"  "<<yyy<<"\n";

    new_position = position_;
	previous_frame=frame;
	return true;
}

cv::Ptr<Tracker> createTrackerproject()
{
    return cv::Ptr<Tracker>(new Trackerproject());
}

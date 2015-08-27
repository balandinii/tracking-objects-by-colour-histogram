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

cv::Mat old_hist;

bool Trackerproject::init( const cv::Mat& frame, const cv::Rect& initial_position )
{
    position_ = initial_position;
	previous_frame=frame;
	xxx=position_.x;
	yyy=position_.y;

	cv::Mat for_hist(previous_frame,position_);
	int histSize[3] = {32, 32, 32};
    float range[2] = {0, 256};
    const float * ranges[3] = {range, range, range};
    int channels[3] = {0, 1, 2};
    calcHist(&for_hist, 1, channels, cv::Mat(), old_hist, 3, histSize, ranges);

	return true;
}

bool Trackerproject::track( const cv::Mat& frame, cv::Rect& new_position )
{
	
	if (old_hist.empty())
	{
		std::cout << "Empty histogram!" << std:: endl;
		return false;
	}

	std::vector<long> possible_dy;
	std::vector<long> possible_dx;
	std::vector<double> ver;

	for(long i=-30;i<40;i+=10)for(long j=-30;j<40;j+=10){
														if((i==0)&&(j==0)) continue;
														possible_dx.push_back(i);possible_dy.push_back(j);
														}
	ver.resize(possible_dx.size());

	for(long i=0;i<possible_dx.size();i++){
										  cv::Rect position1(position_);
										  cv::Mat hist1;
										  position1.x+=possible_dx[i];
										  position1.y+=possible_dy[i];
										  {
										  //отслеживаем случай, когда прямоугольник position1 выходит за границы кадра
										  cv::Point p1(0,0);
										  cv::Point p2((frame.cols-1),(frame.rows-1));
										  cv::Rect tmp(p1,p2);
										  bool vyx=false;
										  if( (position1.x<tmp.x) || (position1.y<tmp.y) ) vyx=true;
										  if((position1.x+position1.width)>(tmp.x+tmp.width)) vyx=true;
										  if((position1.y+position1.height)>(tmp.y+tmp.height)) vyx=true;
										  if(vyx){ver[i]=-5.0;continue;}
										  }
										  cv::Mat for_hist(frame,position1);		
										  int histSize[3] = {32, 32, 32};
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

	double norm=0;
	for(long i=0;i<ver.size();i++){norm+=ver[i];}
	for(long i=0;i<ver.size();i++){ver[i]/=norm;}

	double dx=0;
	double dy=0;

	for(long i=0;i<ver.size();i++){
								  dx+=(possible_dx[i]*ver[i]);
								  dy+=(possible_dy[i]*ver[i]);
								  }

	double dl=sqrt(dx*dx+dy*dy);
	if(dl>0.000001){
				   dx/=dl;dy/=dl;
					double maxver=0;
					long tt=1;
					for(long t=1;t<100;t++){
							             cv::Rect position1;position1=position_;
										 cv::Mat hist1;
										 position1.x+=(dx*double(t));
										 position1.y+=(dy*double(t));
										 {
										 //отслеживаем случай, когда прямоугольник position1 выходит за границы кадра
										 cv::Point p1(0,0);
										 cv::Point p2((frame.cols-1),(frame.rows-1));
										 cv::Rect tmp(p1,p2);
										 bool vyx=false;
										 if( (position1.x<tmp.x) || (position1.y<tmp.y) ) vyx=true;
										 if((position1.x+position1.width)>(tmp.x+tmp.width)) vyx=true;
										 if((position1.y+position1.height)>(tmp.y+tmp.height)) vyx=true;
										 if(vyx){continue;}
										 }
										 cv::Mat for_hist(frame,position1);
										 int histSize[3] = {32, 32, 32};
										 float range[2] = {0, 256};
										 const float * ranges[3] = {range, range, range};
										 int channels[3] = {0, 1, 2};
										 calcHist(&for_hist, 1, channels, cv::Mat(), hist1, 3, histSize, ranges);
										 double ver1=compareHist(old_hist,hist1,CV_COMP_CORREL);
										 if((ver1)>(maxver)){maxver=ver1;tt=t;}
										 else{break;}
					        }
					dx*=tt;dy*=tt;
					}

	xxx+=dx;
	yyy+=dy;
	position_.x=xxx;
	position_.y=yyy;

    new_position = position_;
	previous_frame=frame;
	return true;
}

cv::Ptr<Tracker> createTrackerproject()
{
    return cv::Ptr<Tracker>(new Trackerproject());
}

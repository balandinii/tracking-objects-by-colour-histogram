#include <tracker.hpp>

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
	previous_frame = frame;
	return true;
}



























long*** make_hist(cv::Mat mat, cv::Rect& h){
											long*** ans=new long**[8];
											for(long i=0;i<8;i++){
																 ans[i]=new long*[8];
																 for(long j=0;j<8;j++) ans[i][j]=new long[8];
																 }
											for(long i=0;i<8;i++)for(long j=0;j<8;j++)for(long k=0;k<8;k++) ans[i][j][k]=0;
											for(long i=h.x;i<(h.x+h.width);i++)for(long j=h.y;j<(h.y+h.height);j++){
																													auto k=mat.ptr(i,j);
																													ans[k[0]/32][k[1]/32][k[2]/32]++;
																													}
											return ans;
											}

struct three{
	long x;
	long y;
	double dist;
	three(){}
	three(long h,long j){x=h;y=j;}
};

#include "vector"

bool Trackerproject::track( const cv::Mat& frame, cv::Rect& new_position )
{
    long*** old_hist=make_hist(previous_frame,position_);

	std::vector<three> hipoteses;//гипотезы
for(int i=-4;i<5;i++)for(int j=-4;j<5;j++){
										  if((i==0)&&(j==0)) continue;
										  hipoteses.push_back(three(i,j));
}
	for(long hjk=0;hjk<hipoteses.size();hjk++){
											  cv::Rect mon=position_;
											  mon.x+=hipoteses[hjk].x;
											  mon.y+=hipoteses[hjk].y;
											  long*** new_hist=make_hist(frame,mon);
											  double sum1=0;for(long i=0;i<8;i++)for(long j=0;j<8;j++)for(long k=0;k<8;k++){sum1+=old_hist[i][j][k];}
											  double sum2=0;for(long i=0;i<8;i++)for(long j=0;j<8;j++)for(long k=0;k<8;k++){sum2+=new_hist[i][j][k];}
											  double sum3=0;for(long i=0;i<8;i++)for(long j=0;j<8;j++)for(long k=0;k<8;k++){sum3+=sqrt(double(new_hist[i][j][k]*old_hist[i][j][k]));}
											  hipoteses[hjk].dist=sqrt(1.0-1/sqrt(sum1*sum2)*sum3);
											  }
	for(long hjk=0;hjk<hipoteses.size();hjk++){hipoteses[hjk].dist=1-(hipoteses[hjk].dist);}
	double norm=0;	for(long hjk=0;hjk<hipoteses.size();hjk++){norm+=hipoteses[hjk].dist;}
		for(long hjk=0;hjk<hipoteses.size();hjk++){hipoteses[hjk].dist/=norm;}
		double dx=0;
		double dy=0;
		for(long hjk=0;hjk<hipoteses.size();hjk++){dx+=hipoteses[hjk].dist*hipoteses[hjk].x;}
		for(long hjk=0;hjk<hipoteses.size();hjk++){dy+=hipoteses[hjk].dist*hipoteses[hjk].y;}

		long ddx;long ddy;
		double maxd=0;for(long hjk=0;hjk<hipoteses.size();hjk++){if(maxd<hipoteses[hjk].dist){maxd=hipoteses[hjk].dist;ddx=hipoteses[hjk].x;ddy=hipoteses[hjk].y;}}

    new_position=position_;
	new_position.x+=ddx;
	new_position.y+=ddy;
	previous_frame=frame;
	return true;
}

cv::Ptr<Tracker> createTrackerproject()
{
    return cv::Ptr<Tracker>(new Trackerproject());
}

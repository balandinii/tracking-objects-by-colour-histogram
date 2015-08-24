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
long* old_hist;

#include "iostream"

long* make_hist(cv::Mat mat, cv::Rect& h){std::cout<<"start";
											//long*** ans=new long**[8];
											//for(long i=0;i<8;i++){
											//					 ans[i]=new long*[8];
											//					 for(long j=0;j<8;j++) ans[i][j]=new long[8];
											//					 }
											//for(long i=0;i<8;i++)for(long j=0;j<8;j++)for(long k=0;k<8;k++) ans[i][j][k]=0;
											//for(long i=h.x;i<(h.x+h.width);i++)for(long j=h.y;j<(h.y+h.height);j++){
											//																		auto k=mat.ptr(i,j);
											//																		ans[int(k[0])/32][int(k[1])/32][int(k[2])/32]++;
											//																	//	std::cout<<k[0]<<" "<<k[1]<<" "<<k[2]<<std::endl;
											//																		}
											long one=0;long two=0;long thr=0;
											long* ans=new long[3];
											std::cout<<"start";
											for(long i=h.x;i<(h.x+h.width);i++)for(long j=h.y;j<(h.y+h.height);j++){
																													//std::cout<<"start";
																													auto k=mat.ptr(i,j);
																													one+=long(k[0]);two+=long(k[1]);thr+=long(k[2]);
																													}
											ans[0]=one;ans[1]=two;ans[2]=thr;
											std::cout<<one<<" "<<two<<" "<<thr;
											return ans;
											}

bool Trackerproject::init( const cv::Mat& frame, const cv::Rect& initial_position )
{std::cout<<"init";
    position_ = initial_position;
	previous_frame = frame;
	old_hist=make_hist(previous_frame,position_);
	return true;
}





























struct three{
	long x;
	long y;
	double dist;
	three(){}
	three(long h,long j){x=h;y=j;}
};

#include "vector"

static long she=0;

bool Trackerproject::track( const cv::Mat& frame, cv::Rect& new_position )
{std::cout<<"track";she++;
    //long*** old_hist=make_hist(previous_frame,position_);
//	if(she==20) old_hist=make_hist(previous_frame,position_);

	std::vector<three> hipoteses;//гипотезы
for(int i=-4;i<5;i++)for(int j=-4;j<5;j++){
										  if((i==0)&&(j==0)) continue;
										  hipoteses.push_back(three(i,j));
}
	for(long hjk=0;hjk<hipoteses.size();hjk++){
											  cv::Rect mon=position_;
											  mon.x+=hipoteses[hjk].x;
											  mon.y+=hipoteses[hjk].y;
											  long* new_hist=make_hist(frame,mon);
	/*										  double sum1=0;for(long i=0;i<8;i++)for(long j=0;j<8;j++)for(long k=0;k<8;k++){sum1+=old_hist[i][j][k];}
											  double sum2=0;for(long i=0;i<8;i++)for(long j=0;j<8;j++)for(long k=0;k<8;k++){sum2+=new_hist[i][j][k];}
											  double sum3=0;for(long i=0;i<8;i++)for(long j=0;j<8;j++)for(long k=0;k<8;k++){sum3+=sqrt(double(new_hist[i][j][k]*old_hist[i][j][k]));}
											  hipoteses[hjk].dist=sqrt(1.0-1/sqrt(sum1*sum2)*sum3);*/
											  hipoteses[hjk].dist=0;
											  hipoteses[hjk].dist+=(new_hist[0]-old_hist[0])*(new_hist[0]-old_hist[0]);
											  hipoteses[hjk].dist+=(new_hist[1]-old_hist[1])*(new_hist[1]-old_hist[1]);
											  hipoteses[hjk].dist+=(new_hist[2]-old_hist[2])*(new_hist[2]-old_hist[2]);
											  }
	/*for(long hjk=0;hjk<hipoteses.size();hjk++){hipoteses[hjk].dist=1-(hipoteses[hjk].dist);}
	double norm=0;	for(long hjk=0;hjk<hipoteses.size();hjk++){norm+=hipoteses[hjk].dist;}
		for(long hjk=0;hjk<hipoteses.size();hjk++){hipoteses[hjk].dist/=norm;}
		double dx=0;
		double dy=0;
		for(long hjk=0;hjk<hipoteses.size();hjk++){dx+=hipoteses[hjk].dist*hipoteses[hjk].x;}
		for(long hjk=0;hjk<hipoteses.size();hjk++){dy+=hipoteses[hjk].dist*hipoteses[hjk].y;}*/

		long ddx;long ddy;
		double maxd=1000000000;for(long hjk=0;hjk<hipoteses.size();hjk++){if(maxd>hipoteses[hjk].dist){maxd=hipoteses[hjk].dist;ddx=hipoteses[hjk].x;ddy=hipoteses[hjk].y;}}

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

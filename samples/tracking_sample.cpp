#include "tracker.hpp"
#include "gui.hpp"
#include "benchmark.hpp"

#include <iostream>
#include <fstream>

void help(const char *argv0)
{
    std::cout <<
        "Usage:\n"
        << argv0 <<
        " <tracker_algorithm> <video_name> <bounding_box or path_to_gt_file>\n"
        "\nVideo examples can be found in \"dataset\" folder\n"
        "\nBounding box should be given in format \"x1,y1,x2,y2\",\n"
        "where x's and y's are integer cordinates of opposed corners of bounding box\n"
        "\nGround truth files are text files where each line is the representation\n"
        "of a bounding box in the described format. Examples can also be found \n"
        "in the\"dataset\" folder.\n\n"
        "Examples:\n\n"
        "$ ./bin/tracking_sample dummy ../dataset/car.mp4\n\n"
        "$ ./bin/tracking_sample dummy ../dataset/car.mp4 142,125,232,164\n\n"
        "$ ./bin/tracking_sample dummy ../dataset/car.mp4 ../dataset/car.txt\n\n"
              << std::endl;
}

static const char* keys =
{
    "{1 | |                 | Tracker algorithm }"
    "{2 | |                 | video name        }"
    "{3 | | 0,0,0,0         | Initial bounding frame    }"
};

int main( int argc, const char** argv )
{
    cv::CommandLineParser parser( argc, argv, keys );

    // Parse and validate input parameters
    std::string tracker_algorithm = parser.get<std::string>("1");
    std::string video_name = parser.get<std::string>("2");

    std::cout << "Tracker algorithm: " << tracker_algorithm << std::endl;
    std::cout << "Video name: " << video_name << std::endl;

    if( tracker_algorithm.empty() || video_name.empty() )
    {
        std::cout << "Error: can't recognize tracking algorithm or open video" << std::endl;
        help(argv[0]);
        return 1;
    }

    // Open the video file
    cv::VideoCapture cap;
    if(video_name=="camera"){cap.open(0);}
    else{cap.open( video_name );}

    if( !cap.isOpened() )
    {
        std::cout << "Error: could not initialize video capturing...\n";
        help(argv[0]);
        return 1;
    }

    // Get the first frame
    cv::Mat frame;
    cap >> frame;cap >> frame;cap >> frame;cap >> frame;cap >> frame;
    
    // Initialize GTReader and PrecisionRecallEvaluator
    std::string argv3 = parser.get<std::string>("3");
    GTReader gt_reader(argv3);
    PrecisionRecallEvaluator pr_evaluator;

    // Initialize GUI
    GUI gui;
    cv::Rect bounding_box = gui.initBoundingBox(gt_reader.get(), frame);

    // Create Tracker
    cv::Ptr<Tracker> tracker = createTracker(tracker_algorithm);

    if (tracker == NULL)
    {
        std::cout << "Error: can't create tracker \"" << tracker_algorithm << "\"" << std::endl;
        return 1;
    }

    if (!tracker->init(frame, bounding_box))
    {
        std::cout << "Error: can't initialize tracker..." << std::endl;
        return 1;
    }


    long sh=0;
    
    cv::VideoWriter captur;
    captur.open("kol.avi",8,1.0/24.0,static_cast<cv::Size>(frame.size()));
    if(!captur.isOpened()) std::cout<<"not opened";

    // Run tracking
    while (true)
    {
        // Fetch next frame
        cap >> frame;
        if(frame.empty())
            break;

        // Track object
        cv::Rect position;
        bool found = tracker->track(frame, position);

        // Compare the predicted position with ground truth, if known
        cv::Rect gt = gt_reader.next();
        cv::Scalar rect_color = cv::Scalar(0, 255, 0);
        if (gt_reader.isOpen() && !pr_evaluator.updateMetrics(position, gt))
        {
            // Make rect red, if the prediction is incorrect
            rect_color = cv::Scalar(0, 0, 255);
        }

        // Display frame with predicted and ground truth rectangles, if known
        if (!gui.displayImage(frame,
                              found ? position : cv::Rect(),
                              rect_color,
                              gt))
            break;

		{
			for(long x=position.x;x<=(position.x+position.width);x++){
				for(long i=0;i<3;i++){frame.ptr(position.y+i,x)[0]=0;frame.ptr(position.y+i,x)[1]=255;frame.ptr(position.y+i,x)[2]=0;}
}
						for(long x=position.x;x<=(position.x+position.width);x++){
			for(long i=-2;i<1;i++){frame.ptr(position.y+i+position.height,x)[0]=0;frame.ptr(position.y+i+position.height,x)[1]=255;frame.ptr(position.y+i+position.height,x)[2]=0;}}
		















							for(long y=position.y;y<=(position.y+position.height);y++){
				for(long i=0;i<3;i++){frame.ptr(y,position.x+i)[0]=0;frame.ptr(y,position.x+i)[1]=255;frame.ptr(y,position.x+i)[2]=0;}
}
						for(long y=position.y;y<=(position.y+position.height);y++){
							for(long i=-2;i<1;i++){frame.ptr(y,position.x+i+position.width)[0]=0;frame.ptr(y,position.x+i+position.width)[1]=255;frame.ptr(y,position.x+i+position.width)[2]=0;}}
		
		}
        //if(sh>1000) break;
        sh++;
        captur<<frame;
    }

    if (gt_reader.isOpen())
    {
        std::pair<float, float> metrics = pr_evaluator.getMetrics();
        std::cout << "Precision:\t" << metrics.first << std::endl;
        std::cout << "Recall   :\t" << metrics.second << std::endl;
    }

    return 0;
}

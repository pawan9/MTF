#include "mtf/SM/GridTrackerFlow.h"
#include "mtf/Utilities/miscUtils.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdexcept>

_MTF_BEGIN_NAMESPACE

template <class AM, class SSM>
GridTrackerFlow<AM, SSM >::GridTrackerFlow(const ParamType *grid_params,
	const EstimatorParams *_est_params, const AMParams *am_params, 
	const SSMParams *ssm_params) :
	GridBase(), am(am_params), ssm(ssm_params),
	params(grid_params), est_params(_est_params){
	printf("\n");
	printf("Using Optical Flow Grid tracker with:\n");
	printf("grid_size: %d x %d\n", params.grid_size_x, params.grid_size_y);
	printf("search window size: %d x %d\n", params.search_window_x, params.search_window_y);
	printf("pyramid_levels: %d\n", params.pyramid_levels);
	printf("use_const_grad: %d\n", params.use_const_grad);
	printf("min_eig_thresh: %f\n", params.min_eig_thresh);
	printf("max_iters: %d\n", params.max_iters);
	printf("show_trackers: %d\n", params.show_trackers);
	printf("debug_mode: %d\n", params.debug_mode);
	printf("appearance model: %s\n", am.name.c_str());
	printf("state space model: %s\n", ssm.name.c_str());
	printf("\n");

	printf("Using %s estimator with:\n", ssm.name.c_str());
	est_params.print();

	name = "grid_flow";

	if(ssm.getResX() != params.getResX() || ssm.getResY() != params.getResY()){
		throw utils::InvalidArgument(
			cv::format("GridTrackerFlow: SSM has invalid sampling resolution: %d x %d",
			ssm.getResX(), ssm.getResY()));
	}

	n_pts = static_cast<unsigned int>(params.grid_size_x *params.grid_size_y);
	search_window = cv::Size(params.search_window_x, params.search_window_y);
	lk_termination_criteria = cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS,
		params.max_iters, params.epsilon);

	patch_corners.create(2, 4, CV_64FC1);
	cv_corners_mat.create(2, 4, CV_64FC1);

	prev_pts_mat.create(n_pts, 2, CV_32FC1);
	curr_pts_mat.create(n_pts, 2, CV_32FC1);

	prev_pts.resize(n_pts);
	curr_pts.resize(n_pts);

	ssm_update.resize(ssm.getStateSize());
	pix_mask.resize(n_pts);
	std::fill(pix_mask.begin(), pix_mask.end(), 1);
	pause_seq = 0;

	if(params.show_trackers){
		patch_win_name = "Patch Trackers";
		cv::namedWindow(patch_win_name);
	}
}

template <class AM, class SSM>
void GridTrackerFlow<AM, SSM >::initialize(const cv::Mat &corners) {
	ssm.initialize(corners);

	for(unsigned int pt_id = 0; pt_id < n_pts; ++pt_id){
		Vector2d patch_centroid = ssm.getPts().col(pt_id);
		prev_pts[pt_id].x = static_cast<float>(patch_centroid(0));
		prev_pts[pt_id].y = static_cast<float>(patch_centroid(1));
	}
	am.getCurrImg().copyTo(prev_img);

	ssm.getCorners(cv_corners_mat);
	if(params.show_trackers){ showPts(); }
}

template <class AM, class SSM>
void GridTrackerFlow<AM, SSM >::update() {
	am.estimateOpticalFlow(curr_pts, prev_img, prev_pts, search_window, n_pts,
		params.max_iters, params.epsilon, params.use_const_grad);
	ssm.estimateWarpFromPts(ssm_update, pix_mask, prev_pts, curr_pts, est_params);

	Matrix24d opt_warped_corners;
	ssm.applyWarpToCorners(opt_warped_corners, ssm.getCorners(), ssm_update);
	ssm.setCorners(opt_warped_corners);

	for(unsigned int pt_id = 0; pt_id < n_pts; ++pt_id){
		prev_pts[pt_id].x = curr_pts[pt_id].x;
		prev_pts[pt_id].y = curr_pts[pt_id].y;
	}
	ssm.getCorners(cv_corners_mat);
	am.getCurrImg().copyTo(prev_img);

	if(params.show_trackers){ showPts(); }
}

template <class AM, class SSM>
void GridTrackerFlow<AM, SSM >::setImage(const cv::Mat &img){
	am.setCurrImg(img);
	if(curr_img.empty()){
		prev_img.create(img.rows, img.cols, am.inputType());
	}
	if(params.show_trackers && curr_img_disp.empty()){
		curr_img_disp.create(img.rows, img.cols, CV_8UC3);
	}
	curr_img = img;
}


template <class AM, class SSM>
void GridTrackerFlow<AM, SSM >::setRegion(const cv::Mat& corners) {
	ssm.setCorners(corners);
	for(unsigned int pt_id = 0; pt_id < n_pts; ++pt_id){
		prev_pts[pt_id].x = static_cast<float>(ssm.getPts()(0, pt_id));
		prev_pts[pt_id].y = static_cast<float>(ssm.getPts()(1, pt_id));
	}
	ssm.getCorners(cv_corners_mat);
}

template <class AM, class SSM>
void GridTrackerFlow<AM, SSM >::showPts(){
	curr_img.convertTo(curr_img_disp, curr_img_disp.type());
	cv::cvtColor(curr_img_disp, curr_img_disp, cv::COLOR_GRAY2BGR);
	utils::drawRegion(curr_img_disp, cv_corners_mat, CV_RGB(0, 0, 255), 2);
	for(unsigned int pt_id = 0; pt_id < n_pts; ++pt_id) {
		cv::Scalar tracker_color;
		if(pix_mask[pt_id]){
			tracker_color = cv::Scalar(0, 255, 0);
		} else{
			tracker_color = cv::Scalar(0, 0, 255);
		}
		circle(curr_img_disp, curr_pts[pt_id], 2, tracker_color, 2);
	}
	imshow(patch_win_name, curr_img_disp);
	//int key = cv::waitKey(1 - pause_seq);
	//if(key == 32){
	//	pause_seq = 1 - pause_seq;
	//}
}

_MTF_END_NAMESPACE

#ifndef HEADER_ONLY_MODE
#include "mtf/Macros/register.h"
_REGISTER_TRACKERS(GridTrackerFlow);
#endif
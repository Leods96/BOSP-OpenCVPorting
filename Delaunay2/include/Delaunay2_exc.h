/**
 *       @file  Delaunay2_exc.h
 *      @brief  The Delaunay2 BarbequeRTRM application
 *
 * Description: Adaptation of the opencv sample Delaunay2.cpp to AEM
 *
 *     @author  Leonardo Romano (10529860), leonardo1.romano@mail.polimi.it
 *
 *     Company  Politecnico di Milano
 *   Copyright  Copyright (c) 2020, Leonardo Romano
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License as published by the Free Software Foundation.
 * =====================================================================================
 */

#ifndef DELAUNAY2_EXC_H_
#define DELAUNAY2_EXC_H_

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

#include <bbque/bbque_exc.h>
#include <bbque/utils/utility.h>

using bbque::rtlib::BbqueEXC;
using namespace cv;
using namespace std;

class Delaunay2 : public BbqueEXC {

public:

	Delaunay2(std::string const & name,
			std::string const & recipe,
			RTLIB_Services_t *rtlib);

private:

	RTLIB_ExitCode_t onSetup();
	RTLIB_ExitCode_t onConfigure(int8_t awm_id);
	RTLIB_ExitCode_t onRun();
	RTLIB_ExitCode_t onMonitor();
	RTLIB_ExitCode_t onSuspend();
	RTLIB_ExitCode_t onRelease();
};

#endif // DELAUNAY2_EXC_H_

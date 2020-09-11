/**
 *       @file  Delaunay2_exc.cc
 *      @brief  The Delaunay2 BarbequeRTRM application
 *
 * Description: Adaptation of the opencv sample delaunay2.cpp to AEM
 *
 *     @author  Leonardo Romano (10529860), leonardo1.romano@mail.polimi.it
 *
 *     Company  Your Company
 *   Copyright  Copyright (c) 2020, Leonardo Romano
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License as published by the Free Software Foundation.
 * =====================================================================================
 */


#include "Delaunay2_exc.h"

#include <cstdio>
#include <bbque/utils/utility.h>

//libraries
#include <iostream>

// Setup logging
#undef  BBQUE_LOG_MODULE
#define BBQUE_LOG_MODULE "aem.delaunay2"
#undef  BBQUE_LOG_UID
#define BBQUE_LOG_UID GetChUid()

//functions declaration
static void draw_subdiv_point( Mat& img, Point2f fp, Scalar color );
static void draw_subdiv( Mat& img, Subdiv2D& subdiv, Scalar delaunay_color );
static void locate_point( Mat& img, Subdiv2D& subdiv, Point2f fp, Scalar active_color );
static void paint_voronoi( Mat& img, Subdiv2D& subdiv );

//vars
bool stopFlag = false;

Delaunay2::Delaunay2(std::string const & name,
		std::string const & recipe,
		RTLIB_Services_t *rtlib) :
	BbqueEXC(name, recipe, rtlib) {

	logger->Warn("New Delaunay2::Delaunay2()");
	logger->Info("EXC Unique IDentifier (UID): %u", GetUniqueID());

}

RTLIB_ExitCode_t Delaunay2::onSetup() {
	logger->Warn("Delaunay2::onSetup()");

	return RTLIB_OK;
}

RTLIB_ExitCode_t Delaunay2::onConfigure(int8_t awm_id) {

	logger->Warn("Delaunay2::onConfigure(): EXC [%s] => AWM [%02d]",
		exc_name.c_str(), awm_id);

	int32_t proc_quota, proc_nr, mem;
	GetAssignedResources(PROC_ELEMENT, proc_quota);
	GetAssignedResources(PROC_NR, proc_nr);
	GetAssignedResources(MEMORY, mem);
	logger->Notice("MayApp::onConfigure(): "
		"EXC [%s], AWM[%02d] => R<PROC_quota>=%3d, R<PROC_nr>=%2d, R<MEM>=%3d",
		exc_name.c_str(), awm_id, proc_quota, proc_nr, mem);

	return RTLIB_OK;
}

RTLIB_ExitCode_t Delaunay2::onRun() {
	logger->Warn("Delaunay2::onRun()");

    Scalar active_facet_color(0, 0, 255), delaunay_color(255,255,255);
    Rect rect(0, 0, 600, 600);

    Subdiv2D subdiv(rect);
    Mat img(rect.size(), CV_8UC3);

    img = Scalar::all(0);
    string win = "Delaunay Demo";
    imshow(win, img);

    for( int i = 0; i < 200 && !stopFlag; i++ )
    {
        Point2f fp( (float)(rand()%(rect.width-10)+5),
                    (float)(rand()%(rect.height-10)+5));

        locate_point( img, subdiv, fp, active_facet_color );
        imshow( win, img );

        if( waitKey( 100 ) >= 0 )
            stopFlag = true;

        subdiv.insert(fp);

        img = Scalar::all(0);
        draw_subdiv( img, subdiv, delaunay_color );
        imshow( win, img );

        if( waitKey( 100 ) >= 0 )
            stopFlag = true;
    }
    if(!stopFlag) {
    	img = Scalar::all(0);
    	paint_voronoi( img, subdiv );
    	imshow( win, img );

    	waitKey(0);
	}

	return RTLIB_OK;
}

RTLIB_ExitCode_t Delaunay2::onMonitor() {

	if (stopFlag){
        logger->Warn("Delaunay2::onMonitor()  : exit");
        return RTLIB_EXC_WORKLOAD_NONE;
    }

	return RTLIB_OK;
}

RTLIB_ExitCode_t Delaunay2::onSuspend() {

	logger->Warn("Delaunay2::onSuspend()  : suspension...");

	return RTLIB_OK;
}

RTLIB_ExitCode_t Delaunay2::onRelease() {

	logger->Warn("Delaunay2::onRelease()  : exit");

	return RTLIB_OK;
}

//Functions
static void draw_subdiv_point( Mat& img, Point2f fp, Scalar color )
{
    circle( img, fp, 3, color, FILLED, LINE_8, 0 );
}

static void draw_subdiv( Mat& img, Subdiv2D& subdiv, Scalar delaunay_color )
{
    vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    vector<Point> pt(3);

    for( size_t i = 0; i < triangleList.size(); i++ )
    {
        Vec6f t = triangleList[i];
        pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = Point(cvRound(t[4]), cvRound(t[5]));
        line(img, pt[0], pt[1], delaunay_color, 1, LINE_AA, 0);
        line(img, pt[1], pt[2], delaunay_color, 1, LINE_AA, 0);
        line(img, pt[2], pt[0], delaunay_color, 1, LINE_AA, 0);
    }
}

static void locate_point( Mat& img, Subdiv2D& subdiv, Point2f fp, Scalar active_color )
{
    int e0=0, vertex=0;

    subdiv.locate(fp, e0, vertex);

    if( e0 > 0 )
    {
        int e = e0;
        do
        {
            Point2f org, dst;
            if( subdiv.edgeOrg(e, &org) > 0 && subdiv.edgeDst(e, &dst) > 0 )
                line( img, org, dst, active_color, 3, LINE_AA, 0 );

            e = subdiv.getEdge(e, Subdiv2D::NEXT_AROUND_LEFT);
        }
        while( e != e0 );
    }

    draw_subdiv_point( img, fp, active_color );
}

static void paint_voronoi( Mat& img, Subdiv2D& subdiv )
{
    vector<vector<Point2f> > facets;
    vector<Point2f> centers;
    subdiv.getVoronoiFacetList(vector<int>(), facets, centers);

    vector<Point> ifacet;
    vector<vector<Point> > ifacets(1);

    for( size_t i = 0; i < facets.size(); i++ )
    {
        ifacet.resize(facets[i].size());
        for( size_t j = 0; j < facets[i].size(); j++ )
            ifacet[j] = facets[i][j];

        Scalar color;
        color[0] = rand() & 255;
        color[1] = rand() & 255;
        color[2] = rand() & 255;
        fillConvexPoly(img, ifacet, color, 8, 0);

        ifacets[0] = ifacet;
        polylines(img, ifacets, true, Scalar(), 1, LINE_AA, 0);
        circle(img, centers[i], 3, Scalar(), FILLED, LINE_AA, 0);
    }
}

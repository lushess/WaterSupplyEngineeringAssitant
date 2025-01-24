#pragma once
#include "StdAfx.h"
#include "CDatabaseUtil.h"
namespace CPolylineUtil
{

	//根据顶点数组创建轻量多线段
	AcDbObjectId add(const AcGePoint2dArray& Points, double width = 0);

	//根据顶点创建三维多线段
	AcDbObjectId add(const AcGePoint3dArray& Points);

	//根据中心点、边数、外接圆半径、旋转角度（弧度）和线宽创建正多边形
	AcDbObjectId addPolygon(AcGePoint2d& pCenterPoint, int number, double radius, double ratation, double width = 0);

	//根据两个角点创建矩形
	AcDbObjectId addRect(const AcGePoint2d& Point1, const AcGePoint2d& Point2, double width = 0);

	void SetAxonometry45AllVertex(AcDbPolyline* pPolyline);

	void getAllVertex(AcGePoint2dArray& ptArr2D, AcDbPolyline* pPolyline);

	AcGePoint3d getCenter(AcDbPolyline* pPolyline);
}


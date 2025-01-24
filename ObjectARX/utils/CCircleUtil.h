#pragma once
#include "StdAfx.h"
#include "CGetPointUtil.h"
#include "CDatabaseUtil.h"
namespace CCircleUtil
{
	//圆心、圆所在平面和半径作圆
	AcDbObjectId add(const AcGePoint3d& Point, const AcGeVector3d& Normal, double radius);

	//圆心、XOY平面和半径作圆
	AcDbObjectId add(AcGePoint3d& Point, double radius);

	//两点在XOY平面上作圆
	AcDbObjectId add(AcGePoint2d& FirstPoint, AcGePoint2d& SecondPoint);

	//三点在XOY平面作圆
	AcDbObjectId add(AcGePoint2d& FirstPoint, AcGePoint2d& SecondPoint, AcGePoint2d& ThirdPoint);

	void setAxonometry45Destination(AcDbCircle* pCircle);
}


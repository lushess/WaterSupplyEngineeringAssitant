#pragma once
#include "StdAfx.h"
class CGetPointUtil
{
public:
	inline static AcGePoint3d Get3DPointFrom2DPointByElevation(const AcGePoint2d& point2D, const double& elevation)
	{
		return AcGePoint3d(point2D.x, point2D.y, elevation);
	}
	//求两个3d点连线的中点
	static AcGePoint3d getMiddlePoint(const AcGePoint3d& FirstPoint, const AcGePoint3d& SecondPoint);

	//求两个2d点连线的中点
	static AcGePoint2d getMiddlePoint(const AcGePoint2d& FirstPoint, const AcGePoint2d& SecondPoint);

	static AcGePoint3d GetAxonometry45Destination(const AcGePoint3d& srcPoint);

	static AcGePoint2d GetAxonometry45Destination(const AcGePoint2d& srcPoint, const double& elevation);

	static AcGePoint3d getCenterPointFromArray(const AcGePoint3dArray& ptArr3D);

	static AcGePoint3d getCenterPointFromArray(const AcGePoint2dArray& ptArr2D, const double elevation = 0);

};


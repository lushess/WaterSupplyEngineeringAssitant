#include "stdafx.h"
#include "CGetPointUtil.h"


AcGePoint3d CGetPointUtil::getMiddlePoint(const AcGePoint3d& FirstPoint, const AcGePoint3d& SecondPoint)
{
	double x = (FirstPoint.x + SecondPoint.x) * 0.5;
	double y = (FirstPoint.y + SecondPoint.y) * 0.5;
	double z = (FirstPoint.z + SecondPoint.z) * 0.5;
	return AcGePoint3d(x, y, z);
}

AcGePoint2d CGetPointUtil::getMiddlePoint(const AcGePoint2d& FirstPoint, const AcGePoint2d& SecondPoint)
{
	double x = (FirstPoint.x + SecondPoint.x) * 0.5;
	double y = (FirstPoint.y + SecondPoint.y) * 0.5;
	return AcGePoint2d(x, y);
}

AcGePoint3d CGetPointUtil::GetAxonometry45Destination(const AcGePoint3d& srcPoint)
{
	double sqrt2 = 1.4142135623730950488;
	double x = srcPoint.x;
	double y = srcPoint.y;
	double z = srcPoint.z;

	x = x + (sqrt2 / 4) * y;
	y = (sqrt2 / 4) * y + z;
	z = 0;

	return AcGePoint3d(x,y,z);
}

AcGePoint2d CGetPointUtil::GetAxonometry45Destination(const AcGePoint2d& srcPoint, const double& elevation)
{
	double sqrt2 = 1.4142135623730950488;
	double x = srcPoint.x;
	double y = srcPoint.y;

	x = x + (sqrt2 / 4) * y;
	y = (sqrt2 / 4) * y + elevation;

	return AcGePoint2d(x, y);
}

/*
AcGePoint3d CGetPointUtil::Get3DPointFrom2DPointByElevation(const AcGePoint2d& point2D, const double elevation)
{
	return AcGePoint3d(point2D.x, point2D.y, elevation);
}
*/

AcGePoint3d CGetPointUtil::getCenterPointFromArray(const AcGePoint3dArray& ptArr3D)
{
	unsigned int len = ptArr3D.length();
	AcGeVector3d tmpVec = AcGeVector3d();

	for (unsigned int i = 0 ; i<len ; i++)
	{
		tmpVec += ptArr3D.at(i).asVector();
	}

	tmpVec /= len;

	return AcGePoint3d(tmpVec.x, tmpVec.y, tmpVec.z);
}


AcGePoint3d CGetPointUtil::getCenterPointFromArray(const AcGePoint2dArray& ptArr2D, const double elevation)
{
	unsigned int len = ptArr2D.length();
	AcGeVector2d tmpVec = AcGeVector2d();

	for (unsigned int i = 0; i < len; i++)
	{
		tmpVec += ptArr2D.at(i).asVector();
	}

	tmpVec /= len;

	return AcGePoint3d(tmpVec.x, tmpVec.y, elevation);
}

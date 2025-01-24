#pragma once
#include "StdAfx.h"
#include "CGetPointUtil.h"
#include "CDatabaseUtil.h"
namespace CCircleUtil
{
	//Բ�ġ�Բ����ƽ��Ͱ뾶��Բ
	AcDbObjectId add(const AcGePoint3d& Point, const AcGeVector3d& Normal, double radius);

	//Բ�ġ�XOYƽ��Ͱ뾶��Բ
	AcDbObjectId add(AcGePoint3d& Point, double radius);

	//������XOYƽ������Բ
	AcDbObjectId add(AcGePoint2d& FirstPoint, AcGePoint2d& SecondPoint);

	//������XOYƽ����Բ
	AcDbObjectId add(AcGePoint2d& FirstPoint, AcGePoint2d& SecondPoint, AcGePoint2d& ThirdPoint);

	void setAxonometry45Destination(AcDbCircle* pCircle);
}


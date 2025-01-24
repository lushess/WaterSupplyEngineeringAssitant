#pragma once
#include "StdAfx.h"
#include "CDatabaseUtil.h"
namespace CPolylineUtil
{

	//���ݶ������鴴���������߶�
	AcDbObjectId add(const AcGePoint2dArray& Points, double width = 0);

	//���ݶ��㴴����ά���߶�
	AcDbObjectId add(const AcGePoint3dArray& Points);

	//�������ĵ㡢���������Բ�뾶����ת�Ƕȣ����ȣ����߿����������
	AcDbObjectId addPolygon(AcGePoint2d& pCenterPoint, int number, double radius, double ratation, double width = 0);

	//���������ǵ㴴������
	AcDbObjectId addRect(const AcGePoint2d& Point1, const AcGePoint2d& Point2, double width = 0);

	void SetAxonometry45AllVertex(AcDbPolyline* pPolyline);

	void getAllVertex(AcGePoint2dArray& ptArr2D, AcDbPolyline* pPolyline);

	AcGePoint3d getCenter(AcDbPolyline* pPolyline);
}


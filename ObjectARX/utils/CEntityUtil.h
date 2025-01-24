#pragma once
#include "StdAfx.h"

namespace CEntityUtil
{
	enum EntityType {
		isLine = 0,
		is2DPline,
		isPline,
		isSpline,
		isCircle,
		isArc,
		isBlockReference,
		isText,
		isMText,
		isHatch,

		unknowType = 0xff
	};

	//�޸�ʵ����ɫ
	void setColor(AcDbObjectId objectID, int ColorIndex);

	//�޸�ʵ���ͼ��
	void setLayer(AcDbObjectId objectID, const TCHAR* LayerName);

	//�޸�ʵ�������
	void setLineType(AcDbObjectId objectID, const TCHAR* LineType);

	//����ָ���Ƕ���һ����תָ����ʵ��
	Acad::ErrorStatus Rotate(AcDbObjectId objectID, const AcGePoint2d& pBasePoint, double rotationAngle);

	//��ʵ���һ���ƶ�������һ��
	Acad::ErrorStatus Move(AcDbObjectId objectID, AcGePoint3d& pBasePoint, AcGePoint3d& pDest);

	//��һ�㰴��������ָ����ʵ��
	Acad::ErrorStatus Scale(AcDbObjectId objectID, const AcGePoint3d& pBasePoint, double scaleSize);

	//��ȡʵ������
	EntityType getEntityType(AcDbEntity* pEntity);

	//��ȡʵ������ĵ�
	AcGePoint3d getEntityCeterPoint(AcDbObjectId objectID);

	AcGePoint3d getEntityCeterPoint(AcDbEntity* pEntity);

}


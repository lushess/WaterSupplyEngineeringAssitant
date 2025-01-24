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

	//修改实体颜色
	void setColor(AcDbObjectId objectID, int ColorIndex);

	//修改实体的图层
	void setLayer(AcDbObjectId objectID, const TCHAR* LayerName);

	//修改实体的线性
	void setLineType(AcDbObjectId objectID, const TCHAR* LineType);

	//按照指定角度沿一点旋转指定的实体
	Acad::ErrorStatus Rotate(AcDbObjectId objectID, const AcGePoint2d& pBasePoint, double rotationAngle);

	//把实体从一点移动到另外一点
	Acad::ErrorStatus Move(AcDbObjectId objectID, AcGePoint3d& pBasePoint, AcGePoint3d& pDest);

	//沿一点按比列缩放指定的实体
	Acad::ErrorStatus Scale(AcDbObjectId objectID, const AcGePoint3d& pBasePoint, double scaleSize);

	//获取实体类型
	EntityType getEntityType(AcDbEntity* pEntity);

	//获取实体的中心点
	AcGePoint3d getEntityCeterPoint(AcDbObjectId objectID);

	AcGePoint3d getEntityCeterPoint(AcDbEntity* pEntity);

}


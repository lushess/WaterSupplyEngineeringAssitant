#pragma once
#include "StdAfx.h"
#include "CDatabaseUtil.h"
namespace CLineUtil
{
	typedef AcArray<AcDbLine*> AcDbpLineArray;
	//ͨ�������յ㹹��һ��ֱ��
	AcDbObjectId add(const AcGePoint3d& pStartPoint, const AcGePoint3d& pEndPoint);

	inline AcGePoint3d startPoint(const AcDbLine* pLine);

	inline AcGePoint3d endPoint(const AcDbLine* pLine);

	void SetAxonometry45StartPointEndPoint(AcDbLine* pLine);

	inline double getLenth(const AcDbLine* pLine)
	{
		return pLine->startPoint().distanceTo(pLine->endPoint());
	}

	double getLenth(const AcDbpLineArray* pLineArr, const TCHAR* layerName = NULL);
	double getLenth(const AcDbObjectIdArray* pLineIds, const TCHAR* layerName = NULL);
}

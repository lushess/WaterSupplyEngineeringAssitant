#pragma once
#include "StdAfx.h"
#include "CGetPointUtil.h"
namespace CSplineUtil
{
	void SetAxonometry45AllControlPoints(AcDbSpline* pSpline);
	
	void getAllControlPoints(AcGePoint3dArray& ptArr3D, AcDbSpline* pSpline);

	AcGePoint3d getCenter(AcDbSpline* pSpline);

};


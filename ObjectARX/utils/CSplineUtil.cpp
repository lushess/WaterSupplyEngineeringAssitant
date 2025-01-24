#include "stdafx.h"
#include "CSplineUtil.h"

namespace CSplineUtil
{
	void SetAxonometry45AllControlPoints(AcDbSpline* pSpline)
	{
		int numPts = pSpline->numControlPoints();

		for (int i = 0; i < numPts; i++)
		{
			AcGePoint3d ctrlPt;
			pSpline->getControlPointAt(i, ctrlPt);
			ctrlPt = CGetPointUtil::GetAxonometry45Destination(ctrlPt);
			pSpline->setControlPointAt(i, ctrlPt);
		}
	}

	void getAllControlPoints(AcGePoint3dArray& ptArr3D, AcDbSpline* pSpline)
	{
		int numPts = pSpline->numControlPoints();

		for (int i = 0; i < numPts; i++)
		{
			AcGePoint3d ctrlPt;
			pSpline->getControlPointAt(i, ctrlPt);
			ptArr3D.append(ctrlPt);
		}
	}

	AcGePoint3d getCenter(AcDbSpline* pSpline)
	{
		AcGePoint3dArray ptArr3D;
		getAllControlPoints(ptArr3D, pSpline);
		return CGetPointUtil::getCenterPointFromArray(ptArr3D);
	}
}
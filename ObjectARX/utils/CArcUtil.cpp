#include "stdafx.h"
#include "CArcUtil.h"
#include "CGetPointUtil.h"
namespace CArcUtil
{
	void setAxonometry45Destination(AcDbArc* pArc)
	{
		pArc->setCenter(CGetPointUtil::GetAxonometry45Destination(pArc->center()));
	}
}
#include "stdafx.h"
#include "CEntityUtil.h"
#include "CDatabaseUtil.h"
#include "CGetPointUtil.h"
#include "CLineUtil.h"
#include "CCircleUtil.h"
#include "CArcUtil.h"
#include "CBlockReferenceUtil.h"
#include "CPolylineUtil.h"
#include "CSplineUtil.h"
#include "axonometry.h"
namespace Axonometry45Util
{
	Acad::ErrorStatus ChangeToAxonometry45(const AcDbObjectId Id)
	{
		Acad::ErrorStatus es;
		AcDbEntity* pEntToChange = NULL;
		es = acdbOpenObject(pEntToChange, Id, AcDb::kForRead);

		if (Acad::eOk == es)
		{
			CEntityUtil::EntityType entType = CEntityUtil::getEntityType(pEntToChange);

			switch (entType)
			{
			case CEntityUtil::isLine:
				CLineUtil::SetAxonometry45StartPointEndPoint(dynamic_cast<AcDbLine*>(pEntToChange));
				break;
			case CEntityUtil::isCircle:
				CCircleUtil::setAxonometry45Destination(dynamic_cast<AcDbCircle*>(pEntToChange));
				break;
			case CEntityUtil::isArc:
				CArcUtil::setAxonometry45Destination(dynamic_cast<AcDbArc*>(pEntToChange));
				break;
			case CEntityUtil::isBlockReference:
				CBlockReferenceUtil::setAxonometry45Destination(dynamic_cast<AcDbBlockReference*>(pEntToChange));
				break;
			case CEntityUtil::isPline:
				CPolylineUtil::SetAxonometry45AllVertex(dynamic_cast<AcDbPolyline*>(pEntToChange));
				break;
			case CEntityUtil::isSpline:
				CSplineUtil::SetAxonometry45AllControlPoints(dynamic_cast<AcDbSpline*>(pEntToChange));
				break;


				// TODO:添加更多对应实体类型的方法
			default:
				pEntToChange->close();
				return Acad::eUnknownHandle;
			}
		}

		pEntToChange->close();
		return es;
	}


	Acad::ErrorStatus ChangeToAxonometry45(AcDbEntity* pEnt)
	{
		Acad::ErrorStatus es = Acad::eOk;

		if (Acad::eOk == es)
		{
			CEntityUtil::EntityType entType = CEntityUtil::getEntityType(pEnt);

			switch (entType)
			{
			case CEntityUtil::isLine:
				CLineUtil::SetAxonometry45StartPointEndPoint(dynamic_cast<AcDbLine*>(pEnt));
				break;
			case CEntityUtil::isCircle:
				CCircleUtil::setAxonometry45Destination(dynamic_cast<AcDbCircle*>(pEnt));
				break;
			case CEntityUtil::isArc:
				CArcUtil::setAxonometry45Destination(dynamic_cast<AcDbArc*>(pEnt));
				break;
			case CEntityUtil::isBlockReference:
				CBlockReferenceUtil::setAxonometry45Destination(dynamic_cast<AcDbBlockReference*>(pEnt));
				break;
			case CEntityUtil::isPline:
				CPolylineUtil::SetAxonometry45AllVertex(dynamic_cast<AcDbPolyline*>(pEnt));
				break;
			case CEntityUtil::isSpline:
				CSplineUtil::SetAxonometry45AllControlPoints(dynamic_cast<AcDbSpline*>(pEnt));
				break;


				// TODO:添加更多对应实体类型的方法
			default:
				return Acad::eUnknownHandle;
			}
		}

		return es;
	}
}

void ChangeSelectedEntityToAxonometry45(void)
{
	AcDbObjectIdArray* selectedIDs = new AcDbObjectIdArray;

	CDatabaseUtil::getSelectedIDs(selectedIDs);

	for (int i = 0; i < selectedIDs->length(); i++)
	{
		AcDbEntity* pEntToChange = NULL;
		acdbOpenObject(pEntToChange, selectedIDs->at(i), AcDb::kForRead);
		if (CEntityUtil::isBlockReference == CEntityUtil::getEntityType(pEntToChange))
		{
			AcDbObjectIdArray* exids = new AcDbObjectIdArray;
			CBlockReferenceUtil::explodeBlockReference(exids, dynamic_cast<AcDbBlockReference*>(pEntToChange));

			for (int j = 0; j < exids->length(); j++)
				CDatabaseUtil::saveEntityChangement(exids->at(j), Axonometry45Util::ChangeToAxonometry45);

			delete exids;
			exids = NULL;
		}
		else CDatabaseUtil::saveEntityChangement(selectedIDs->at(i), Axonometry45Util::ChangeToAxonometry45);
		pEntToChange->close();
	}

	delete selectedIDs;
	selectedIDs = NULL;
}

void ChangeSelectedEntityToAxonometry45Init(void)
{
	acedRegCmds->addCommand((L"Axonametry"), (L"Axonametry45"), (L"轴测图45度"), ACRX_CMD_MODAL, ChangeSelectedEntityToAxonometry45);
}

void ChangeSelectedEntityToAxonometry45Unload(void)
{
	acedRegCmds->removeGroup((L"Axonametry"));
}
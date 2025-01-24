#include "stdafx.h"
#include "CEntityUtil.h"
#include "CGetPointUtil.h"
#include "CPolylineUtil.h"
#include "CSplineUtil.h"
#include "CBlockReferenceUtil.h"
namespace CEntityUtil
{

	void setColor(AcDbObjectId objectID, int ColorIndex)
	{
		
		assert(ColorIndex >= 0 && ColorIndex <= 255);
		AcDbEntity* pEntity;
		if (acdbOpenAcDbEntity(pEntity, objectID, AcDb::kForWrite) == Acad::eOk)
		{
			pEntity->setColorIndex(ColorIndex);
			pEntity->close();
		}
	}

	void setLayer(AcDbObjectId objectID, const TCHAR* LayerName)
	{
		AcDbEntity* pEntity;
		if (acdbOpenAcDbEntity(pEntity, objectID, AcDb::kForWrite) == Acad::eOk)
		{
			pEntity->setLayer(LayerName);
			pEntity->close();
		}
	}

	void setLineType(AcDbObjectId objectID, const TCHAR* LineType)
	{
		AcDbEntity* pEntity;
		if (acdbOpenAcDbEntity(pEntity, objectID, AcDb::kForWrite) == Acad::eOk)
		{
			pEntity->setLinetype(LineType);
			pEntity->close();
		}
	}

	Acad::ErrorStatus Rotate(AcDbObjectId objectid, const AcGePoint2d& pBasePoint, double rotationAngle)
	{
		AcGeMatrix3d xform;
		AcGeVector3d vec(0, 0, 1);
		AcGePoint3d RotatePoint(pBasePoint.x, pBasePoint.y, 0);
		xform.setToRotation(rotationAngle, vec, RotatePoint);

		Acad::ErrorStatus es;
		AcDbEntity* pEntity;
		es = acdbOpenObject(pEntity, objectid, AcDb::kForWrite);
		if (Acad::eOk == es)
		{
			es = pEntity->transformBy(xform);
			pEntity->close();
		}
		return es;
	}

	Acad::ErrorStatus Move(AcDbObjectId objectID, AcGePoint3d& pBasePoint, AcGePoint3d& pDest)
	{
		AcGeMatrix3d xform;
		AcGeVector3d vec(pDest.x - pBasePoint.x, pDest.y - pBasePoint.y, pDest.z - pBasePoint.z);
		xform.setToTranslation(vec);
		Acad::ErrorStatus es;
		AcDbEntity* pEntity;
		es = acdbOpenObject(pEntity, objectID, AcDb::kForWrite);
		if (Acad::eOk == es)
		{
			es = pEntity->transformBy(xform);
			pEntity->close();
		}
		return es;
	}

	Acad::ErrorStatus Scale(AcDbObjectId objectID, const AcGePoint3d& pBasePoint, double scaleSize)
	{
		AcGeMatrix3d xform;
		xform.setToScaling(scaleSize, pBasePoint);
		Acad::ErrorStatus es;
		AcDbEntity* pEntity;
		es = acdbOpenObject(pEntity, objectID, AcDb::kForWrite);
		if (Acad::eOk == es)
		{
			es = pEntity->transformBy(xform);
			pEntity->close();
		}
		return es;
	}

	AcGePoint3d getEntityCeterPoint(AcDbObjectId objectID)
	{
		Acad::ErrorStatus es;
		AcDbEntity* pEntToChange = NULL;
		es = acdbOpenObject(pEntToChange, objectID, AcDb::kForRead);

		if (Acad::eOk == es)
		{
			EntityType entType = getEntityType(pEntToChange);
			pEntToChange->close();

			switch (entType)
			{
			case isLine:
			{
				es = acdbOpenObject(pEntToChange, objectID, AcDb::kForRead);
				AcGePoint3d startPt = dynamic_cast<AcDbLine*>(pEntToChange)->startPoint();
				AcGePoint3d endPt = dynamic_cast<AcDbLine*>(pEntToChange)->endPoint();
				pEntToChange->close();
				return AcGePoint3d(CGetPointUtil::getMiddlePoint(startPt, endPt));
			}
			case isCircle:
			{
				es = acdbOpenObject(pEntToChange, objectID, AcDb::kForRead);
				AcGePoint3d centerPt = dynamic_cast<AcDbCircle*>(pEntToChange)->center();
				pEntToChange->close();
				return centerPt;
			}
			case isBlockReference:
			{
				es = acdbOpenObject(pEntToChange, objectID, AcDb::kForRead);
				AcGePoint3d centerPt = CBlockReferenceUtil::getCenter(dynamic_cast<AcDbBlockReference*>(pEntToChange));
				pEntToChange->close();
				return centerPt;
			}
			case isPline:
			{
				es = acdbOpenObject(pEntToChange, objectID, AcDb::kForRead);
				AcGePoint3d centerPt = CPolylineUtil::getCenter(dynamic_cast<AcDbPolyline*>(pEntToChange));
				pEntToChange->close();
				return centerPt;
			}
			case isSpline:
			{
				es = acdbOpenObject(pEntToChange, objectID, AcDb::kForRead);
				AcGePoint3d centerPt = CSplineUtil::getCenter(dynamic_cast<AcDbSpline*>(pEntToChange));
				pEntToChange->close();
				return centerPt;
			}

			
			default:
				break;
			}
		}
		return AcGePoint3d();
	}

	AcGePoint3d getEntityCeterPoint(AcDbEntity* pEntity)
	{
		EntityType entType = getEntityType(pEntity);
		pEntity->close();

		switch (entType)
		{
		case isLine:
		{
			AcGePoint3d startPt = dynamic_cast<AcDbLine*>(pEntity)->startPoint();
			AcGePoint3d endPt = dynamic_cast<AcDbLine*>(pEntity)->endPoint();
			pEntity->close();
			return AcGePoint3d(CGetPointUtil::getMiddlePoint(startPt, endPt));
		}
		case isCircle:
		{
			AcGePoint3d centerPt = dynamic_cast<AcDbCircle*>(pEntity)->center();
			pEntity->close();
			return centerPt;
		}
		case isBlockReference:
		{
			AcGePoint3d centerPt = CBlockReferenceUtil::getCenter(dynamic_cast<AcDbBlockReference*>(pEntity));
			pEntity->close();
			return centerPt;
		}
		case isPline:
		{
			AcGePoint3d centerPt = CPolylineUtil::getCenter(dynamic_cast<AcDbPolyline*>(pEntity));
			pEntity->close();
			return centerPt;
		}
		case isSpline:
		{
			AcGePoint3d centerPt = CSplineUtil::getCenter(dynamic_cast<AcDbSpline*>(pEntity));
			pEntity->close();
			return centerPt;
		}

		
		default:
			break;
		}
		return AcGePoint3d();
	}

	/*
	*
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
	*/

	EntityType getEntityType(AcDbEntity* pEntity)
	{
		if (AcDbLine::desc() == pEntity->isA()) return EntityType::isLine;

		if (AcDb2dPolyline::desc() == pEntity->isA()) return EntityType::is2DPline;

		if (AcDbPolyline::desc() == pEntity->isA()) return EntityType::isPline;

		if (AcDbSpline::desc() == pEntity->isA()) return EntityType::isSpline;

		if (AcDbCircle::desc() == pEntity->isA()) return EntityType::isCircle;

		if (AcDbArc::desc() == pEntity->isA()) return EntityType::isArc;

		if (AcDbBlockReference::desc() == pEntity->isA()) return EntityType::isBlockReference;

		if (AcDbText::desc() == pEntity->isA()) return EntityType::isText;

		if (AcDbMText::desc() == pEntity->isA()) return EntityType::isMText;

		if (AcDbHatch::desc() == pEntity->isA()) return EntityType::isHatch;

		return EntityType::unknowType;
	}

}
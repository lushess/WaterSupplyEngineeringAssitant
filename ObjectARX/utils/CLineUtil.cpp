#include "stdafx.h"
#include "CLineUtil.h"
#include "CGetPointUtil.h"
namespace CLineUtil
{

	AcDbObjectId add(const AcGePoint3d& pStartPoint, const AcGePoint3d& pEndPoint)
	{
		AcDbLine* pLine = new AcDbLine(pStartPoint, pEndPoint);
		return CDatabaseUtil::postToModelSpace(pLine);
	}

	AcGePoint3d startPoint(const AcDbLine* pLine)
	{
		return pLine->startPoint();
	}

	AcGePoint3d endPoint(const AcDbLine* pLine)
	{
		return pLine->endPoint();
	}

	void SetAxonometry45StartPointEndPoint(AcDbLine* pLine)
	{
		pLine->setStartPoint(CGetPointUtil::GetAxonometry45Destination(pLine->startPoint()));
		pLine->setEndPoint(CGetPointUtil::GetAxonometry45Destination(pLine->endPoint()));
	}

	double getLenth(const AcDbpLineArray* pLineArr, const TCHAR* layerName)
	{
		bool bifFilterlayer = false;
		AcDbObjectId layerid;
		if (NULL != layerName)
		{
			AcDbLayerTable* layertable = NULL;
			acdbHostApplicationServices()->workingDatabase()->getSymbolTable(layertable, AcDb::kForRead);
			if (!layertable->has(layerName))
			{
				layertable->close();
				//acutPrintf(_T("?????????¦Ä????????: %s ?????"), layerName);
				return 0;
			}
			layertable->getAt(layerName, layerid);
			layertable->close();
			bifFilterlayer = true;
		}

		int len = pLineArr->length();
		double lineLen = 0;
		for (int i = 0; i < len; i++)
		{
			if (bifFilterlayer)
			{
				if(layerid == pLineArr->at(i)->layerId()) lineLen += getLenth(pLineArr->at(i));
			}
			else lineLen += getLenth(pLineArr->at(i));			
		}

		return lineLen;
	}

	double getLenth(const AcDbObjectIdArray* pLineIds, const TCHAR* layerName)
	{
		bool bifFilterlayer = false;
		AcDbObjectId layerid;
		if (NULL != layerName)
		{
			AcDbLayerTable* layertable = NULL;
			acdbHostApplicationServices()->workingDatabase()->getSymbolTable(layertable, AcDb::kForRead);
			if (!layertable->has(layerName))
			{
				layertable->close();
				//acutPrintf(_T("?????????¦Ä????????: %s ?????"), layerName);
				return 0;
			}
			layertable->getAt(layerName, layerid);
			layertable->close();
			bifFilterlayer = true;
		}

		int len = pLineIds->length();
		double lineLen = 0;
		AcDbEntity* pEnt = NULL;
		for (int i = 0; i < len; i++)
		{
			acdbOpenObject(pEnt, pLineIds->at(i), AcDb::kForRead);
			if (bifFilterlayer)
			{	
				if (layerid == pEnt->layerId())
				{
					lineLen += CLineUtil::getLenth(dynamic_cast<AcDbLine*>(pEnt));
				}
			}
			else
			{
				lineLen += CLineUtil::getLenth(dynamic_cast<AcDbLine*>(pEnt));
			}	
			pEnt->close();
		}

		return lineLen;
	}
}
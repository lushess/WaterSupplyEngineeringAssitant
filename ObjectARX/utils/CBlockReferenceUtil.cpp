#include "stdafx.h"
#include "CBlockReferenceUtil.h"
#include <atlstr.h>

namespace CBlockReferenceUtil
{
	AcDbBlockTableRecord* newBlockDefine(const ACHAR* pName)
	{
		AcDbBlockTable* pBlockTable = NULL;
		acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForWrite);

		AcDbObjectId blockDefineId;
		AcDbBlockTableRecord* pBlockTableRecord = new AcDbBlockTableRecord();
		pBlockTableRecord->setName(pName);
		pBlockTable->add(blockDefineId, pBlockTableRecord);
		pBlockTableRecord->close();

		return pBlockTableRecord;
	}

	AcDbBlockTableRecord* newBlockDefine(AcDbBlockReference* pBlkRef,const ACHAR* pName)
	{
		AcDbBlockTableRecord* pBlockTableRecord = new AcDbBlockTableRecord();

		if (!pName)
		{
			pBlockTableRecord->setName(pName);
		}
		else
		{

		}
		pBlockTableRecord->close();

		return pBlockTableRecord;
	}

	//void InsertBlock()
	//{
	//	TCHAR BlockName[40];
	//	if (acedGetString(Adesk::kFalse, _T("\n����ͼ�������:"), BlockName) != RTNORM)
	//	{
	//		return;
	//	}
	//	AcDbBlockTable* pBlockTable;
	//	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForWrite);
	//	CString strBlkName;
	//	strBlkName.Format(TEXT("%S"), BlockName);
	//	if (!pBlockTable->has(strBlkName))
	//	{
	//		acutPrintf(_T("\n��ǰͼ����δ����ָ�����Ƶ�ͼ��"));
	//		pBlockTable->close();
	//		return;
	//	}

	//	ads_point pt;
	//	if (acedGetPoint(NULL, _T("\n�������յĲ����"), pt) != RTNORM)
	//	{
	//		pBlockTable->close();
	//		return;
	//	}
	//	AcDbObjectId pBlockDefId;
	//	pBlockTable->getAt(strBlkName, pBlockDefId);

	//	AcGePoint3d InsertPoint = asPnt3d(pt);

	//	AcDbBlockReference* pBlockRef = new AcDbBlockReference(InsertPoint, pBlockDefId);

	//	CDatabaseUtil::postToModelSpace(pBlockRef);
	//}

	bool GetAttributeValue(AcDbBlockReference* pBlockRef, TCHAR* Tag, AcString& Value)
	{
		bool bifFind = false;
		AcDbObjectIterator* it = pBlockRef->attributeIterator();
		if (it)
		{
			for (it->start(); !it->done(); it->step())
			{
				AcDbAttribute* pAttribute = NULL;
				AcDbObjectId objectid = it->objectId();
				if (pBlockRef->openAttribute(pAttribute, objectid, AcDb::kForRead) == Acad::eOk)
				{
					TCHAR* szTag = pAttribute->tag();
					CString TagName = Tag;
					TagName.MakeUpper();				//�����������Tag��Զ�����Ǵ�д��ĸ����˴���Ĳ�����Ҫ����ת��
					if (TagName.Compare(szTag) == 0)
					{
						TCHAR* szValue = pAttribute->textString();
						Value = szValue;
						bifFind = true;
						acutDelString(szValue);
						acutDelString(szTag);
						pAttribute->close();
						break;
					}
					acutDelString(szTag);
					pAttribute->close();
				}
			}
			delete it;
			it = NULL;
			return bifFind;
		}
		else
		{
			return false;
		}
	}

	void setAxonometry45Destination(AcDbBlockReference* pBlkRfr)
	{
		AcGePoint3d centerPt = getCenter(pBlkRfr);

		AcGePoint3d dstCenterPt = CGetPointUtil::GetAxonometry45Destination(centerPt);

		AcGeVector3d srcVec = centerPt.asVector();
		AcGeVector3d dstVec = dstCenterPt.asVector();
		double vecAngle = srcVec.angleTo(srcVec,dstVec);

		if (srcVec.angleTo(AcGeVector3d::kXAxis) < dstVec.angleTo(AcGeVector3d::kXAxis))
		{
			pBlkRfr->setRotation(pBlkRfr->rotation() + vecAngle);
		}
		else
		{
			pBlkRfr->setRotation(pBlkRfr->rotation() - vecAngle);
		}

		//�����λ�ƺ������Ϊ����㵱ǰ�����������ĵ��λ������(�����ĵ��λ�����������λ��)
		AcGeVector3d transVec = pBlkRfr->position().asVector() + (dstVec - srcVec);

		AcGeMatrix3d matrix = pBlkRfr->blockTransform();
		//����λ�ƾ���
		matrix = matrix.setTranslation(transVec);
		pBlkRfr->setBlockTransform(matrix);
		//pBlkRfr->setPosition(AcGePoint3d(transVec.x, transVec.y, transVec.z));

		/*
		AcGePoint3d srcPoint = pBlkRfr->position();
		AcGePoint3d dstPoint = CGetPointUtil::GetAxonometry45Destination(srcPoint);
		AcGeVector3d srcVec(srcPoint.x, srcPoint.y, 0);
		AcGeVector3d dstVec(dstPoint.x, dstPoint.y, 0);

		double vecAngle = srcVec.angleTo(dstVec);
		if (srcVec.angleTo(AcGeVector3d::kXAxis)< dstVec.angleTo(AcGeVector3d::kXAxis))
		{
			pBlkRfr->setRotation(pBlkRfr->rotation() + vecAngle);
		}
		else
		{
			pBlkRfr->setRotation(pBlkRfr->rotation() - vecAngle);
		}
	
		pBlkRfr->setPosition(CGetPointUtil::GetAxonometry45Destination(pBlkRfr->position()));
		*/
	
		/*
		AcGeMatrix3d matrix = pBlkRfr->blockTransform();
		AcGeVector3d srcVec = matrix.translation();
		AcGePoint3d srcPoint = pBlkRfr->position();
		AcGePoint3d dstPoint = CGetPointUtil::GetAxonometry45Destination(srcPoint);
		AcGeVector3d dstVec(dstPoint.x - srcPoint.x, dstPoint.y - srcPoint.y, dstPoint.z - srcPoint.z);

		double vecAngle = srcVec.angleTo(srcVec, dstVec);
	
		matrix = matrix.setTranslation(srcVec+dstVec);
		pBlkRfr->setBlockTransform(matrix);
		*/
	}

	/*
	void explodeBlockReference(AcDbObjectId blkRefId, AcDbBlockTableRecord* pTargetBlock)
	{
		AcDbBlockReference* pBlkRef = NULL;

		acdbOpenObject(pBlkRef, blkRefId, AcDb::kForRead, true);

		if (pBlkRef)
		{
			AcDbObjectId blkTblRecId = pBlkRef->blockTableRecord();
			AcDbBlockTableRecord* pBlkTblRec = NULL;

			acdbOpenObject(pBlkTblRec, blkTblRecId, AcDb::kForRead);

			if (pBlkTblRec)
			{
				// ???��??????�??????????  
				if (!pTargetBlock)
				{
					AcDbBlockTable* pBlockTable;
					acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);
					pBlockTable->getAt(ACDB_MODEL_SPACE, pTargetBlock, AcDb::kForRead);
					pBlockTable->close();
				}

				AcDbBlockTableRecordIterator* it = NULL;
				pBlkTblRec->newIterator(it);
				for (it->start(); !it->done(); it->step())
				{
					AcDbEntity* pEnt = NULL;
					it->getEntity(pEnt);
					if (pEnt)
					{
						AcDbEntity* pNewEnt = dynamic_cast<AcDbEntity*>(pEnt->clone());
						// ????????��?????? 
						if (pNewEnt)
						{
							// ??????????????  
							pTargetBlock->appendAcDbEntity(pNewEnt);
							pNewEnt->close();
						}
						// ????????????????  
						AcDbBlockReference* pNestedBlkRef = AcDbBlockReference::cast(pEnt);

						if (pNestedBlkRef)
						{
							// ??????????????  
							explodeBlockReference(pNestedBlkRef->objectId(), pTargetBlock);
						}
					}
					//?????????
					pEnt->erase();
					pEnt->close();
				}

				if(it) delete it;
				pBlkTblRec->close();  			
			}

			pBlkRef->close();
		}
	}
	*/

	void explodeBlockReference(AcDbVoidPtrArray*& entitySet, AcDbBlockReference* pBlockRef)
	{
		int preLen = entitySet->length();
		pBlockRef->explode(*entitySet); //explode��������ʵ�������������
		int nextLen = entitySet->length();
		int curLen = nextLen - preLen; //�õ�����explode��������ʵ������
	
		for (int i = 0 ; i < curLen; i++)
		{
			if (CEntityUtil::isBlockReference == CEntityUtil::getEntityType(static_cast<AcDbEntity*>(entitySet->at(preLen + i))))
			{	
				explodeBlockReference(entitySet, dynamic_cast<AcDbBlockReference*>(static_cast<AcDbEntity*>(entitySet->at(preLen + i))));
				//�ͷſ�����ڴ�
				delete entitySet->at(preLen + i);
				entitySet->at(preLen + i) = NULL;
				entitySet->removeAt(preLen + i); //��������ɾ������գ�ʹ���յõ�������û�п����
			}
		}
	}

	void explodeBlockReference(AcDbObjectIdArray*& ids, AcDbBlockReference* pBlockRef)
	{
		AcDbVoidPtrArray* entitySet = new AcDbVoidPtrArray;

		explodeBlockReference(entitySet,pBlockRef);
		CDatabaseUtil::postToModelSpace(ids,entitySet);

		pBlockRef->erase();
		delete entitySet;
		entitySet = NULL;
	}

	AcGePoint3d getCenter(AcDbBlockReference* pBlkRfr)
	{
		AcDbVoidPtrArray* entitySet = new AcDbVoidPtrArray;

		explodeBlockReference(entitySet, pBlkRfr);

		AcGePoint3dArray pt3DArr;
		for (int i = 0 ; i < entitySet->length() ; i++)
		{
			pt3DArr.append(CEntityUtil::getEntityCeterPoint(static_cast<AcDbEntity*>(entitySet->at(i))));
		}
		delete entitySet;
		entitySet = NULL;

		return CGetPointUtil::getCenterPointFromArray(pt3DArr);
	}


	int getIdsOFBlkDefine(const TCHAR* tabRecName, AcDbObjectIdArray* ids)
	{
		Acad::ErrorStatus es = Acad::eNullBlockName;
		//?????????????????
		AcDbBlockTable* pBlockTable;
		es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);
		if (Acad::eOk != es) return es;

		//?????????????????
		AcDbBlockTableRecord* pBlockTableRecord;
		es = pBlockTable->getAt(tabRecName, pBlockTableRecord, AcDb::kForRead);
		if (Acad::eOk != es) return es;

		if (NULL == ids)
		{
			AcDbObjectIdArray* ids = new AcDbObjectIdArray;
			pBlockTableRecord->getBlockReferenceIds(*ids);
			int len = ids->length();
			delete ids;
			ids = NULL;
			return len;
		}
		else
		{
			pBlockTableRecord->getBlockReferenceIds(*ids);
			return ids->length();
		}

		return 0;
	}

	Acad::ErrorStatus getIdsOFBlkDefine(const TCHAR* tabRecName, AcDbObjectIdArray*& ids, const TCHAR* layerName)
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
				//acutPrintf(_T("?????????��????????: %s ?????"), layerName);
				return Acad::eInvalidLayer;
			}
			layertable->getAt(layerName, layerid);
			layertable->close();
			bifFilterlayer = true;
		}

		Acad::ErrorStatus es = Acad::eNullBlockName;
		//?????????????????
		AcDbBlockTable* pBlockTable;
		es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);
		if (Acad::eOk != es) return es;

		//?????????????????
		AcDbBlockTableRecord* pBlockTableRecord;
		es = pBlockTable->getAt(tabRecName, pBlockTableRecord, AcDb::kForRead);
		if (Acad::eOk != es) return es;

		if (bifFilterlayer)
		{
			pBlockTableRecord->getBlockReferenceIds(*ids);
			for (int i = 0; i < ids->length(); i++)
			{
				AcDbEntity* pEnt = NULL;
				acdbOpenObject(pEnt, ids->at(i), AcDb::kForRead);
				if (layerid != pEnt->layerId())
				{
					ids->removeAt(i);
				}
				pEnt->close();
			}
		}
		else
		{
			pBlockTableRecord->getBlockReferenceIds(*ids);
		}

		return es;
	}

	Acad::ErrorStatus getAllBlkDefineName(AcTCHARArray* names)
	{
		Acad::ErrorStatus es = Acad::eOk;
		AcDbBlockTable* pBlockTable = NULL;
		es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);
		if (Acad::eOk != es) return es;

		AcDbBlockTableIterator* it = NULL;
		pBlockTable->newIterator(it);
		for (it->start(); !it->done(); it->step())
		{
			TCHAR* name = NULL;
			AcDbBlockTableRecord* pBlockTableRecord = NULL;

			it->getRecord(pBlockTableRecord, AcDb::kForRead);
			es = pBlockTableRecord->getName(name);
			names->append(name);
		}
		delete it;
		it = NULL;
		
		return es;
	}

}
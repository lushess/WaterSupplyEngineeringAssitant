#include "stdafx.h"
#include "CDatabaseUtil.h"

namespace CDatabaseUtil
{

	AcDbObjectId postToModelSpace(AcDbEntity* pEntity)
	{
		Acad::ErrorStatus es;
		assert(pEntity != NULL);

		AcDbBlockTable* pBlockTable;
		es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);

		AcDbBlockTableRecord* pBlockTableRecord;
		es = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);


		AcDbObjectId ObjectID;
		es = pBlockTableRecord->appendAcDbEntity(ObjectID, pEntity);
		if (es != Acad::eOk)
		{
			pBlockTable->close();
			pBlockTableRecord->close();
			delete pEntity;
			pEntity = NULL;
			return AcDbObjectId::kNull;
		}

		pBlockTable->close();
		pBlockTableRecord->close();
		pEntity->close();

		return ObjectID;
	}

	void postToModelSpace(AcDbObjectIdArray*& ids,const AcDbVoidPtrArray* entitySet)
	{
		Acad::ErrorStatus es;
		assert(pEntity != NULL);

		AcDbBlockTable* pBlockTable;
		es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);

		AcDbBlockTableRecord* pBlockTableRecord;
		es = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);


		for (int i = 0; i < entitySet->length(); i++)
		{
			AcDbObjectId ObjectID;
			es = pBlockTableRecord->appendAcDbEntity(ObjectID, static_cast<AcDbEntity*>(entitySet->at(i)));
			static_cast<AcDbEntity*>(entitySet->at(i))->close();
			ids->append(ObjectID);
		}
		
		pBlockTable->close();
		pBlockTableRecord->close();
	}

	Acad::ErrorStatus saveEntityChangement(const AcDbObjectId id, ChangeCommand cmd)
	{
		Acad::ErrorStatus es;

		AcDbBlockTable* pBlockTable = NULL;
		AcDbBlockTableRecord* pBlockTableRecord = NULL;
		es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);

		if (Acad::eOk == es) es = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);

		if (Acad::eOk == es) es = cmd(id);

		pBlockTable->close();
		pBlockTableRecord->close();

		return es;
	}

	Acad::ErrorStatus saveEntityChangement(const AcDbObjectIdArray*& ids, ChangeCommand cmd)
	{
		Acad::ErrorStatus es;

		AcDbBlockTable* pBlockTable = NULL;
		AcDbBlockTableRecord* pBlockTableRecord = NULL;
		es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);

		if (Acad::eOk == es) es = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);

		if (Acad::eOk == es) 
			for (int i = 0; i<ids->length(); i++)
			{
				es = cmd(ids->at(i));
			}

		pBlockTable->close();
		pBlockTableRecord->close();

		return es;
	}


	AcDbObjectIdArray getAllEntityIDs(const TCHAR* layerName)
	{
		AcDbObjectIdArray EntityIDs;
		bool bifFilterlayer = false;
		AcDbObjectId layerid;
		if (layerName != NULL)
		{
			AcDbLayerTable* layertable = NULL;
			acdbHostApplicationServices()->workingDatabase()->getSymbolTable(layertable, AcDb::kForRead);
			if (!layertable->has(layerName))
			{
				layertable->close();
				acutPrintf((L"在当前模型空间未找到名称为: %s 的图层"), layerName);
				return EntityIDs;
			}
			layertable->getAt(layerName, layerid);
			layertable->close();
			bifFilterlayer = true;
		}

		AcDbBlockTable* pBlockTable;
		acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);
		AcDbBlockTableRecord* PBlockTableRecoard;
		pBlockTable->getAt(ACDB_MODEL_SPACE, PBlockTableRecoard, AcDb::kForRead);
		pBlockTable->close();

		AcDbBlockTableRecordIterator* it = NULL;
		PBlockTableRecoard->newIterator(it);
		for (it->start(); !it->done(); it->step())
		{
			AcDbEntity* pEntity = NULL;
			Acad::ErrorStatus es = it->getEntity(pEntity, AcDb::kForRead);
			if (Acad::eOk == es)
			{
				if (bifFilterlayer)					//??????????????????????????????????
				{
					if (pEntity->layerId() == layerid)
					{
						EntityIDs.append(pEntity->objectId());
					}
				}
				else								//????????е??????id??
				{
					EntityIDs.append(pEntity->objectId());
				}
				pEntity->close();
			}
			else
			{
				//acutPrintf(TEXT("\nCDwgDatebaseUtil::GetAllEntityIDs?д?????????????????:%d??"), (int)es);
			}
		}
		delete it;
		it = NULL;
		PBlockTableRecoard->close();
		return EntityIDs;
	}

	Acad::ErrorStatus getAllLayerName(AcTCHARArray* layerNames,bool skipHidden,bool skipReconciled)
	{
		Acad::ErrorStatus es= Acad::eOk;
		AcDbLayerTable* layertable = NULL;
		es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(layertable, AcDb::kForRead);

		if (Acad::eOk != es) return es;
		AcDbLayerTableIterator* it = NULL;
		layertable->newIterator(it);
		it->setSkipHidden(skipHidden);
		it->setSkipReconciled(skipReconciled);
		for (it->start(); !it->done(); it->step())
		{
			AcDbLayerTableRecord* layerTabRec = NULL;
			TCHAR* name = NULL;
			es = it->getRecord(layerTabRec, AcDb::kForRead);
			layerTabRec->getName(name);
			layerNames->append(name);
		}
		delete it;
		it = NULL;

		layertable->close();

		return es;
	}

	Acad::ErrorStatus getSelectedIDs(AcDbObjectIdArray*& selectedIdsArray)
	{
		ads_name ssName;
		Acad::ErrorStatus es = Acad::eNullObjectId;
		if (acedSSGet(NULL, NULL, NULL, NULL, ssName) == RTNORM)
		{
			Adesk::Int32 i, length = 0;
			acedSSLength(ssName, &length);
			for (i = 0; i < length; i++) {
				ads_name entName;
				acedSSName(ssName, i, entName);
				AcDbObjectId entId;
				es = acdbGetObjectId(entId, entName);
				selectedIdsArray->append(entId);
			}

			acedSSFree(ssName);
		}
		return es;
	}

	Acad::ErrorStatus getAllEntityIdsByType(AcDbObjectIdArray*& pIds,enum CEntityUtil::EntityType entType , const TCHAR* layerName)
	{
		Acad::ErrorStatus es = Acad::eWrongObjectType;

		bool bifFilterlayer = false;
		AcDbObjectId layerid;
		if (layerName != NULL)
		{
			AcDbLayerTable* layertable = NULL;
			acdbHostApplicationServices()->workingDatabase()->getSymbolTable(layertable, AcDb::kForRead);
			if (!layertable->has(layerName))
			{
				layertable->close();
				//acutPrintf((L"在当前模型空间未找到名称为：%s 的图层"),layerName);
				return Acad::eInvalidLayer;
			}
			
			layertable->getAt(layerName, layerid);
			layertable->close();
			bifFilterlayer = true;
		}

		AcDbBlockTable* pBlockTable;
		acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);
		AcDbBlockTableRecord* PBlockTableRecoard;
		pBlockTable->getAt(ACDB_MODEL_SPACE, PBlockTableRecoard, AcDb::kForRead);
		pBlockTable->close();

		AcDbBlockTableRecordIterator* it = NULL;
		PBlockTableRecoard->newIterator(it);
		for (it->start(); !it->done(); it->step())
		{
			AcDbEntity* pEntity;
			es = it->getEntity(pEntity);

			if (Acad::eOk == es)
			{
				if (bifFilterlayer)
				{
					if (layerid == pEntity->layerId())
					{
						if (entType == CEntityUtil::getEntityType(pEntity))
						{
							pIds->append(pEntity->objectId());
						}
					}
				}
				else
				{
					if (entType == CEntityUtil::getEntityType(pEntity))
					{
						pIds->append(pEntity->objectId());
					}
				}
			}
			else return es;

			pEntity->close();
		}
		delete it;
		it = NULL;
		PBlockTableRecoard->close();

		return es;
	}


}
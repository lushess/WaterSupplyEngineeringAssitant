#pragma once
#include "StdAfx.h"
#include "CEntityUtil.h"
typedef AcArray<TCHAR*>AcTCHARArray;

namespace CDatabaseUtil
{
	typedef Acad::ErrorStatus(*ChangeCommand)(AcDbObjectId);
	typedef Acad::ErrorStatus(*EntityMethod)(void);
	Acad::ErrorStatus getSelectedIDs(AcDbObjectIdArray* &selectedIdsArray);
	Acad::ErrorStatus getAllEntityIdsByType(AcDbObjectIdArray*& pIds,enum CEntityUtil::EntityType entType, const TCHAR* layerName = NULL);

	//skipHidden为跳过隐藏图层,skipReconciled为跳过参照图层
	Acad::ErrorStatus getAllLayerName(AcTCHARArray* layerNames,bool skipHidden = true,bool skipReconciled = true);

	//将模型对象添加到图形数据库
	AcDbObjectId postToModelSpace(AcDbEntity* pEntity);

	void postToModelSpace(AcDbObjectIdArray*& ids,const AcDbVoidPtrArray* entitySet);

	Acad::ErrorStatus saveEntityChangement(const AcDbObjectId id,ChangeCommand cmd);

	Acad::ErrorStatus saveEntityChangement(const AcDbObjectIdArray*& ids, ChangeCommand cmd);

	//遍历当前图形的模型空间，获取位于某个图层上的所有实体，若图层名为空，则遍历当前模型空间的所有实体，返回ID集
	AcDbObjectIdArray getAllEntityIDs(const TCHAR* layerName = NULL);

	
}
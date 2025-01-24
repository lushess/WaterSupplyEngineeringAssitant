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

	//skipHiddenΪ��������ͼ��,skipReconciledΪ��������ͼ��
	Acad::ErrorStatus getAllLayerName(AcTCHARArray* layerNames,bool skipHidden = true,bool skipReconciled = true);

	//��ģ�Ͷ�����ӵ�ͼ�����ݿ�
	AcDbObjectId postToModelSpace(AcDbEntity* pEntity);

	void postToModelSpace(AcDbObjectIdArray*& ids,const AcDbVoidPtrArray* entitySet);

	Acad::ErrorStatus saveEntityChangement(const AcDbObjectId id,ChangeCommand cmd);

	Acad::ErrorStatus saveEntityChangement(const AcDbObjectIdArray*& ids, ChangeCommand cmd);

	//������ǰͼ�ε�ģ�Ϳռ䣬��ȡλ��ĳ��ͼ���ϵ�����ʵ�壬��ͼ����Ϊ�գ��������ǰģ�Ϳռ������ʵ�壬����ID��
	AcDbObjectIdArray getAllEntityIDs(const TCHAR* layerName = NULL);

	
}
#pragma once
#include "StdAfx.h"
typedef AcArray<TCHAR*>AcTCHARArray;
#include "CGetPointUtil.h"
#include "CDatabaseUtil.h"
namespace CBlockReferenceUtil
{	

	AcDbBlockTableRecord* newBlockDefine(const ACHAR * pName);
	AcDbBlockTableRecord* newBlockDefine(AcDbBlockReference* pBlkRef,const ACHAR* pName = NULL); //TODO
	////����һ�������ԵĿ鶨�壬���ؿ�����
	//void addMyBlockDefine();

	////�����û�����Ŀ�����λ�õ��������
	//void InsertBlock();

	//��ͼ������ȡ�ض����Ե�ֵ
	bool GetAttributeValue(AcDbBlockReference* pBlockRef, TCHAR* Tag, AcString& Value);

	//void explodeBlockReference(AcDbObjectId blkRefId, AcDbBlockTableRecord* pTargetBlock = NULL);

	//entitySetΪʵ��ָ������,����ʵ�岢δ����ӵ�Database��
	void explodeBlockReference(AcDbVoidPtrArray*& entitySet, AcDbBlockReference* pBlockRef);

	//�ֽⲢ�������Ԫ�ص�ģ�Ϳռ�,ɾ��ԭ��,����ids����
	void explodeBlockReference(AcDbObjectIdArray*& ids, AcDbBlockReference* pBlockRef);

	void setAxonometry45Destination(AcDbBlockReference* pBlkRfr);

	AcGePoint3d getCenter(AcDbBlockReference* pBlkRfr);

	//����Ids���鳤��
	int getIdsOFBlkDefine(const TCHAR* tabRecName, AcDbObjectIdArray* ids = NULL);

	//Ч�ʲ�
	Acad::ErrorStatus getIdsOFBlkDefine(const TCHAR* tabRecName,AcDbObjectIdArray*& ids,const TCHAR* layerName = NULL);

	Acad::ErrorStatus getAllBlkDefineName(AcTCHARArray* names);

}

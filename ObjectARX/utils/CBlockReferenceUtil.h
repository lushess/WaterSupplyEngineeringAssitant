#pragma once
#include "StdAfx.h"
typedef AcArray<TCHAR*>AcTCHARArray;
#include "CGetPointUtil.h"
#include "CDatabaseUtil.h"
namespace CBlockReferenceUtil
{	

	AcDbBlockTableRecord* newBlockDefine(const ACHAR * pName);
	AcDbBlockTableRecord* newBlockDefine(AcDbBlockReference* pBlkRef,const ACHAR* pName = NULL); //TODO
	////创建一个带属性的块定义，返回块名称
	//void addMyBlockDefine();

	////根据用户输入的块名和位置点插入块参照
	//void InsertBlock();

	//从图块中提取特定属性的值
	bool GetAttributeValue(AcDbBlockReference* pBlockRef, TCHAR* Tag, AcString& Value);

	//void explodeBlockReference(AcDbObjectId blkRefId, AcDbBlockTableRecord* pTargetBlock = NULL);

	//entitySet为实体指针数组,其内实体并未被添加到Database中
	void explodeBlockReference(AcDbVoidPtrArray*& entitySet, AcDbBlockReference* pBlockRef);

	//分解并添加其内元素到模型空间,删除原块,返回ids数组
	void explodeBlockReference(AcDbObjectIdArray*& ids, AcDbBlockReference* pBlockRef);

	void setAxonometry45Destination(AcDbBlockReference* pBlkRfr);

	AcGePoint3d getCenter(AcDbBlockReference* pBlkRfr);

	//返回Ids数组长度
	int getIdsOFBlkDefine(const TCHAR* tabRecName, AcDbObjectIdArray* ids = NULL);

	//效率差
	Acad::ErrorStatus getIdsOFBlkDefine(const TCHAR* tabRecName,AcDbObjectIdArray*& ids,const TCHAR* layerName = NULL);

	Acad::ErrorStatus getAllBlkDefineName(AcTCHARArray* names);

}

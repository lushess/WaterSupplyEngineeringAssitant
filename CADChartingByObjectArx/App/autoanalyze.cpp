#include "stdafx.h"
#include <Windows.h>
#include "sqlite3.h"
#include <dbents.h>
#include <adslib.h>
#include <dbapserv.h>
#include <map>
#include "aced.h"

#pragma comment(lib, "sqlite3.lib")

// ���ݿ��ļ�·�������޸ģ�
//#define DB_PATH "WaterSupplyEngineeringAutoAnalysis.db"
const char* DB_PATH = "WaterSupplyEngineeringAutoAnalysis.db";

// ��λӳ���
struct EntityUnit {
    const char* entityName;
    const char* unit;
};

static EntityUnit UNIT_MAP[] = {
    {"Line",          "��"},
    {"Pline",         "��"},
    {"BlockReference", "��"},
    {nullptr,         nullptr}
};

// ͳ�����ݽṹ
struct EntityStats {
    ACHAR layerName[256];
    int count = 0;
};

// ����ʵ�ֺ���
void createAndAnalyzeDatabase() {
    sqlite3* db = nullptr;
    char* errMsg = nullptr;
    int rc;

    // ����/�����ݿ�
    rc = sqlite3_open(DB_PATH, &db);
    if (rc != SQLITE_OK) {
        acutPrintf(L"\n�޷��������ݿ�: %s", sqlite3_errmsg(db));
        return;
    }

    // ����STATS��
    const char* createTableSQL =
        "CREATE TABLE IF NOT EXISTS STATS ("
        "    \"ORDER\" INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    ENTITYNAME TEXT NOT NULL,"
        "    BYLAYER TEXT NOT NULL,"
        "    NUMBER INTEGER NOT NULL,"
        "    UTIL TEXT NOT NULL"
        ");";

    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        acutPrintf(L"\n������ʧ��: %s", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return;
    }

    // ׼���������
    sqlite3_stmt* stmt;
    const char* insertSQL = "INSERT INTO STATS (ENTITYNAME, BYLAYER, NUMBER, UTIL) VALUES (?, ?, ?, ?)";
    rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        acutPrintf(L"\n׼���������ʧ��: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // ��ʼ����
    sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);

    // ͳ��ʵ��
    std::map<std::string, std::map<std::wstring, EntityStats>> stats;

    AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
    AcDbBlockTable* pBlockTable;
    pDb->getBlockTable(pBlockTable, AcDb::kForRead);

    AcDbBlockTableIterator* pIt;
    pBlockTable->newIterator(pIt);

    for (pIt->start(); !pIt->done(); pIt->step()) {
        AcDbBlockTableRecord* pBlock;
        pIt->getRecord(pBlock, AcDb::kForRead);

        AcDbBlockTableRecordIterator* pEntIt;
        pBlock->newIterator(pEntIt);

        for (pEntIt->start(); !pEntIt->done(); pEntIt->step()) {
            AcDbEntity* pEnt;
            pEntIt->getEntity(pEnt, AcDb::kForRead);

            // ��ȡʵ������
            const char* entityType = "Unknown";
            if (pEnt->isKindOf(AcDbLine::desc()))
                entityType = "Line";
            else if (pEnt->isKindOf(AcDbPolyline::desc()))
                entityType = "Pline";
            else if (pEnt->isKindOf(AcDbBlockReference::desc()))
                entityType = "BlockReference";

            // ��ȡͼ������
            const ACHAR* layerName = pEnt->layer();
            

            // ����ͳ��
            std::wstring wLayerName(layerName);
            stats[entityType][wLayerName].count++;

            pEnt->close();
        }
        pBlock->close();
        delete pEntIt;
    }
    delete pIt;
    pBlockTable->close();

    // д�����ݿ�
    for (auto& entityPair : stats) {
        const char* entityName = entityPair.first.c_str();
        const char* unit = "��";

        // ���ҵ�λ
        for (int i = 0; UNIT_MAP[i].entityName; i++) {
            if (strcmp(entityName, UNIT_MAP[i].entityName) == 0) {
                unit = UNIT_MAP[i].unit;
                break;
            }
        }

        for (auto& layerPair : entityPair.second) {
            std::wstring layer = layerPair.first;
            int count = layerPair.second.count;

            // �󶨲���
            sqlite3_bind_text(stmt, 1, entityName, -1, SQLITE_STATIC);
            sqlite3_bind_text16(stmt, 2, layer.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 3, count);
            sqlite3_bind_text(stmt, 4, unit, -1, SQLITE_STATIC);

            // ִ�в���
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                acutPrintf(L"\n��������ʧ��: %s", sqlite3_errmsg16(db));
            }
            sqlite3_reset(stmt);
        }
    }

    // �ύ��������
    sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    acutPrintf(L"\nͳ�Ʒ�����ɣ�����ѱ�������%s", DB_PATH);
}

// ����ע��
void createAndAnalyzeDatabaseInit(void) {
    acedRegCmds->addCommand(
        L"autoanalyze",
        L"WS_Analyze",
        L"Analyze",
        ACRX_CMD_MODAL,
        createAndAnalyzeDatabase
    );
}

void createAndAnalyzeDatabaseUnload(void)
{
    acedRegCmds->removeGroup(L"autoanalyze");
}

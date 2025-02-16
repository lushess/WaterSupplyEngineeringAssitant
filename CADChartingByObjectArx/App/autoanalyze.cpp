#include "stdafx.h"
#include <Windows.h>
#include "sqlite3.h"
#include <dbents.h>
#include <adslib.h>
#include <dbapserv.h>
#include <map>
#include "aced.h"

#pragma comment(lib, "sqlite3.lib")

// 数据库文件路径（可修改）
//#define DB_PATH "WaterSupplyEngineeringAutoAnalysis.db"
const char* DB_PATH = "WaterSupplyEngineeringAutoAnalysis.db";

// 单位映射表
struct EntityUnit {
    const char* entityName;
    const char* unit;
};

static EntityUnit UNIT_MAP[] = {
    {"Line",          "米"},
    {"Pline",         "米"},
    {"BlockReference", "个"},
    {nullptr,         nullptr}
};

// 统计数据结构
struct EntityStats {
    ACHAR layerName[256];
    int count = 0;
};

// 命令实现函数
void createAndAnalyzeDatabase() {
    sqlite3* db = nullptr;
    char* errMsg = nullptr;
    int rc;

    // 创建/打开数据库
    rc = sqlite3_open(DB_PATH, &db);
    if (rc != SQLITE_OK) {
        acutPrintf(L"\n无法创建数据库: %s", sqlite3_errmsg(db));
        return;
    }

    // 创建STATS表
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
        acutPrintf(L"\n创建表失败: %s", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return;
    }

    // 准备插入语句
    sqlite3_stmt* stmt;
    const char* insertSQL = "INSERT INTO STATS (ENTITYNAME, BYLAYER, NUMBER, UTIL) VALUES (?, ?, ?, ?)";
    rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        acutPrintf(L"\n准备插入语句失败: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // 开始事务
    sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);

    // 统计实体
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

            // 获取实体类型
            const char* entityType = "Unknown";
            if (pEnt->isKindOf(AcDbLine::desc()))
                entityType = "Line";
            else if (pEnt->isKindOf(AcDbPolyline::desc()))
                entityType = "Pline";
            else if (pEnt->isKindOf(AcDbBlockReference::desc()))
                entityType = "BlockReference";

            // 获取图层名称
            const ACHAR* layerName = pEnt->layer();
            

            // 更新统计
            std::wstring wLayerName(layerName);
            stats[entityType][wLayerName].count++;

            pEnt->close();
        }
        pBlock->close();
        delete pEntIt;
    }
    delete pIt;
    pBlockTable->close();

    // 写入数据库
    for (auto& entityPair : stats) {
        const char* entityName = entityPair.first.c_str();
        const char* unit = "无";

        // 查找单位
        for (int i = 0; UNIT_MAP[i].entityName; i++) {
            if (strcmp(entityName, UNIT_MAP[i].entityName) == 0) {
                unit = UNIT_MAP[i].unit;
                break;
            }
        }

        for (auto& layerPair : entityPair.second) {
            std::wstring layer = layerPair.first;
            int count = layerPair.second.count;

            // 绑定参数
            sqlite3_bind_text(stmt, 1, entityName, -1, SQLITE_STATIC);
            sqlite3_bind_text16(stmt, 2, layer.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 3, count);
            sqlite3_bind_text(stmt, 4, unit, -1, SQLITE_STATIC);

            // 执行插入
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                acutPrintf(L"\n插入数据失败: %s", sqlite3_errmsg16(db));
            }
            sqlite3_reset(stmt);
        }
    }

    // 提交事务并清理
    sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    acutPrintf(L"\n统计分析完成！结果已保存至：%s", DB_PATH);
}

// 命令注册
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

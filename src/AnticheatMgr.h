/*
 *MIT License
 *
 *Copyright (c) 2022 Azerothcore
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *SOFTWARE.
 */

#ifndef SC_ACMGR_H
#define SC_ACMGR_H

#include "Common.h"
#include "SharedDefines.h"
#include "ScriptMgr.h"
#include "AnticheatData.h"
#include "Chat.h"
#include "ObjectGuid.h"

class Player;
class AnticheatData;

enum ReportTypes
{
    SPEED_HACK_REPORT = 0,
    FLY_HACK_REPORT = 1,
    WALK_WATER_HACK_REPORT = 2,
    JUMP_HACK_REPORT = 3,
    TELEPORT_PLANE_HACK_REPORT = 4,
    CLIMB_HACK_REPORT = 5,
    TELEPORT_HACK_REPORT = 6,
    IGNORE_CONTROL_REPORT = 7,
    ZAXIS_HACK_REPORT = 8,
    ANTISWIM_HACK_REPORT = 9,
    GRAVITY_HACK_REPORT = 10
   // MAX_REPORT_TYPES
};

// GUID is the key.
typedef std::map<ObjectGuid, AnticheatData> AnticheatPlayersDataMap;

class AnticheatMgr
{
    AnticheatMgr();
    ~AnticheatMgr();

    public:
    static AnticheatMgr* instance()
        {
           static AnticheatMgr* instance = new AnticheatMgr();
           return instance;
        }

        void StartHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
        void SavePlayerData(Player* player);
        void SavePlayerDataDaily(Player* player);
        void HandlePlayerLogin(Player* player);
        void HandlePlayerLogout(Player* player);

        uint32 GetTotalReports(ObjectGuid guid);
        float GetAverage(ObjectGuid guid);
        uint32 GetTypeReports(ObjectGuid guid, uint8 type);

        void AnticheatGlobalCommand(ChatHandler* handler);
        void AnticheatDeleteCommand(ObjectGuid guid);
        void AnticheatPurgeCommand(ChatHandler* handler);
        void ResetDailyReportStates();
    private:
        void SpeedHackDetection(Player* player, MovementInfo movementInfo);
        void FlyHackDetection(Player* player, MovementInfo movementInfo);
        void WalkOnWaterHackDetection(Player* player, MovementInfo movementInfo);
        void JumpHackDetection(Player* player, MovementInfo movementInfo,uint32 opcode);
        void TeleportPlaneHackDetection(Player* player, MovementInfo, uint32 opcode);
        void ClimbHackDetection(Player* player,MovementInfo movementInfo, uint32 opcode);
        void AntiSwimHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
        void TeleportHackDetection(Player* player, MovementInfo movementInfo);
        void IgnoreControlHackDetection(Player* player, MovementInfo movementInfo, uint32 opcode);
        void ZAxisHackDetection(Player* player, MovementInfo movementInfo);
        void GravityHackDetection(Player* player, MovementInfo movementInfo);
        void BuildReport(Player* player,uint16 reportType);

        bool MustCheckTempReports(uint8 type);
        uint32 _counter = 0;
        uint32 _alertFrequency;
        AnticheatPlayersDataMap m_Players;                        ///< Player data
};

#define sAnticheatMgr AnticheatMgr::instance()

#endif

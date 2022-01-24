/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Config.h"
#include "AnticheatMgr.h"
#include "Object.h"
#include "AccountMgr.h"
#include "Chat.h"
#include "Player.h"
#include "Timer.h"
#include "GameTime.h"

Seconds resetTime = 0s;
Seconds lastIterationPlayer = GameTime::GetUptime() + 30s; //TODO: change 30 secs static to a configurable option

class AnticheatPlayerScript : public PlayerScript
{
public:
	AnticheatPlayerScript() : PlayerScript("AnticheatPlayerScript") { }

	void OnLogout(Player* player) override
	{
		sAnticheatMgr->HandlePlayerLogout(player);
	}

	void OnLogin(Player* player) override
	{
		sAnticheatMgr->HandlePlayerLogin(player);

        if (sConfigMgr->GetOption<bool>("Anticheat.LoginMessage", true))
			ChatHandler(player->GetSession()).PSendSysMessage("This server is running an Anticheat Module.");
	}
};

class AnticheatWorldScript : public WorldScript
{
public:
	AnticheatWorldScript() : WorldScript("AnticheatWorldScript") { }

    void OnUpdate(uint32 /* diff */) override // unusued parameter
	{
		if (GameTime::GetGameTime() > resetTime)
		{
			LOG_INFO("module", "Anticheat: Resetting daily report states.");
			sAnticheatMgr->ResetDailyReportStates();
			UpdateReportResetTime();
            FMT_LOG_INFO("module", "Anticheat: Next daily report reset: {}", Acore::Time::TimeToHumanReadable(resetTime));
		}

        if (GameTime::GetUptime() > lastIterationPlayer)
		{
			lastIterationPlayer = GameTime::GetUptime() + Seconds(sConfigMgr->GetOption<uint32>("Anticheat.SaveReportsTime", 60));

            FMT_LOG_INFO("module", "Saving reports for {} players.", sWorld->GetPlayerCount());

			for (SessionMap::const_iterator itr = sWorld->GetAllSessions().begin(); itr != sWorld->GetAllSessions().end(); ++itr)
				if (Player* plr = itr->second->GetPlayer())
					sAnticheatMgr->SavePlayerData(plr);
		}
	}

    void OnAfterConfigLoad(bool /* reload */) override // unusued parameter
	{
        LOG_INFO("module", "AnticheatModule Loaded.");
	}

    void UpdateReportResetTime()
	{
		resetTime = Seconds(Acore::Time::GetNextTimeWithDayAndHour(-1, 6));
	}
};

class AnticheatMovementHandlerScript : public MovementHandlerScript
{
public:
    AnticheatMovementHandlerScript() : MovementHandlerScript("AnticheatMovementHandlerScript") { }

    void OnPlayerMove(Player* player, MovementInfo mi, uint32 opcode) override
    {
		if (!AccountMgr::IsGMAccount(player->GetSession()->GetSecurity()) || sConfigMgr->GetOption<bool>("Anticheat.EnabledOnGmAccounts", false))
			sAnticheatMgr->StartHackDetection(player, mi, opcode);
    }
};

void startAnticheatScripts()
{
	new AnticheatWorldScript();
	new AnticheatPlayerScript();
	new AnticheatMovementHandlerScript();
}

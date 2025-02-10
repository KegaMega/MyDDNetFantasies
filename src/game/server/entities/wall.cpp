/* (c) Shereef Marzouk. See "licence DDRace.txt" and the readme.txt in the root of the distribution for more information. */
#include "wall.h"
#include "character.h"

#include <game/generated/protocol.h>
#include <game/mapitems.h>
#include <game/teamscore.h>

#include <game/server/gamecontext.h>
#include <game/server/player.h>

CWall::CWall(CGameWorld *pGameWorld, vec2 Pos, vec2 Pos2, CCharacter *User, 
	int Number) :
	CEntity(pGameWorld, CGameWorld::ENTTYPE_LASER)
{
	m_Player = User;
	m_Number = Number;
	m_Pos = Pos;
	m_Length = distance(Pos, Pos2);
	m_Direction = Pos2-Pos;
	vec2 To = Pos + normalize(m_Direction) * m_Length;

	GameServer()->Collision()->IntersectNoLaser(Pos, To, &this->m_To, 0);
	ResetCollision();
	GameWorld()->InsertEntity(this);
}

void CWall::ResetCollision()
{
}

void CWall::RemoveWall()
{
	GameWorld()->RemoveEntity(this);
}

void CWall::Reset()
{
	m_MarkedForDestroy = true;
}

void CWall::Snap(int SnappingClient)
{
	if(m_Player->m_DeleteWalls)
	{
		RemoveWall();
	}
	if(NetworkClipped(SnappingClient, m_Pos) && NetworkClipped(SnappingClient, m_To))
		return;

	int SnappingClientVersion = GameServer()->GetClientVersion(SnappingClient);

	vec2 From;
	int StartTick;

	if(SnappingClientVersion >= VERSION_DDNET_ENTITY_NETOBJS)
	{
		From = m_To;
		StartTick = -1;
	}
	else
	{
		CCharacter *pChr = GameServer()->GetPlayerChar(SnappingClient);

		if(SnappingClient != SERVER_DEMO_CLIENT && (GameServer()->m_apPlayers[SnappingClient]->GetTeam() == TEAM_SPECTATORS || GameServer()->m_apPlayers[SnappingClient]->IsPaused()) && GameServer()->m_apPlayers[SnappingClient]->m_SpectatorId != SPEC_FREEVIEW)
			pChr = GameServer()->GetPlayerChar(GameServer()->m_apPlayers[SnappingClient]->m_SpectatorId);

		if(pChr && pChr->Team() != TEAM_SUPER && pChr->IsAlive() && !Switchers().empty() && Switchers()[m_Number].m_aStatus[pChr->Team()])
		{
			From = m_To;
		}
		else
		{
			From = m_Pos;
		}
		StartTick = Server()->Tick();
	}

	GameServer()->SnapLaserObject(CSnapContext(SnappingClientVersion), GetId(),
		m_Pos, From, StartTick, -1, LASERTYPE_DOOR, 0, m_Number);
}

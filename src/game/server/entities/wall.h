/* (c) Shereef Marzouk. See "licence DDRace.txt" and the readme.txt in the root of the distribution for more information. */
#ifndef GAME_SERVER_ENTITIES_WALL_H
#define GAME_SERVER_ENTITIES_WALL_H

#include <game/server/entity.h>
#include "character.h"

class CGameWorld;

class CWall : public CEntity
{
	vec2 m_To;
	void ResetCollision();
	int m_Length;
	vec2 m_Direction;

public:
	CWall(CGameWorld *pGameWorld, vec2 Pos, vec2 Pos2, CCharacter User,
		int Number);

	CCharacter *m_Player;

	void RemoveWall();

	void Reset() override;
	void Snap(int SnappingClient) override;
};

#endif // GAME_SERVER_ENTITIES_WALL_H
/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* ScriptData
SDName: Blades_Edge_Mountains
SD%Complete: 90
SDComment: Quest support: 10502, 10503, 10504, 10505, 10556, 10682, 10980. Ogri'la->Skettis Flight. (npc_daranelle needs bit more work before consider complete)
SDCategory: Blade's Edge Mountains
EndScriptData */

/* ContentData
mobs_bladespire_ogre
mobs_bloodmaul_ogre
npc_daranelle
npc_overseer_nuaar
npc_saikkal_the_elder
npc_skyguard_handler_irena
EndContentData */

#include "sc_creature.h"
#include "sc_gossip.h"

/*######
## mobs_bladespire_ogre
######*/

//TODO: add support for quest 10512 + creature abilities
struct MANGOS_DLL_DECL mobs_bladespire_ogreAI : public ScriptedAI
{
    mobs_bladespire_ogreAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
    }

    void Aggro(Unit* who)
    {
    }

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
            ((Player*)Killer)->KilledMonster(19995, m_creature->GetGUID());
    }
};
CreatureAI* GetAI_mobs_bladespire_ogre(Creature *_Creature)
{
    return new mobs_bladespire_ogreAI (_Creature);
}

/*######
## mobs_bloodmaul_ogre
######*/

//TODO: check if they can be done in ACID (meaning, not involved in other quests that require special scripting)
struct MANGOS_DLL_DECL mobs_bloodmaul_ogreAI : public ScriptedAI
{
    mobs_bloodmaul_ogreAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
    }

    void Aggro(Unit* who)
    {
    }

    void JustDied(Unit* Killer)
    {
        if (Killer->GetTypeId() == TYPEID_PLAYER)
            ((Player*)Killer)->KilledMonster(19991, m_creature->GetGUID());
    }
};
CreatureAI* GetAI_mobs_bloodmaul_ogre(Creature *_Creature)
{
    return new mobs_bloodmaul_ogreAI (_Creature);
}

/*######
## npc_daranelle
######*/

struct MANGOS_DLL_DECL npc_daranelleAI : public ScriptedAI
{
    npc_daranelleAI(Creature *c) : ScriptedAI(c) {Reset();}

    void Reset()
    {
    }

    void Aggro(Unit* who)
    {
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->GetTypeId() == TYPEID_PLAYER)
        {
            if(who->HasAura(36904,0))
            {
                DoSay("Good $N, you are under the spell's influence. I must analyze it quickly, then we can talk.",LANG_COMMON,who);
                //TODO: Move the below to updateAI and run if this statement == true
                ((Player*)who)->KilledMonster(21511, m_creature->GetGUID());
                ((Player*)who)->RemoveAurasDueToSpell(36904);
            }
        }

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                //Begin melee attack if we are within range
                DoStartAttackAndMovement(who);
            }
        }
    }
};

CreatureAI* GetAI_npc_daranelle(Creature *_Creature)
{
    return new npc_daranelleAI (_Creature);
}

/*######
## npc_overseer_nuaar
######*/

bool GossipHello_npc_overseer_nuaar(Player *player, Creature *_Creature)
{
    if (player->GetQuestStatus(10682) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, "Overseer, I am here to negotiate on behalf of the Cenarion Expedition.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    player->SEND_GOSSIP_MENU(10532, _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_overseer_nuaar(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        player->SEND_GOSSIP_MENU(10533, _Creature->GetGUID());
        player->AreaExploredOrEventHappens(10682);
    }
    return true;
}

/*######
## npc_saikkal_the_elder
######*/

bool GossipHello_npc_saikkal_the_elder(Player *player, Creature *_Creature)
{
    if (player->GetQuestStatus(10980) == QUEST_STATUS_INCOMPLETE)
        player->ADD_GOSSIP_ITEM( 0, "Yes... yes, it's me.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    player->SEND_GOSSIP_MENU(10794, _Creature->GetGUID());

    return true;
}

bool GossipSelect_npc_saikkal_the_elder(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            player->ADD_GOSSIP_ITEM( 0, "Yes elder. Tell me more of the book.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            player->SEND_GOSSIP_MENU(10795, _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            player->TalkedToCreature(_Creature->GetEntry(), _Creature->GetGUID());
            player->SEND_GOSSIP_MENU(10796, _Creature->GetGUID());
            break;
    }
    return true;
}

/*######
## npc_skyguard_handler_irena
######*/

#define GOSSIP_SKYGUARD "Fly me to Skettis please"

bool GossipHello_npc_skyguard_handler_irena(Player *player, Creature *_Creature )
{
    if (_Creature->isQuestGiver())
        player->PrepareQuestMenu( _Creature->GetGUID() );

    if (player->GetReputationRank(1031) >= REP_HONORED)
        player->ADD_GOSSIP_ITEM( 2, GOSSIP_SKYGUARD, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    player->SEND_GOSSIP_MENU(_Creature->GetNpcTextId(), _Creature->GetGUID());

    return true;
}
bool GossipSelect_npc_skyguard_handler_irena(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if (action == GOSSIP_ACTION_INFO_DEF+1)
    {
        player->CLOSE_GOSSIP_MENU();

        std::vector<uint32> nodes;

        nodes.resize(2);
        nodes[0] = 172;                                     //from ogri'la
        nodes[1] = 171;                                     //end at skettis
        player->ActivateTaxiPathTo(nodes);                  //TaxiPath 706
    }
    return true;
}

/*######
## AddSC
######*/

void AddSC_blades_edge_mountains()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mobs_bladespire_ogre";
    newscript->GetAI = GetAI_mobs_bladespire_ogre;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="mobs_bloodmaul_ogre";
    newscript->GetAI = GetAI_mobs_bloodmaul_ogre;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_daranelle";
    newscript->GetAI = GetAI_npc_daranelle;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_overseer_nuaar";
    newscript->pGossipHello = &GossipHello_npc_overseer_nuaar;
    newscript->pGossipSelect = &GossipSelect_npc_overseer_nuaar;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_saikkal_the_elder";
    newscript->pGossipHello = &GossipHello_npc_saikkal_the_elder;
    newscript->pGossipSelect = &GossipSelect_npc_saikkal_the_elder;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="npc_skyguard_handler_irena";
    newscript->pGossipHello =  &GossipHello_npc_skyguard_handler_irena;
    newscript->pGossipSelect = &GossipSelect_npc_skyguard_handler_irena;
    m_scripts[nrscripts++] = newscript;
}
#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_

//###########################################################################
//## Gameplay
//###########################################################################

#define GAME_MAXOBJS_TYPE Uint8
#define TICK_TYPE Uint16

// REPLICATION IDS
#define GOBJ_ID       1 
#define GENTITY_ID    5
#define GPLAYER_ID    35
#define GSHOT_ID      70
#define GBOMB_ID      85
#define GEXTRA_ID     100
#define GRAIL_ID      125
#define GFRAY_ID      150
#define GSOUND_ID     175
#define GBSHOT_ID     200
#define GGRENADE_ID   225


#define EX_WEP_BASE   0
#define EX_AMM_BASE   30
#define EX_POS_BASE   60
#define EX_NEG_BASE   90
#define EX_PTS_BASE   120
#define EX_SUP_BASE   150

#define EX_WEP_BOMB       EX_WEP_BASE+0      
#define EX_WEP_GRENADE    EX_WEP_BASE+3
#define EX_WEP_RAILGUN    EX_WEP_BASE+6
#define EX_WEP_BECHER     EX_WEP_BASE+9
#define EX_WEP_NUM 4

#define EX_AMM_BOMB       EX_AMM_BASE+0
#define EX_AMM_RAIL       EX_AMM_BASE+3
#define EX_AMM_GREN       EX_AMM_BASE+6
#define EX_AMM_BECHER     EX_AMM_BASE+9
#define EX_AMM_SHIELD     EX_AMM_BASE+12
#define EX_AMM_WARP       EX_AMM_BASE+15
#define EX_AMM_NUM 6

#define EX_POS_PWBM       EX_POS_BASE+0
#define EX_POS_INFRA      EX_POS_BASE+3
#define EX_POS_MVBM       EX_POS_BASE+6
#define EX_POS_INVIS      EX_POS_BASE+9
#define EX_POS_SPEED      EX_POS_BASE+12
#define EX_POS_BERSERK    EX_POS_BASE+15
#define EX_POS_NUM 6

#define EX_NEG_GLUE       EX_NEG_BASE+0
#define EX_NEG_REVERSE    EX_NEG_BASE+3
#define EX_NEG_SLOW       EX_NEG_BASE+6
#define EX_NEG_NOWEAPON   EX_NEG_BASE+9
#define EX_NEG_NUM 4

#define EX_PTS_COCT1      EX_PTS_BASE+0
#define EX_PTS_COCT2      EX_PTS_BASE+3
#define EX_PTS_GOLD1      EX_PTS_BASE+6
#define EX_PTS_GOLD2      EX_PTS_BASE+9
#define EX_PTS_ENERGY     EX_PTS_BASE+12
#define EX_PTS_NUM 5

#define EX_SUP_BECHER     EX_SUP_BASE+0
#define EX_SUP_BECHERX    EX_SUP_BASE+3
#define EX_SUP_NUM 2


#define EX_FLAG_FIXED       1         // fixed / instant
#define EX_FLAG_RESPAWN     2         // stay / respawn (data time)
#define EX_FLAG_RANDOMPLACE 4
#define EX_FLAG_RANDOM_POS  8
#define EX_FLAG_RANDOM_NEG  16
#define EX_FLAG_RANDOM_WEP  32
#define EX_FLAG_RANDOM_PTS  64
#define EX_FLAG_RANDOM_AMM  128

#define PLAYER_BORNPOS  0
#define PLAYER_WARPPOS  1


#define SM_STD_VOLUME 100


#endif

//###########################################################################
//## STD config
//###########################################################################

#define RELEVANT_DESTINATION "50"

#define STD_CLIENT_NAME "noname"
#define STD_CLIENT_DESC "PacPerson"
#define STD_SERVER_NAME "Prototype PacWars2 server"
#define STD_WELCOME_MSG "Becher golden edition"
#define REMOTE_CMD_CHAR '!'

#define KILLING_TIME "400"
#define BANZAIING_TIME "200"

#define MENU_VOLUME "70"

#define PLAYER_SA_PISTOLKA    "30"
#define PLAYER_SA_BECHEROMET  "10"
#define PLAYER_SA_RAILGUN     "5"
#define PLAYER_SA_BOMBS       "20"
#define PLAYER_SA_GRENADES    "10"

#define PLAYER_SA_WITEMS    "1"
#define PLAYER_SA_WEAPON    "0"

#define PLAYER_SA_SHIELDS "1"
#define PLAYER_SA_WARPS   "1"
#define PLAYER_SA_POWER   "30"

#define SOUND_SHIFTX  "200"

#define PISTOLKA_RELOAD_TIME "28"
#define BECHEROMET_RELOAD_TIME "24"
#define RAILGUN_RELOAD_TIME "30"
#define BOMB_RELOAD_TIME "10"
#define GRENADE_RELOAD_TIME "30"
#define WARP_RELOAD_TIME "30"

#define BORN_FRAMEDELAY "8"
#define BORNIN_TIME "48"

#define WARP_FRAMEDELAY "4"
#define WARPIN_TIME "32"
#define WARPOUT_TIME "32"

#define BOMBHIT_TIMEOUT "20"

#define SHOT_LIFE "14"
#define BSHOT_LIFE "22"

#define SHOT_STEP "20"
#define BSHOT_STEP "16"

#define BOMB_TIMEOUT "70"
#define GRENADE_TIMEOUT "150"

#define PISTOLKA_FIRE_END_ANIM "6"
#define BECHEROMET_FIRE_END_ANIM "4"
#define RAILGUN_FIRE_END_ANIM "20"
#define BOMBS_FIRE_END_ANIM "20"
#define GRENADES_FIRE_END_ANIM "20"

#define SHIELDTIME "80"

#define INVISALPHA "110"

#define PLAYER_RAIL_DELAY "12"
#define PLAYER_RAIL_SHIFTX "21"
#define PLAYER_RAIL_SHIFTY "7"
#define PLAYER_BECHEROMET_SHIFTX "16"
#define PLAYER_BECHEROMET_SHIFTY "6"
#define PLAYER_PISTOLKA_SHIFTX "0"
#define PLAYER_PISTOLKA_SHIFTY "5"
#define PLAYER_GRENADE_SHIFTX "20"
#define PLAYER_GRENADE_SHIFTY "0"
#define PLAYER_GRENADE_MOVEMENT "10"

#define BOMB_MV_SPEED "8"

#define FRAY_GR_SX "32"
#define FRAY_GR_SY "32"
#define MAX_FRAY "40"

#define SKIN_GR_SX "32"
#define SKIN_GR_SY "32"

#define RAIL_GR_SX "16"
#define RAIL_GR_SY "16"
#define MAX_RAIL "50"
#define RAIL_LIFE "12"

#define MAX_AMMO_BOMBS         "50"
#define MAX_AMMO_GRENADES      "20"
#define MAX_AMMO_RAILGUN       "70"
#define MAX_AMMO_PISTOLKA      "100"
#define MAX_AMMO_BECHEROMET    "200"

#define SPEC_AMMO_BOMBS      "5"
#define SPEC_AMMO_GRENADES   "10"
#define SPEC_AMMO_RAILGUN    "10"
#define SPEC_AMMO_BECHEROMET "20"
#define SPEC_AMMO_PISTOLKA   "30"

#define SPEC_INFRA_TIME     "3000"
#define SPEC_REVERSE_TIME   "200"
#define SPEC_SLOW_TIME      "800"
#define SPEC_SPEED_TIME     "400"
#define SPEC_GLUE_TIME      "150"
#define SPEC_INVIS_TIME     "750"
#define SPEC_NOWEAPON_TIME  "400"
#define SPEC_BERSERK_TIME   "500"

#define MAX_WARPS        "50"
#define MAX_SHIELDS      "50"

#define SPEC_ADD_WARPS   "2"
#define SPEC_ADD_SHIELDS "2"

#define EXTRA_POWER "40"            // musi byt mensi nez relevant destination

#define POINTS_BECHER "80"
#define POINTS_RAPID  "20"
#define POINTS_COCT1  "13"
#define POINTS_COCT2  "11"
#define POINTS_GOLD1  "5"
#define POINTS_GOLD2  "6"
#define POINTS_FRAG   "15"
#define POINTS_MFRAG   "10"
#define POINTS_SUPERBECHER "100"
#define POINTS_SUPERBECHERDESTROY "0"

extern cvar_t relevant_destination;
extern cvar_t killing_time;
extern cvar_t banzaiing_time;
extern cvar_t menu_volume;
extern cvar_t player_sa_pistolka   ;
extern cvar_t player_sa_becheromet ;
extern cvar_t player_sa_railgun    ;
extern cvar_t player_sa_bombs      ;
extern cvar_t player_sa_grenades   ;
extern cvar_t player_sa_witems    ;
extern cvar_t player_sa_shields;
extern cvar_t player_sa_warps  ;
extern cvar_t player_sa_power  ;
extern cvar_t player_sa_weapon;
extern cvar_t sound_shiftx ;
extern cvar_t pistolka_reload_time;
extern cvar_t becheromet_reload_time;
extern cvar_t railgun_reload_time;
extern cvar_t bomb_reload_time;
extern cvar_t grenade_reload_time;
extern cvar_t born_framedelay;
extern cvar_t bornin_time;
extern cvar_t warp_reload_time;
extern cvar_t warp_framedelay;
extern cvar_t warpin_time;
extern cvar_t warpout_time;
extern cvar_t bombhit_timeout;
extern cvar_t shot_life;
extern cvar_t bshot_life;
extern cvar_t shot_step;
extern cvar_t bshot_step;
extern cvar_t bomb_timeout;
extern cvar_t grenade_timeout;
extern cvar_t pistolka_fire_end_anim;
extern cvar_t becheromet_fire_end_anim;
extern cvar_t railgun_fire_end_anim;
extern cvar_t bombs_fire_end_anim;
extern cvar_t grenades_fire_end_anim;
extern cvar_t shieldtime;
extern cvar_t invisalpha;
extern cvar_t player_rail_delay;
extern cvar_t player_rail_shiftx;
extern cvar_t player_rail_shifty;
extern cvar_t player_becheromet_shiftx;
extern cvar_t player_becheromet_shifty;
extern cvar_t player_pistolka_shiftx;
extern cvar_t player_pistolka_shifty;
extern cvar_t player_grenade_shiftx;
extern cvar_t player_grenade_shifty;
extern cvar_t player_grenade_movement;
extern cvar_t bomb_mv_speed;
extern cvar_t fray_gr_sx;
extern cvar_t fray_gr_sy;
extern cvar_t max_fray;
extern cvar_t skin_gr_sx;
extern cvar_t skin_gr_sy;
extern cvar_t rail_gr_sx;
extern cvar_t rail_gr_sy;
extern cvar_t max_rail;
extern cvar_t rail_life;
extern cvar_t max_ammo_bombs        ;
extern cvar_t max_ammo_grenades     ;
extern cvar_t max_ammo_railgun      ;
extern cvar_t max_ammo_pistolka     ;
extern cvar_t max_ammo_becheromet   ;
extern cvar_t spec_ammo_bombs     ;
extern cvar_t spec_ammo_grenades  ;
extern cvar_t spec_ammo_railgun   ;
extern cvar_t spec_ammo_becheromet;
extern cvar_t spec_ammo_pistolka  ;
extern cvar_t spec_infra_time    ;
extern cvar_t spec_reverse_time  ;
extern cvar_t spec_slow_time     ;
extern cvar_t spec_speed_time    ;
extern cvar_t spec_glue_time     ;
extern cvar_t spec_invis_time    ;
extern cvar_t spec_noweapon_time ;
extern cvar_t spec_berserk_time  ;
extern cvar_t max_warps       ;
extern cvar_t max_shields     ;
extern cvar_t spec_add_warps  ;
extern cvar_t spec_add_shields;
extern cvar_t extra_power;
extern cvar_t points_becher;
extern cvar_t points_rapid ;
extern cvar_t points_coct1 ;
extern cvar_t points_coct2 ;
extern cvar_t points_gold1 ;
extern cvar_t points_gold2 ;
extern cvar_t points_frag  ;
extern cvar_t points_mfrag  ;
extern cvar_t points_superbecher;
extern cvar_t points_superbecherdestroy;

extern void ResetGameSettings();



//###########################################################################
//## STD config
//###########################################################################
#include "const.h"
#include "cvar.h"
#include "stdconfig.h"

cvar_t relevant_destination = {"relevant_destination", RELEVANT_DESTINATION, false, true};
cvar_t killing_time = {"killing_time", KILLING_TIME, false, true};
cvar_t banzaiing_time = {"banzaiing_time", BANZAIING_TIME, false, true};

cvar_t menu_volume = {"menu_volume", MENU_VOLUME, false, true};

cvar_t player_sa_pistolka    = {"player_sa_pistolka", PLAYER_SA_PISTOLKA   , false, true};
cvar_t player_sa_becheromet  = {"player_sa_becheromet", PLAYER_SA_BECHEROMET , false, true};
cvar_t player_sa_railgun     = {"player_sa_railgun", PLAYER_SA_RAILGUN    , false, true};
cvar_t player_sa_bombs       = {"player_sa_bombs", PLAYER_SA_BOMBS      , false, true};
cvar_t player_sa_grenades    = {"player_sa_grenades", PLAYER_SA_GRENADES   , false, true};

cvar_t player_sa_witems     = {"player_sa_witems", PLAYER_SA_WITEMS    , false, true};
cvar_t player_sa_weapon     = {"player_sa_weapon", PLAYER_SA_WEAPON    , false, true};

cvar_t player_sa_shields = {"player_sa_shields", PLAYER_SA_SHIELDS, false, true};
cvar_t player_sa_warps   = {"player_sa_warps", PLAYER_SA_WARPS  , false, true};
cvar_t player_sa_power   = {"player_sa_power", PLAYER_SA_POWER  , false, true};

cvar_t sound_shiftx  = {"sound_shiftx", SOUND_SHIFTX , false, true};

cvar_t pistolka_reload_time = {"pistolka_reload_time", PISTOLKA_RELOAD_TIME, false, true};
cvar_t becheromet_reload_time = {"becheromet_reload_time", BECHEROMET_RELOAD_TIME, false, true};
cvar_t railgun_reload_time = {"railgun_reload_time", RAILGUN_RELOAD_TIME, false, true};
cvar_t bomb_reload_time = {"bomb_reload_time", BOMB_RELOAD_TIME, false, true};
cvar_t grenade_reload_time = {"grenade_reload_time", GRENADE_RELOAD_TIME, false, true};
cvar_t warp_reload_time = {"warp_reload_time", WARP_RELOAD_TIME, false, true};

cvar_t born_framedelay = {"born_framedelay", BORN_FRAMEDELAY, false, true};
cvar_t bornin_time = {"bornin_time", BORNIN_TIME, false, true};

cvar_t warp_framedelay = {"warp_framedelay", WARP_FRAMEDELAY, false, true};
cvar_t warpin_time = {"warpin_time", WARPIN_TIME, false, true};
cvar_t warpout_time = {"warpout_time", WARPOUT_TIME, false, true};

cvar_t bombhit_timeout = {"bombhit_timeout", BOMBHIT_TIMEOUT, false, true};

cvar_t shot_life = {"shot_life", SHOT_LIFE, false, true};
cvar_t bshot_life = {"bshot_life", BSHOT_LIFE, false, true};

cvar_t shot_step = {"shot_step", SHOT_STEP, false, true};
cvar_t bshot_step = {"bshot_step", BSHOT_STEP, false, true};

cvar_t bomb_timeout = {"bomb_timeout", BOMB_TIMEOUT, false, true};
cvar_t grenade_timeout = {"grenade_timeout", GRENADE_TIMEOUT, false, true};

cvar_t pistolka_fire_end_anim = {"pistolka_fire_end_anim", PISTOLKA_FIRE_END_ANIM, false, true};
cvar_t becheromet_fire_end_anim = {"becheromet_fire_end_anim", BECHEROMET_FIRE_END_ANIM, false, true};
cvar_t railgun_fire_end_anim = {"railgun_fire_end_anim", RAILGUN_FIRE_END_ANIM, false, true};
cvar_t bombs_fire_end_anim = {"bombs_fire_end_anim", BOMBS_FIRE_END_ANIM, false, true};
cvar_t grenades_fire_end_anim = {"grenades_fire_end_anim", GRENADES_FIRE_END_ANIM, false, true};

cvar_t shieldtime = {"shieldtime", SHIELDTIME, false, true};

cvar_t invisalpha = {"invisalpha", INVISALPHA, false, true};

cvar_t player_rail_delay = {"player_rail_delay", PLAYER_RAIL_DELAY, false, true};
cvar_t player_rail_shiftx = {"player_rail_shiftx", PLAYER_RAIL_SHIFTX, false, true};
cvar_t player_rail_shifty = {"player_rail_shifty", PLAYER_RAIL_SHIFTY, false, true};
cvar_t player_becheromet_shiftx = {"player_becheromet_shiftx", PLAYER_BECHEROMET_SHIFTX, false, true};
cvar_t player_becheromet_shifty = {"player_becheromet_shifty", PLAYER_BECHEROMET_SHIFTY, false, true};
cvar_t player_pistolka_shiftx = {"player_pistolka_shiftx", PLAYER_PISTOLKA_SHIFTX, false, true};
cvar_t player_pistolka_shifty = {"player_pistolka_shifty", PLAYER_PISTOLKA_SHIFTY, false, true};
cvar_t player_grenade_shiftx = {"player_grenade_shiftx", PLAYER_GRENADE_SHIFTX, false, true};
cvar_t player_grenade_shifty = {"player_grenade_shifty", PLAYER_GRENADE_SHIFTY, false, true};
cvar_t player_grenade_movement = {"player_grenade_movement", PLAYER_GRENADE_MOVEMENT, false, true};

cvar_t bomb_mv_speed = {"bomb_mv_speed", BOMB_MV_SPEED, false, true};

cvar_t fray_gr_sx = {"fray_gr_sx", FRAY_GR_SX, false, true};
cvar_t fray_gr_sy = {"fray_gr_sy", FRAY_GR_SY, false, true};
cvar_t max_fray = {"max_fray", MAX_FRAY, false, true};

cvar_t skin_gr_sx = {"skin_gr_sx", SKIN_GR_SX, false, true};
cvar_t skin_gr_sy = {"skin_gr_sy", SKIN_GR_SY, false, true};

cvar_t rail_gr_sx = {"rail_gr_sx", RAIL_GR_SX, false, true};
cvar_t rail_gr_sy = {"rail_gr_sy", RAIL_GR_SY, false, true};
cvar_t max_rail = {"max_rail", MAX_RAIL, false, true};
cvar_t rail_life = {"rail_life", RAIL_LIFE, false, true};

cvar_t max_ammo_bombs         = {"max_ammo_bombs", MAX_AMMO_BOMBS        , false, true};
cvar_t max_ammo_grenades      = {"max_ammo_grenades", MAX_AMMO_GRENADES     , false, true};
cvar_t max_ammo_railgun       = {"max_ammo_railgun", MAX_AMMO_RAILGUN      , false, true};
cvar_t max_ammo_pistolka      = {"max_ammo_pistolka", MAX_AMMO_PISTOLKA     , false, true};
cvar_t max_ammo_becheromet    = {"max_ammo_becheromet", MAX_AMMO_BECHEROMET   , false, true};

cvar_t spec_ammo_bombs      = {"spec_ammo_bombs", SPEC_AMMO_BOMBS     , false, true};
cvar_t spec_ammo_grenades   = {"spec_ammo_grenades", SPEC_AMMO_GRENADES  , false, true};
cvar_t spec_ammo_railgun    = {"spec_ammo_railgun", SPEC_AMMO_RAILGUN   , false, true};
cvar_t spec_ammo_becheromet = {"spec_ammo_becheromet", SPEC_AMMO_BECHEROMET, false, true};
cvar_t spec_ammo_pistolka   = {"spec_ammo_pistolka", SPEC_AMMO_PISTOLKA  , false, true};

cvar_t spec_infra_time     = {"spec_infra_time", SPEC_INFRA_TIME    , false, true};
cvar_t spec_reverse_time   = {"spec_reverse_time", SPEC_REVERSE_TIME  , false, true};
cvar_t spec_slow_time      = {"spec_slow_time", SPEC_SLOW_TIME     , false, true};
cvar_t spec_speed_time     = {"spec_speed_time", SPEC_SPEED_TIME    , false, true};
cvar_t spec_glue_time      = {"spec_glue_time", SPEC_GLUE_TIME     , false, true};
cvar_t spec_invis_time     = {"spec_invis_time", SPEC_INVIS_TIME    , false, true};
cvar_t spec_noweapon_time  = {"spec_noweapon_time", SPEC_NOWEAPON_TIME , false, true};
cvar_t spec_berserk_time   = {"spec_berserk_time", SPEC_BERSERK_TIME  , false, true};

cvar_t max_warps        = {"max_warps", MAX_WARPS       , false, true};
cvar_t max_shields      = {"max_shields", MAX_SHIELDS     , false, true};

cvar_t spec_add_warps   = {"spec_add_warps", SPEC_ADD_WARPS  , false, true};
cvar_t spec_add_shields = {"spec_add_shields", SPEC_ADD_SHIELDS, false, true};

cvar_t extra_power = {"extra_power", EXTRA_POWER, false, true};

cvar_t points_becher = {"points_becher", POINTS_BECHER, false, true};
cvar_t points_rapid  = {"points_rapid", POINTS_RAPID , false, true};
cvar_t points_coct1  = {"points_coct1", POINTS_COCT1 , false, true};
cvar_t points_coct2  = {"points_coct2", POINTS_COCT2 , false, true};
cvar_t points_gold1  = {"points_gold1", POINTS_GOLD1 , false, true};
cvar_t points_gold2  = {"points_gold2", POINTS_GOLD2 , false, true};
cvar_t points_frag   = {"points_frag", POINTS_FRAG  , false, true};
cvar_t points_mfrag   = {"points_mfrag", POINTS_MFRAG  , false, true};
cvar_t points_superbecher = {"points_superbecher", POINTS_SUPERBECHER, false, true};
cvar_t points_superbecherdestroy = {"points_superbecherdestroy", POINTS_SUPERBECHERDESTROY, false, true};

void ResetGameSettings()
{
  Cvar_Set("relevant_destination", RELEVANT_DESTINATION);
  Cvar_Set("killing_time", KILLING_TIME);
  Cvar_Set("banzaiing_time", BANZAIING_TIME);

//  Cvar_Set("menu_volume", MENU_VOLUME);

  Cvar_Set("player_sa_pistolka", PLAYER_SA_PISTOLKA   );
  Cvar_Set("player_sa_becheromet", PLAYER_SA_BECHEROMET );
  Cvar_Set("player_sa_railgun", PLAYER_SA_RAILGUN    );
  Cvar_Set("player_sa_bombs", PLAYER_SA_BOMBS      );
  Cvar_Set("player_sa_grenades", PLAYER_SA_GRENADES   );

  Cvar_Set("player_sa_witems", PLAYER_SA_WITEMS    );
  Cvar_Set("player_sa_weapon", PLAYER_SA_WEAPON);

  Cvar_Set("player_sa_shields", PLAYER_SA_SHIELDS);
  Cvar_Set("player_sa_warps", PLAYER_SA_WARPS  );
  Cvar_Set("player_sa_power", PLAYER_SA_POWER  );

  Cvar_Set("sound_shiftx", SOUND_SHIFTX );

  Cvar_Set("pistolka_reload_time", PISTOLKA_RELOAD_TIME);
  Cvar_Set("becheromet_reload_time", BECHEROMET_RELOAD_TIME);
  Cvar_Set("railgun_reload_time", RAILGUN_RELOAD_TIME);
  Cvar_Set("bomb_reload_time", BOMB_RELOAD_TIME);
  Cvar_Set("grenade_reload_time", GRENADE_RELOAD_TIME);
  Cvar_Set("warp_reload_time", WARP_RELOAD_TIME);

  Cvar_Set("born_framedelay", BORN_FRAMEDELAY);
  Cvar_Set("bornin_time", BORNIN_TIME);

  Cvar_Set("warp_framedelay", WARP_FRAMEDELAY);
  Cvar_Set("warpin_time", WARPIN_TIME);
  Cvar_Set("warpout_time", WARPOUT_TIME);

  Cvar_Set("bombhit_timeout", BOMBHIT_TIMEOUT);

  Cvar_Set("shot_life", SHOT_LIFE);
  Cvar_Set("bshot_life", BSHOT_LIFE);

  Cvar_Set("shot_step", SHOT_STEP);
  Cvar_Set("bshot_step", BSHOT_STEP);

  Cvar_Set("bomb_timeout", BOMB_TIMEOUT);
  Cvar_Set("grenade_timeout", GRENADE_TIMEOUT);

  Cvar_Set("pistolka_fire_end_anim", PISTOLKA_FIRE_END_ANIM);
  Cvar_Set("becheromet_fire_end_anim", BECHEROMET_FIRE_END_ANIM);
  Cvar_Set("railgun_fire_end_anim", RAILGUN_FIRE_END_ANIM);
  Cvar_Set("bombs_fire_end_anim", BOMBS_FIRE_END_ANIM);
  Cvar_Set("grenades_fire_end_anim", GRENADES_FIRE_END_ANIM);

  Cvar_Set("shieldtime", SHIELDTIME);

  Cvar_Set("invisalpha", INVISALPHA);

  Cvar_Set("player_rail_delay", PLAYER_RAIL_DELAY);
  Cvar_Set("player_rail_shiftx", PLAYER_RAIL_SHIFTX);
  Cvar_Set("player_rail_shifty", PLAYER_RAIL_SHIFTY);
  Cvar_Set("player_becheromet_shiftx", PLAYER_BECHEROMET_SHIFTX);
  Cvar_Set("player_becheromet_shifty", PLAYER_BECHEROMET_SHIFTY);
  Cvar_Set("player_pistolka_shiftx", PLAYER_PISTOLKA_SHIFTX);
  Cvar_Set("player_pistolka_shifty", PLAYER_PISTOLKA_SHIFTY);
  Cvar_Set("player_grenade_shiftx", PLAYER_GRENADE_SHIFTX);
  Cvar_Set("player_grenade_shifty", PLAYER_GRENADE_SHIFTY);
  Cvar_Set("player_grenade_movement", PLAYER_GRENADE_MOVEMENT);

  Cvar_Set("bomb_mv_speed", BOMB_MV_SPEED);

  Cvar_Set("fray_gr_sx", FRAY_GR_SX);
  Cvar_Set("fray_gr_sy", FRAY_GR_SY);
  Cvar_Set("max_fray", MAX_FRAY);

  Cvar_Set("skin_gr_sx", SKIN_GR_SX);
  Cvar_Set("skin_gr_sy", SKIN_GR_SY);

  Cvar_Set("rail_gr_sx", RAIL_GR_SX);
  Cvar_Set("rail_gr_sy", RAIL_GR_SY);
  Cvar_Set("max_rail", MAX_RAIL);
  Cvar_Set("rail_life", RAIL_LIFE);

  Cvar_Set("max_ammo_bombs", MAX_AMMO_BOMBS        );
  Cvar_Set("max_ammo_grenades", MAX_AMMO_GRENADES     );
  Cvar_Set("max_ammo_railgun", MAX_AMMO_RAILGUN      );
  Cvar_Set("max_ammo_pistolka", MAX_AMMO_PISTOLKA     );
  Cvar_Set("max_ammo_becheromet", MAX_AMMO_BECHEROMET   );

  Cvar_Set("spec_ammo_bombs", SPEC_AMMO_BOMBS     );
  Cvar_Set("spec_ammo_grenades", SPEC_AMMO_GRENADES  );
  Cvar_Set("spec_ammo_railgun", SPEC_AMMO_RAILGUN   );
  Cvar_Set("spec_ammo_becheromet", SPEC_AMMO_BECHEROMET);
  Cvar_Set("spec_ammo_pistolka", SPEC_AMMO_PISTOLKA  );

  Cvar_Set("spec_infra_time", SPEC_INFRA_TIME    );
  Cvar_Set("spec_reverse_time", SPEC_REVERSE_TIME  );
  Cvar_Set("spec_slow_time", SPEC_SLOW_TIME     );
  Cvar_Set("spec_speed_time", SPEC_SPEED_TIME    );
  Cvar_Set("spec_glue_time", SPEC_GLUE_TIME     );
  Cvar_Set("spec_invis_time", SPEC_INVIS_TIME    );
  Cvar_Set("spec_noweapon_time", SPEC_NOWEAPON_TIME );
  Cvar_Set("spec_berserk_time", SPEC_BERSERK_TIME  );

  Cvar_Set("max_warps", MAX_WARPS       );
  Cvar_Set("max_shields", MAX_SHIELDS     );

  Cvar_Set("spec_add_warps", SPEC_ADD_WARPS  );
  Cvar_Set("spec_add_shields", SPEC_ADD_SHIELDS);

  Cvar_Set("extra_power", EXTRA_POWER);

  Cvar_Set("points_becher", POINTS_BECHER);
  Cvar_Set("points_rapid", POINTS_RAPID );
  Cvar_Set("points_coct1", POINTS_COCT1 );
  Cvar_Set("points_coct2", POINTS_COCT2 );
  Cvar_Set("points_gold1", POINTS_GOLD1 );
  Cvar_Set("points_gold2", POINTS_GOLD2 );
  Cvar_Set("points_frag", POINTS_FRAG  );
  Cvar_Set("points_mfrag", POINTS_MFRAG  );
  Cvar_Set("points_superbecher", POINTS_SUPERBECHER);
  Cvar_Set("points_superbecherdestroy", POINTS_SUPERBECHERDESTROY);
}

/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Common modifications of gameplay
//## author:  Antonin Hildebrand
//## 
//## started: 04.01.2001
//## revised: ???
//## 
//###########################################################################

// (IMPORTS/EXPORTS)
// MUST BE COMPATIBILE WITH PACSCRIPT

#ifndef MODS_INTERFACE
#define MODS_INTERFACE

void Mod_Base()
{

}

void Mod_Masacre()
{
  Cvar_Set("player_sa_witems", "3");  // becheromet+pistolka only
  Cvar_Set("becheromet_reload_time", "5");
}

void Mod_Western()
{
  Cvar_Set("pistolka_reload_time", "40");
  Cvar_Set("player_sa_bombs", "0");  
  Cvar_Set("player_sa_pistolka", "230");  
  Cvar_Set("player_sa_railgun", "0");  
  Cvar_Set("player_sa_becheromet", "0");  
  Cvar_Set("player_sa_grenades", "0");  
  Cvar_Set("player_sa_shields", "6");  
  Cvar_Set("player_sa_warps", "2");  
}

void Mod_Bombing(char* power)
{
  Cvar_Set("player_sa_witems", "8");  // bombs only
  Cvar_Set("player_sa_power", power);   // initial power of flame
  Cvar_Set("player_sa_weapon", "3");  // set bombs as a starting weapon

  // other items set to 0
  Cvar_Set("player_sa_pistolka", "0");  
  Cvar_Set("player_sa_railgun", "0");  
  Cvar_Set("player_sa_bombs", "200");  // 200 bombs for start
  Cvar_Set("player_sa_becheromet", "0");  
  Cvar_Set("player_sa_grenades", "0");  
  Cvar_Set("player_sa_shields", "0");  
  Cvar_Set("player_sa_warps", "5");  
}

void Mod_RailingOnly(char* rail_delay)
{
  Cvar_Set("player_sa_witems", "4");  // possible weapons = rail
  Cvar_Set("player_sa_weapon", "2");  // starting weapon = rail

  // other items set to 0
  Cvar_Set("player_rail_delay", rail_delay);  
  Cvar_Set("player_sa_bombs", "0");  
  Cvar_Set("player_sa_pistolka", "50");  
  Cvar_Set("player_sa_railgun", "40");  
  Cvar_Set("player_sa_becheromet", "0");  
  Cvar_Set("player_sa_grenades", "0");  
  Cvar_Set("player_sa_shields", "5");  
  Cvar_Set("player_sa_warps", "2");  
}


void Mod_UltraRailingOnly()
{
  Cvar_Set("player_sa_witems", "4");  // possible weapons = rail
  Cvar_Set("player_sa_weapon", "2");  // starting weapon = rail

  // other items set to 0
  Cvar_Set("player_rail_delay", "6");  
  Cvar_Set("player_sa_bombs", "0");  
  Cvar_Set("player_sa_pistolka", "50");  
  Cvar_Set("player_sa_railgun", "40");  
  Cvar_Set("player_sa_becheromet", "0");  
  Cvar_Set("player_sa_grenades", "0");  
  Cvar_Set("player_sa_shields", "5");  
  Cvar_Set("player_sa_warps", "2");  
}


void Mod_SlowRailing()
{
  Cvar_Set("player_sa_witems", "4");  // possible weapons = rail + pistolka
  Cvar_Set("player_sa_weapon", "2");  // starting weapon = rail

  // other items set to 0
  Cvar_Set("player_rail_delay", "35");  // big rail delay
  Cvar_Set("player_sa_bombs", "0");  
  Cvar_Set("player_sa_pistolka", "0");  
  Cvar_Set("player_sa_railgun", "40");  
  Cvar_Set("player_sa_becheromet", "0");  
  Cvar_Set("player_sa_grenades", "0");  
  Cvar_Set("player_sa_shields", "5");  
  Cvar_Set("player_sa_warps", "2");  
}


#endif




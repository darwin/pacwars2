/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   network protocol
//## author:  Antonin Hildebrand
//##         
//## started: 29.3.2000
//## revised: ???
//## 
//##          
//## 
//###########################################################################

#ifndef _PACNETPROTOCOL_H_
#define _PACNETPROTOCOL_H_

#include "SDL_net.h"
#include "const.h"

//###########################################################################
//## Transfer souboru pres TCP
/*
   pres msg kanal bude probihat veskera synchronizace pomoci mechanismu zprav [UDP]
   pres file kanal bude probihat prenost dat souboru [TCP]

   vyssi: download
          upload   

   fajl -> komprimace -> transfer -> dekomprimace -> temp_fajl
   po dokonceni prejmenovani temp_fajlu na fajl
*/

//###########################################################################
//## Constants & defines
//###########################################################################


//#define PWP_MAX_PLAYERS     16 
#define PWP_TICKS_PER_PACKET_SERVER 1
#define PWP_TICKS_PER_PACKET_CLIENT 1

#define PWP_TOTALMAX_CLIENTS 16 // !!! keep SDLNET_MAX_UDPCHANNELS >= PWP_MAX_CLIENTS+1
#define PWP_MAX_PLAYERS_PER_CLIENT 4
#define PWP_MAX_CLIENT_NAME 12
#define PWP_MAX_SERVER_NAME 40
#define PWP_MAX_PLAYER_NAME 14     // especially 14 chars for WIZard Raist
#define PWP_MAX_FILE_NAME   128
#define PWP_MAX_MAP_NAME    MAX_MAP_NAME
#define PWP_MAX_PASSWORD    12

#define PWP_MSG_CPORT    25971
#define PWP_MSG_SPORT    25970
#define PWP_GAME_CPORT   ?????  // vzdy MSG_CPORT+1
#define PWP_GAME_SPORT   25910
#define PWP_FILE_PORT    24990
#define PWP_SOCKET_RANGE    10
#define PWP_PORT_STEP       10
// Resend times
#define PWP_MAX_RESENDS 5

#define RESEND_CHAT        500
#define RESEND_GAME        200
#define RESEND_STD         300
#define RESEND_SYSTEM     1000
#define RESEND_PING       5000
#define RESEND_ENTERINGGAME 100  // this has to be fastest as possible
#define RESEND_GAME_SERVER 900
#define RESEND_GAME_CLIENT 700

// Timeouts
#define STD_FILE_TIMEOUT "10000"      // 10s 
#define STD_INACTIVE_TIMEOUT "5000" // 5s
#define STD_DROP_TIMEOUT "20000" // 20s
#define STD_PING_TIMEOUT "20000" // 20s 


// Messages
// ID, 
const Uint8 MSGID_LOGIN          =2;
const Uint8 MSGID_WELCOME        =3;
const Uint8 MSGID_SERVERFULL     =4;
const Uint8 MSGID_LOGOUT         =5;
const Uint8 MSGID_SERVERDOWN     =6;
const Uint8 MSGID_PING           =7;  // s->c
const Uint8 MSGID_SNAME          =8;  // change server name
const Uint8 MSGID_CNAME          =9;  // change client name
const Uint8 MSGID_CHATPOST      =10;
const Uint8 MSGID_CHATWRITE     =11;
const Uint8 MSGID_REMOTECMD     =12;  // c->s remote command
const Uint8 MSGID_REMOTECON     =13;  // c->s remote console out
const Uint8 MSGID_ATTACHCON     =14;  // c->s remote console attach
const Uint8 MSGID_DETACHCON     =15;  // c->s remote console detach
const Uint8 MSGID_DOWNLOAD      =16;  // c->s download request
const Uint8 MSGID_UPLOAD        =17;  // c->s upload request
const Uint8 MSGID_CHMAP         =18;  // s->c server is going to change map 
const Uint8 MSGID_CHMAPREADY    =19;  // c->s client have chaged map and is ready to play
const Uint8 MSGID_SERVEROFF     =20;  // s->c server is not running
const Uint8 MSGID_YWDROPPED     =21;  // s->c "you were dropped"
const Uint8 MSGID_ENTERGAME     =22;  // s->c, after CHMAPREADY
const Uint8 MSGID_CREATEPLAYER  =23;  // c->s, (after ENTERGAME)
const Uint8 MSGID_DESTROYPLAYER =24;  // c->s
const Uint8 MSGID_OWNERPLAYER   =25;  // c->s, (after ENTERGAME)
const Uint8 MSGID_SKINPLAYER    =26;  // c->s change player skin
const Uint8 MSGID_KBNOTIFY      =27;  // s->c notifies client to free keybindings
const Uint8 MSGID_CHASEPLAYERRQ =28;  // c->s chase certain player
const Uint8 MSGID_CHASEPLAYERCF =29;  // s->c of chasing player confirmation
const Uint8 MSGID_SOUND         =30;  // s->c sound replaying
const Uint8 MSGID_VOICE         =31;  // s->c sound replaying
const Uint8 MSGID_ATTACHDONE    =32;  // s->c console attached


#define MSG_SPOOL_SIZE      30
#define MSG_RPOOL_SIZE      50

#define GAME_SPOOL_SIZE      30
#define GAME_RPOOL_SIZE      50

// MESSAGE header
#define MSG_HEADER_SIZE      4
#define POS_CRC              0
#define LEN_CRC              2
#define POS_NUM              2  // message NUM, to acknowledge receiving
#define LEN_NUM              2  // if NUM=0, this message is an acknowledgement

// MESSAGE bodys
#define MSG_MAX_BODY_SIZE 400
#define POS_MSG_ID    0
#define LEN_MSG_ID    1

// MSGID_LOGIN
#define POS_MSG_NAME  5
#define LEN_MSG_NAME  PWP_MAX_CLIENT_NAME
#define POS_MSG_PASS  (POS_MSG_NAME+LEN_MSG_NAME)
#define LEN_MSG_PASS  PWP_MAX_CLIENT_NAME

// MSGID_CHATPOST, MSGID_CHATWRITE
#define POS_MSG_TEXT 1
#define LEN_MSG_TEXT ???

// MSGID_WELCOME
#define POS_MSG_CLIENT_NUM  1
#define LEN_MSG_CLIENT_NUM  1
#define POS_MSG_WVERSION 2
#define LEN_MSG_WVERSION 2
#define POS_MSG_WSNAME  4
#define LEN_MSG_WSNAME  PWP_MAX_SERVER_NAME
#define POS_MSG_WSMSG  (POS_MSG_WSNAME+LEN_MSG_WSNAME)
#define LEN_MSG_WSMSG  60
#define POS_MSG_WMAPNAME  (POS_MSG_WSMSG+LEN_MSG_WSMSG)
#define LEN_MSG_WMAPNAME  PWP_MAX_MAP_NAME
#define POS_MSG_WSCRIPTNAME  (POS_MSG_WMAPNAME+LEN_MSG_WMAPNAME)
#define LEN_MSG_WSCRIPTNAME  PWP_MAX_MAP_NAME

// MSGID_CHSNAME
#define POS_MSG_SNAME  1
#define LEN_MSG_SNAME  PWP_MAX_SERVER_NAME

// MSGID_CHPNAME
#define POS_MSG_CNAME  1
#define LEN_MSG_CNAME  PWP_MAX_CLIENT_NAME

// MSGID_REMOTECMD
#define POS_MSG_CMD  1
#define LEN_MSG_CMD  MSG_MAX_BODY_SIZE - 1

// MSGID_REMOTECON
#define POS_MSG_CONTYPE  1
#define LEN_MSG_CONTYPE  1
#define POS_MSG_CON  2
#define LEN_MSG_CON  MSG_MAX_BODY_SIZE - 2

// MSGID_DOWNLOAD, MSGID_UPLOAD
#define POS_MSG_FPORT  1
#define LEN_MSG_FPORT  2
#define POS_MSG_DLID   3
#define LEN_MSG_DLID   1
#define POS_MSG_FN     4
#define LEN_MSG_FN      PWP_MAX_FILE_NAME

// MSGID_CHMAP
#define POS_MSG_MAPNAME  1
#define LEN_MSG_MAPNAME  PWP_MAX_MAP_NAME
#define POS_MSG_SCRIPTNAME  (POS_MSG_MAPNAME+LEN_MSG_MAPNAME)
#define LEN_MSG_SCRIPTNAME  PWP_MAX_MAP_NAME

#define PWP_MGP_SCOUNT 10                               // max game packet count on server
#define PWP_MMP_SCOUNT 10                               // max message packet count on server
#define PWP_MGP_CCOUNT 5                                // max game packet count on client
#define PWP_MMP_CCOUNT 5                                // max message packet count on client
#define PWP_MGP_SIZE MSG_HEADER_SIZE+MSG_MAX_BODY_SIZE  // max game packet size
#define PWP_MMP_SIZE MSG_HEADER_SIZE+MSG_MAX_BODY_SIZE  // max message packet size  

// MSGID_CREATEPLAYER
#define POS_MSG_TYPE  1
#define LEN_MSG_TYPE  1
#define POS_MSG_KBD   2
#define LEN_MSG_KBD   1
#define POS_MSG_PLNAME  3
#define LEN_MSG_PLNAME  PWP_MAX_PLAYER_NAME

// MSGID_DESTROYPLAYER
#define POS_MSG_DESTROY_PLNAME  1
#define LEN_MSG_DESTROY_PLNAME  PWP_MAX_PLAYER_NAME

#define POS_MSG_SKIN_PLNAME  1
#define LEN_MSG_SKIN_PLNAME  PWP_MAX_PLAYER_NAME
#define POS_MSG_SKIN_SPRITENAME  (POS_MSG_SKIN_PLNAME+LEN_MSG_SKIN_PLNAME)
#define LEN_MSG_SKIN_SPRITENAME  MAX_SPRITE_NAME

// MSGID_KBNOTIFY
#define POS_MSG_KBINDEX  1
#define LEN_MSG_KBINDEX  1


// FILE TRANSFER
#define PWP_FQUANTUM_SIZE        512
#define PWP_FBUFFER_SIZE    20*PWP_FQUANTUM_SIZE

#define PWP_UPLOAD_COMPRESSION Z_DEFAULT_COMPRESSION
#define PWP_DOWNLOAD_COMPRESSION Z_DEFAULT_COMPRESSION

// DOWNLOAD/UPLOAD IDS
#define DLID_MAP      0
#define DLID_SKIN     1
#define DLID_SPRITE   2
#define DLID_SOUND    3
#define DLID_SCRIPT   4

// MSGID_ATTACHCON
#define POS_MSG_ACPASS  1
#define LEN_MSG_ACPASS  PWP_MAX_PASSWORD

//###########################################################################
//## Replication
//###########################################################################


// REP CODES

const Uint8 REP_CODEBASE     = (GAME_MAX_OBJS+1); // base_id of replicated functions -> params

// server side
const Uint8 REP_END            = REP_CODEBASE+1;   // end of replication
const Uint8 REP_REPLICATION    = REP_CODEBASE+2;   // -> object_slot, object_type
const Uint8 REP_ADJUSTPOSITION = REP_CODEBASE+3;   // -> object_slot, params

//const Uint8 REP_CREATE         = REP_CODEBASE+2;   // -> class_id, object_slot, creation_tick
//const Uint8 REP_DESTROY        = REP_CODEBASE+3;   // -> object_slot
const Uint8 REP_SERVERMOVE     = REP_CODEBASE+4;   // -> object_slot, params
//const Uint8 REP_MOVEAUTONOMOUS = 4;   // -> object_slot, params

// layers
#define REP_MAX_LAYER 3
#define REP_BUFFER_SIZE 10000
#define REP_MAX_MARKERS 200

const Uint8 LAY_MOVEVECTOR      = 0;  // the most important layer

const Uint8 LAY_CREATEDESTROY   = 0;  // the most important layer
const Uint8 LAY_REPLICATION     = 1;  
const Uint8 LAY_ADITIONAL       = 2;  


#endif
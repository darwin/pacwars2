/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   top network engine layer
//## author:  Antonin Hildebrand
//##         
//## started: 13.3.2000
//## revised: ???
//## 
//##          Based on low level multiplatform SDL_net mod for SDL lib
//## 
//###########################################################################

#ifndef _PACNET_H_
#define _PACNET_H_

#include <stdarg.h>
#include <string.h>
#include "SDL_net.h"
#include "protocol.h"
#include "zlib.h"
#include "const.h"
#include "SDL_Console.h"

//###########################################################################
//## Constants & defines
//###########################################################################
#define NS_UNINITED   0
#define NS_INITED     1
#define NS_RUNNING    2
#define NS_CONNECTED  3
#define NS_VIEWING_RESULTS 4

const char NET_STATES_STR[][20] =
{
	"uninited",
	"inited",
	"running",
	"connected",
	"viewing results"
};

#define PS_UNUSED     0
#define PS_USED       1
#define PS_CONFIRMED  2

#define CS_UNUSED       0
#define CS_INACTIVE     1    // netdead or not playing
#define CS_ACTIVE       2    // ACTIVE & playing
#define CS_PREPARING    3    // PREPARING MAP

const char CLIENT_STATES_STR[][20] =
{
	"unused",
	"inactive",
	"active",
	"preparing"
};


#define SS_UNUSED     0
#define SS_ACTIVE     1
#define SS_DEAD       2

const char SERVER_STATES_STR[][20] =
{
	"unused",
	"active",
	"dead"
};

#define FS_UNINITED     0
#define FS_READY        1
#define FS_SENDING      3
#define FS_RECEIVING    4
#define FS_COMPLETED    5
#define FS_ENDED        6
#define FS_WAIT_SENDING    7
#define FS_WAIT_RECEIVING  8

const char FILE_STATES_STR[][20] =
{
	"uninited",
	"ready",
	"sending",
	"receiving",
	"completed",
	"ended",
	"wait for sending",
	"wait for receiving"
};

#define FT_FINISHED         0
#define FT_ABANDONED        1
#define FT_FINISHED_RENAME  2

const char FILETRANSFER_STATES_STR[][20] =
{
	"finished",
	"abandoned",
	"finished rename"
};

#define SD_CHANGING_GAME 0
#define SD_SHUTDOWN      1
#define SD_OFFLINE       2
#define SD_CHANGING_MAP  3

const char SD_STATES_STR[][20] =
{
	"changing game",
	"shutdown",
	"offline"
};

#define PO(packet,offset) ((Uint8*)&packet->data[offset])      // packet offset

//###########################################################################
//## Data structures
//###########################################################################
#define MAX_PSTRING_CHARS 30 // must be enough for all uses of PString

class DECLSPEC PString {
public:
  PString() { len = 0; }
  PString(char* s) { len = strlen(s); strcpy(chars, s); }
  
  operator char*() { chars[len]=0; return chars; }
  bool operator==(PString& rs) { return len==rs.len && memcmp(chars, rs.chars, len)==0; }
  bool operator!=(PString& rs) { return len!=rs.len || memcmp(chars, rs.chars, len)!=0; }
  PString& operator=(char* rs) { len = strlen(rs); strcpy(chars, rs); chars[len]=0; return *this; }
  PString& operator=(PString& rs) { len = rs.len; memcpy(chars, rs.chars, len); chars[len]=0; return *this; }

  void Clear() { for (int i=0; i<len; i++) chars[i]=0; }

  int len;
  char chars[MAX_PSTRING_CHARS+1];
};

typedef struct _net_message net_message;

typedef void (net_msg_callback)(net_message *msg);

struct _net_message {
    int       state;
    Uint8     times;
    Uint16    id;
    Uint32    sended;
    Uint32    timeout;
    net_msg_callback *msgcb;
    Uint16    tag;
    UDPpacket *packet;
};

typedef struct {
    Uint16 msg_num;
    int ssize;
    net_message *smessages;   // sent messages
    int rsize;
    int rpos;
    Uint16 *rmessages;        // received messages
} net_message_pool;

class DECLSPEC net_stats {
public:
    int errCRC;
    int errDuplicity;
    int errDropped;
    
    int PacketsSent;
    int PacketsReceived;
    Uint32 LastRecTime;
    
    void Clear();
};

class DECLSPEC net_msg {
public:
    Uint16 pos;
    Uint8* buf;
    
    net_msg();
    net_msg(Uint16 ipos);
    net_msg(Uint8* ibuf);
    net_msg(Uint16 ipos, Uint8* ibuf);
    ~net_msg();
    
    void W8(Uint8 val);
    void W16(Uint16 val);
    void W32(Uint32 val);
    void WStr(char* s);
    void WMem(void* src, int len);
    
    Uint8  R8(void);
    Uint16 R16(void);
    Uint32 R32(void);
    char*  RStr(void);
    void   RMem(void* dest, int len);
    
    net_msg& operator<<(net_msg&);
    net_msg& operator<<(Uint8);
    net_msg& operator<<(Uint16);
    net_msg& operator<<(Uint32);
    net_msg& operator<<(Sint8);
    net_msg& operator<<(Sint16);
    net_msg& operator<<(Sint32);
    net_msg& operator<<(char *);
    net_msg& operator<<(PString);
    
    net_msg& operator>>(net_msg&);
    net_msg& operator>>(Uint8&);
    net_msg& operator>>(Uint16&);
    net_msg& operator>>(Uint32&);
    net_msg& operator>>(Sint8&);
    net_msg& operator>>(Sint16&);
    net_msg& operator>>(Sint32&);
    net_msg& operator>>(char*&);
    net_msg& operator>>(PString&);
    
    int Send(net_message_pool *pool, net_stats* stats, UDPsocket sock, int channel,
        bool reliable, Uint32 resend=RESEND_STD, net_msg_callback *msgcb=NULL, Uint16 tag=0);
    int Receive(net_message_pool *pool, net_stats* stats, UDPsocket sock, int channel, UDPpacket *packet, bool confirm);
    
    void nprintf(size_t count, const char *fmt, ... );

    net_msg * next;
};

//###########################################################################
//## Globals
//###########################################################################
extern DECLSPEC int net_status;

extern DECLSPEC cvar_t	net_inactive_timeout;
extern DECLSPEC cvar_t	net_drop_timeout;
extern DECLSPEC cvar_t	net_file_timeout;
extern DECLSPEC cvar_t	net_ping_timeout;

//###########################################################################
//## Exported functions
//###########################################################################

extern DECLSPEC char *AddrToS(IPaddress *ip);
extern DECLSPEC bool CheckFileName(char* fname);
extern DECLSPEC bool DLIDtoFileName(Uint8 DLID, char* fname, char* full_name);

//###########################################################################
//## Message pool
//###########################################################################


extern DECLSPEC int SendMsg(net_message_pool *pool, UDPsocket sock, int channel, void *data, int len, 
            bool reliable, Uint32 resend=RESEND_STD, net_msg_callback *msgcb=NULL, Uint16 tag=0);
extern DECLSPEC int SendMsg(net_message_pool *pool, UDPsocket sock, int channel, net_msg msg, 
            bool reliable, Uint32 resend=RESEND_STD, net_msg_callback *msgcb=NULL, Uint16 tag=0);

extern DECLSPEC int ReceiveMsg(net_message_pool *pool, UDPsocket sock, int channel, UDPpacket *packet, bool confirm);
extern DECLSPEC int ResendMsgs(net_message_pool *pool, UDPsocket sock);

extern DECLSPEC int LoadFileBuffer(z_streamp zs, Bytef* quantum, int qsize, Bytef* buffer, int bsize, FILE* file, int* file_status);
extern DECLSPEC int StoreFileBuffer(z_streamp zs, Bytef* quantum, int qsize, Bytef* buffer, int bsize, FILE* file, int* file_status);

extern DECLSPEC int CreatePool(net_message_pool *pool, int send_size, int rec_size, int packet_size);
extern DECLSPEC int ClearPool(net_message_pool *pool);
extern DECLSPEC int DestroyPool(net_message_pool *pool);
extern DECLSPEC int UnconfirmedMsgs(net_message_pool *pool);

extern DECLSPEC int PollNet();

extern DECLSPEC int InitNet();
extern DECLSPEC int DoneNet();

#endif

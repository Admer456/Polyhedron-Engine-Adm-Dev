/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

// qcommon.h -- definitions common between client and server, but not game.dll

#include "config.h"
#include "q_shared.h"
#include "q_files.h"
#include "com_public.h"
#include "protocol.h"
#include "q_msg.h"

#ifdef DEDICATED_ONLY
#define APPLICATION     "q2proded"
#else
#define APPLICATION     "q2pro"
#endif

#define	BASEGAME		"baseq2"
#define BASEGAMEDESCRIPTION	"Quake II"

/*
==============================================================

CMD

Command text buffering and command execution

==============================================================
*/

#define CMD_BUFFER_SIZE		( 1 << 16 )		// bumped max config size up to 64K

#define	ALIAS_LOOP_COUNT		16

typedef struct {
	char	    *text; // may not be NULL terminated
    int         cursize;
    int         maxsize;
	int			waitCount;
	int			aliasCount; // for detecting runaway loops
	void		(*exec)( const char * );
} cmdbuf_t;

extern char			cmd_buffer_text[CMD_BUFFER_SIZE];
extern cmdbuf_t		cmd_buffer;

/*

Any number of commands can be added in a frame, from several different sources.
Most commands come from either keybindings or console line input, but remote
servers can also send across commands and entire text files can be execed.

The + command line options are also added to the command buffer.

The game starts with a Cbuf_AddText ("exec quake.rc\n"); Cbuf_Execute ();

*/

void Cbuf_Init( void );
// allocates an initial text buffer that will grow as needed

void Cbuf_AddTextEx( cmdbuf_t *buf, const char *text );
// as new commands are generated from the console or keybindings,
// the text is added to the end of the command buffer.

void Cbuf_InsertTextEx( cmdbuf_t *buf, const char *text );
// when a command wants to issue other commands immediately, the text is
// inserted at the beginning of the buffer, before any remaining unexecuted
// commands.

void Cbuf_ExecuteEx( cmdbuf_t *buf );
// Pulls off \n terminated lines of text from the command buffer and sends
// them through Cmd_ExecuteString.  Stops when the buffer is empty.
// Normally called once per frame, but may be explicitly invoked.
// Do not call inside a command function!

char *Cbuf_Alloc( cmdbuf_t *buf, int length );

#define Cbuf_AddText( text )	Cbuf_AddTextEx( &cmd_buffer, text )
#define Cbuf_InsertText( text )	Cbuf_InsertTextEx( &cmd_buffer, text )
#define Cbuf_Execute()			Cbuf_ExecuteEx( &cmd_buffer )

//===========================================================================

/*

Command execution takes a null terminated string, breaks it into tokens,
then searches for a command or variable that matches the first token.

*/

void	Cmd_Init( void );

qboolean Cmd_Exists( const char *cmd_name );
// used by the cvar code to check for cvar / command name overlap

xcommand_t Cmd_FindFunction( const char *name );
xmacro_t Cmd_FindMacroFunction( const char *name );
xgenerator_t Cmd_FindGenerator( const char *name, int index );

char *Cmd_AliasCommand( const char *name );
void Cmd_AliasSet( const char *name, const char *cmd );

const char *Cmd_Command_g( const char *text, int state );
const char *Cmd_Alias_g( const char *text, int state );
const char *Cmd_Mixed_g( const char *partial, int state ); 
// attempts to match a partial command for automatic command line completion
// returns NULL if nothing fits

void	Cmd_TokenizeString( const char *text, qboolean macroExpand );
// Takes a null terminated string.  Does not need to be /n terminated.
// breaks the string up into arg tokens.

void	Cmd_ExecuteString( const char *text );
// Parses a single line of text into arguments and tries to execute it
// as if it was typed at the console

void	Cmd_ForwardToServer( void );
// adds the current command line as a clc_stringcmd to the client message.
// things like godmode, noclip, etc, are commands directed to the server,
// so when they are typed in at the console, they will need to be forwarded.

char *Cmd_MacroExpandString( const char *text, qboolean aliasHack );

void Cbuf_ExecuteText( cbufExecWhen_t exec_when, const char *text );
// this can be used in place of either Cbuf_AddText or Cbuf_InsertText

void    Cmd_Register( const cmdreg_t *reg );
void	Cmd_AddCommand( const char *cmd_name, xcommand_t function );
// called by the init functions of other parts of the program to
// register commands and functions to call for them.
// The cmd_name is referenced later, so it should not be in temp memory
// if function is NULL, the command will be forwarded to the server
// as a clc_stringcmd instead of executed locally
void    Cmd_Deregister( const cmdreg_t *reg );
void	Cmd_RemoveCommand( const char *cmd_name );

void Cmd_AddMacro( const char *name, xmacro_t function );

int		Cmd_Argc( void );
char	*Cmd_Argv( int arg );
char	*Cmd_Args( void );
char	*Cmd_RawArgs( void );
char	*Cmd_ArgsFrom( int from );
char	*Cmd_RawArgsFrom( int from );
void	Cmd_ArgsBuffer( char *buffer, int bufferSize );
void	Cmd_ArgvBuffer( int arg, char *buffer, int bufferSize );
int Cmd_EnumParam( int start, const char *sp, const char *lp );
int Cmd_CheckParam( const char *sp, const char *lp );
char *Cmd_FindParam( const char *sp, const char *lp );
int Cmd_ArgOffset( int arg );
int Cmd_FindArgForOffset( int offset );
char *Cmd_RawString( void );
void Cmd_Shift( void );
// The functions that execute commands get their parameters with these
// functions. Cmd_Argv () will return an empty string, not a NULL
// if arg > argc, so string operations are always safe.

void Cmd_Alias_f( void );

void Cmd_FillAPI( cmdAPI_t *api );


#define EXEC_TRIGGER( var ) \
    do { \
        if( (var)->string[0] ) { \
            Cbuf_AddText( (var)->string ); \
        } \
    } while( 0 )

/*
==============================================================

CVAR

==============================================================
*/

/*

cvar_t variables are used to hold scalar or string variables that can be changed or displayed at the console or prog code as well as accessed directly
in C code.

The user can access cvars from the console in three ways:
r_draworder			prints the current value
r_draworder 0		sets the current value to 0
set r_draworder 0	as above, but creates the cvar if not present
Cvars are restricted from having the same names as commands to keep this
interface from being ambiguous.
*/

typedef enum {
	CVAR_SET_CONSOLE,
	CVAR_SET_COMMAND_LINE,
	CVAR_SET_DIRECT
} cvarSetSource_t;

extern	cvar_t	*cvar_vars;
extern	uint32	cvar_latchedModified;
extern	uint32	cvar_infoModified;

void Cvar_SetByVar( cvar_t *var, const char *value, cvarSetSource_t source );

cvar_t *Cvar_UserSet( const char *var_name, const char *value );

cvar_t *Cvar_ForceSet (const char *var_name, const char *value);
// will set the variable even if NOSET or LATCH

cvar_t 	*Cvar_FullSet( const char *var_name, const char *value,
					  int flags, cvarSetSource_t source );

void Cvar_ClampInteger( cvar_t *var, int min, int max );
void Cvar_ClampValue( cvar_t *var, float min, float max );

const char *Cvar_Generator( const char *text, int state );
// attempts to match a partial variable name for command line completion
// returns NULL if nothing fits

void	Cvar_GetLatchedVars (void);
// any CVAR_LATCHEDED variables that have been set will now take effect

void Cvar_FixCheats( void );

void Cvar_Command( cvar_t *v );
// called by Cmd_ExecuteString when Cmd_Argv(0) doesn't match a known
// command.  Returns qtrue if the command was a variable reference that
// was handled. (print or change)

void 	Cvar_WriteVariables( fileHandle_t f );
// appends lines containing "set variable value" for all variables
// with the archive flag set to qtrue.

void	Cvar_Init (void);

char *Cvar_BitInfo( int bit );
char *Cvar_BitInfo_Big( int bit );
//
// returns an info string containing all the CVAR_USERINFO cvars
#define Cvar_Userinfo()     Cvar_BitInfo( CVAR_USERINFO )

// returns an info string containing all the CVAR_SERVERINFO cvars
#define Cvar_Serverinfo()   Cvar_BitInfo( CVAR_SERVERINFO )

cvar_t *Cvar_ForceSetEx( const char *var_name, const char *value, int flags );

qboolean CL_CheatsOK( void );

qboolean Cvar_Exists( const char *name );

cvar_t *Cvar_Get( const char *var_name, const char *value, int flags );
// creates the variable if it doesn't exist, or returns the existing one
// if it exists, the value will not be changed, but flags will be ORed in
// that allows variables to be unarchived without needing bitflags

cvar_t 	*Cvar_Set( const char *var_name, const char *value );
cvar_t 	*Cvar_SetEx( const char *var_name, const char *value, cvarSetSource_t source );
// will create the variable if it doesn't exist

void	Cvar_SetValue( const char *var_name, float value );
void Cvar_SetInteger( const char *var_name, int value );
void Cvar_SetIntegerHex( const char *var_name, uint32 value );
// expands value to a string and calls Cvar_Set

float	Cvar_VariableValue( const char *var_name );
int Cvar_VariableInteger( const char *var_name );
// returns 0 if not defined or non numeric

char	*Cvar_VariableString( const char *var_name );
void Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufferSize );
// returns an empty string if not defined

cvar_t *Cvar_FindVar( const char *var_name );

void Cvar_Set_f( void );

void Cvar_Subsystem( cvarSubsystem_t subsystem );

void Cvar_FillAPI( cvarAPI_t *api );

/*
==============================================================

FIFO

==============================================================
*/

typedef struct {
    byte *data;
    int size;
    int ax, ay, bs;
} fifo_t;

static inline void *FIFO_Reserve( fifo_t *fifo, int *reserved ) {
    int tail;

    if( fifo->bs ) {
        *reserved = fifo->ax - fifo->bs;
        return fifo->data + fifo->bs;
    }

    tail = fifo->size - fifo->ay;
    if( fifo->ax < tail ) {
        *reserved = tail;
        return fifo->data + fifo->ay;
    }

    *reserved = fifo->ax;
    return fifo->data;
}

static inline void FIFO_Commit( fifo_t *fifo, int length ) {
    int tail;

    if( fifo->bs ) {
        fifo->bs += length;
        return;
    }

    tail = fifo->size - fifo->ay;
    if( fifo->ax < tail ) {
        fifo->ay += length;
        return;
    }

    fifo->bs = length;
}

static inline void *FIFO_Peek( fifo_t *fifo, int *length ) {
    *length = fifo->ay - fifo->ax;
    return fifo->data + fifo->ax;
}

static inline void FIFO_Decommit( fifo_t *fifo, int length ) {
    if( fifo->ax + length < fifo->ay ) {
        fifo->ax += length;
        return;
    }

    fifo->ay = fifo->bs;
    fifo->ax = fifo->bs = 0;
}

static inline int FIFO_Percent( fifo_t *fifo ) {
    if( !fifo->size ) {
        return 0;
    }
    return ( fifo->ay - fifo->ax + fifo->bs ) * 100 / fifo->size;
}

static inline int FIFO_Usage( fifo_t *fifo ) {
    return fifo->ay - fifo->ax + fifo->bs;
}

qboolean FIFO_Read( fifo_t *fifo, void *buffer, int length );
qboolean FIFO_Write( fifo_t *fifo, const void *buffer, int length );

/*
==============================================================

NET

==============================================================
*/

// net.h -- quake's interface to the networking layer

#define	PORT_ANY	-1

#define MAX_PACKETLEN	4096		// max length of a single packet
#define	PACKET_HEADER	10			// two ints and a short (worst case)
#define MAX_PACKETLEN_DEFAULT	1400		// default quake2 limit
#define MAX_PACKETLEN_WRITABLE    ( MAX_PACKETLEN - PACKET_HEADER )
#define MAX_PACKETLEN_WRITABLE_DEFAULT    ( MAX_PACKETLEN_DEFAULT - PACKET_HEADER )

typedef enum netadrtype_e {
	NA_BAD,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP
} netadrtype_t;

typedef enum netsrc_e {
	NS_CLIENT,
	NS_SERVER,
	NS_COUNT
} netsrc_t;

typedef enum netflag_e {
	NET_NONE	= 0,
	NET_CLIENT	= ( 1 << 0 ),
	NET_SERVER	= ( 1 << 1 )
} netflag_t;

typedef enum netstat_e {
    NET_OK,
    NET_AGAIN,
    NET_CLOSED,
    NET_ERROR,
} neterr_t;

typedef struct netadr_s {
	netadrtype_t	type;
	byte    ip[4];
	uint16	port;
} netadr_t;

static inline qboolean NET_IsEqualAdr( const netadr_t *a, const netadr_t *b ) {
	if( a->type != b->type ) {
		return qfalse;
	}

	switch( a->type ) {
	case NA_LOOPBACK:
		return qtrue;
	case NA_IP:
	case NA_BROADCAST:
		if( *( uint32 * )a->ip == *( uint32 * )b->ip && a->port == b->port ) {
			return qtrue;
		}
		return qfalse;
	default:
		break;
	}

	return qfalse;
}

static inline qboolean NET_IsEqualBaseAdr( const netadr_t *a, const netadr_t *b ) {
	if( a->type != b->type ) {
		return qfalse;
	}

	switch( a->type ) {
	case NA_LOOPBACK:
		return qtrue;
	case NA_IP:
	case NA_BROADCAST:
		if( *( uint32 * )a->ip == *( uint32 * )b->ip ) {
			return qtrue;
		}
		return qfalse;
	default:
		break;
	}

	return qfalse;
}

static inline qboolean NET_IsLanAddress( const netadr_t *adr ) {
	switch( adr->type ) {
	case NA_LOOPBACK:
		return qtrue;
	case NA_IP:
	case NA_BROADCAST:
		if( adr->ip[0] == 127 || adr->ip[0] == 10 ) {
			return qtrue;
		}
		if( *( uint16 * )adr->ip == MakeShort( 192, 168 ) ||
            *( uint16 * )adr->ip == MakeShort( 172,  16 ) )
		{
			return qtrue;
		}
		return qfalse;
	default:
		break;
	}

	return qfalse;
}

void		NET_Init( void );
void		NET_Shutdown( void );

void		NET_Config( netflag_t flag );
qboolean    NET_GetAddress( netsrc_t sock, netadr_t *adr );

neterr_t	NET_GetPacket( netsrc_t sock );
neterr_t	NET_SendPacket( netsrc_t sock, const netadr_t *to, uint32 length,
        const byte *data );
qboolean	NET_GetLoopPacket( netsrc_t sock );

char *		NET_AdrToString( const netadr_t *a );
qboolean	NET_StringToAdr( const char *s, netadr_t *a );
void		NET_Sleep( int msec );

#define		NET_IsLocalAddress( adr )	( (adr)->type == NA_LOOPBACK )

const char	*NET_ErrorString( void );

extern cvar_t       *net_ip;
extern cvar_t       *net_port;

//============================================================================

typedef enum netstate_e {
    NS_DISCONNECTED,// no socket opened
    NS_CONNECTING,  // connect() not yet completed
    NS_CONNECTED,   // may transmit data
    NS_CLOSED,      // peer has preformed orderly shutdown
    NS_BROKEN       // fatal error has been signaled
} netstate_t;

typedef struct netstream_s {
    int         socket;
    netadr_t    address;
    netstate_t  state;
    fifo_t      recv;
    fifo_t      send;
} netstream_t;

void NET_Close( netstream_t *s );
neterr_t NET_Listen( qboolean listen );
neterr_t NET_Accept( netadr_t *peer, netstream_t *s );
neterr_t NET_Connect( const netadr_t *peer, netstream_t *s );
neterr_t NET_Run( netstream_t *s );

//============================================================================

typedef enum netchan_type_e {
	NETCHAN_OLD,
	NETCHAN_NEW
} netchan_type_t;

typedef struct netchan_s {
	netchan_type_t	type;
	int			protocol;
    int         maxpacketlen;

	qboolean	fatal_error;

	netsrc_t	sock;

	int			dropped;			// between last packet and previous

	uint32		last_received;		// for timeouts
	uint32		last_sent;			// for retransmits

	netadr_t	remote_address;
	int			qport;				// qport value to write when transmitting

	sizebuf_t	message;		// writing buffer for reliable data

	int			reliable_length;
	
	qboolean	reliable_ack_pending;	// set to qtrue each time reliable is received
	qboolean	fragment_pending;

	// sequencing variables
	int			incoming_sequence;
	int			incoming_acknowledged;
	int			outgoing_sequence;

	int			(*Transmit)( struct netchan_s *, int, const byte * );
	int			(*TransmitNextFragment)( struct netchan_s * );
	qboolean	(*Process)( struct netchan_s * );
	qboolean	(*ShouldUpdate)( struct netchan_s * );
} netchan_t;

extern netadr_t     net_from;

extern cvar_t       *net_qport;
extern cvar_t       *net_maxmsglen;
extern cvar_t       *net_chantype;

void Netchan_Init( void );
neterr_t Netchan_OutOfBand( netsrc_t sock, const netadr_t *adr, uint32 length,
        const byte *data );
neterr_t Netchan_OutOfBandPrint( netsrc_t sock, const netadr_t *adr,
        const char *format, ... );
netchan_t *Netchan_Setup( netsrc_t sock, netchan_type_t type,
        const netadr_t *adr, int qport, int maxpacketlen, int protocol );
void Netchan_Close( netchan_t *netchan );

/*
==============================================================

CMODEL

==============================================================
*/

typedef struct mapsurface_s {  // used internally due to name len probs //ZOID
	csurface_t	c;
	char		rname[32];
} mapsurface_t;

typedef struct cnode_s {
	cplane_t			*plane;		// never NULL to differentiate from leafs
	struct cnode_s		*children[2];
} cnode_t;

typedef struct {
	cplane_t		*plane;
	mapsurface_t	*surface;
} cbrushside_t;

typedef struct {
	int				contents;
	int				numsides;
	cbrushside_t	*firstbrushside;
	int				checkcount;		// to avoid repeated testings
} cbrush_t;

typedef struct {
	cplane_t	*plane;			// always NULL to differentiate from nodes
	int			contents;
	int			cluster;
	int			area;
	cbrush_t	**firstleafbrush;
	int			numleafbrushes;
} cleaf_t;

typedef struct {
	int		numareaportals;
	int		firstareaportal;
	int		floodvalid;
} carea_t;

typedef struct {
	uint32		portalnum;
	uint32		otherarea;
} careaportal_t;

typedef struct cmodel_s {
	vec3_t		mins, maxs;
	vec3_t		origin;		// for sounds or lights
	cnode_t		*headnode;
} cmodel_t;

typedef struct cmcache_s {
	char		name[MAX_QPATH];
	mempool_t	pool;
	uint32		checksum;
	qboolean	onlyVis;
	int			refcount;

	int			numbrushsides;
	cbrushside_t *brushsides;

	int				numtexinfo;
	mapsurface_t	*surfaces;

	int				numplanes;
	cplane_t		*planes;

	int			numnodes;
	cnode_t		*nodes;	

	int			numleafs;
	cleaf_t		*leafs;

	int			numleafbrushes;
	cbrush_t	**leafbrushes;

	int			numcmodels;
	cmodel_t	*cmodels;

	int			numbrushes;
	cbrush_t	*brushes;

	int			numclusters;
	int			numvisibility;
	int			visrowsize;
	dvis_t		*vis;

	int			numEntityChars;
	char		*entitystring;

	int			numareas;
	carea_t		*areas;

	int			numareaportals;
	careaportal_t *areaportals;
} cmcache_t;

typedef struct {
	cmcache_t	*cache;
	int			*floodnums;			// if two areas have equal floodnums, they are connected
	qboolean	*portalopen;
} cm_t;

#define CM_LOAD_CLIENT	1
#define CM_LOAD_VISONLY	2

void		CM_Init( void );

void		CM_FreeMap( cm_t *cm );
qboolean	CM_LoadMap( cm_t *cm, const char *name, uint32 flags, uint32 *checksum );
cmodel_t	*CM_InlineModel( cm_t *cm, const char *name );	// *1, *2, etc

int CM_NumClusters( cm_t *cm );
int CM_NumInlineModels( cm_t *cm );
char *CM_EntityString( cm_t *cm );
cnode_t *CM_NodeNum( cm_t *cm, int number );
cleaf_t *CM_LeafNum( cm_t *cm, int number );

// creates a clipping hull for an arbitrary box
cnode_t *CM_HeadnodeForBox( vec3_t mins, vec3_t maxs );


// returns an ORed contents mask
int			CM_PointContents( vec3_t p, cnode_t *headnode );
int			CM_TransformedPointContents( vec3_t p, cnode_t *headnode,
									vec3_t origin, vec3_t angles );

void		CM_BoxTrace( trace_t *trace, vec3_t start, vec3_t end,
						  vec3_t mins, vec3_t maxs,
						  cnode_t *headnode, int brushmask );
void		CM_TransformedBoxTrace( trace_t *trace, vec3_t start, vec3_t end,
						  vec3_t mins, vec3_t maxs,
						  cnode_t * headnode, int brushmask,
						  vec3_t origin, vec3_t angles );

byte		*CM_ClusterPVS( cm_t *cm, int cluster);
byte		*CM_ClusterPHS( cm_t *cm, int cluster );
byte		*CM_FatPVS( cm_t *cm, const vec3_t org );

cleaf_t		*CM_PointLeaf( cm_t *cm, vec3_t p );

// call with topnode set to the headnode, returns with topnode
// set to the first node that splits the box
int			CM_BoxLeafs( cm_t *cm, vec3_t mins, vec3_t maxs, cleaf_t **list,
							int listsize, cnode_t **topnode );

#define CM_LeafContents( leaf )		(leaf)->contents
#define CM_LeafCluster( leaf )		(leaf)->cluster
#define CM_LeafArea( leaf )		(leaf)->area

void		CM_SetAreaPortalState ( cm_t *cm, int portalnum, qboolean open );
qboolean	CM_AreasConnected( cm_t *cm, int area1, int area2 );

int			CM_WriteAreaBits( cm_t *cm, byte *buffer, int area );
int			CM_WritePortalBits( cm_t *cm, byte *buffer );
void		CM_SetPortalStates( cm_t *cm, byte *buffer, int bytes );
qboolean	CM_HeadnodeVisible( cnode_t *headnode, byte *visbits );

void		CM_WritePortalState( cm_t *cm, fileHandle_t f );
void		CM_ReadPortalState( cm_t *cm, fileHandle_t f );

/*
==============================================================

PLAYER MOVEMENT CODE

Common between server and client so prediction matches

==============================================================
*/

typedef struct {
	qboolean	airaccelerate;
	qboolean	strafeHack;
    qboolean    flyfix;
	int			qwmod;
	float		speedMultiplier;
	float		upspeed;
	float		maxspeed;
	float		friction;
	float		waterfriction;
    float       flyfriction;
#ifdef PMOVE_HACK
	vec3_t		origin;
	vec3_t		velocity;
	qboolean	highprec;
#endif
} pmoveParams_t;

void Pmove( pmove_t *pmove, pmoveParams_t *params );

/*
==============================================================

FILESYSTEM

==============================================================
*/

void	FS_Init( void );
void	FS_Shutdown( qboolean total );
qboolean    FS_NeedRestart( void );
void        FS_Restart( void );
qboolean    FS_SafeToRestart( void );

qboolean FS_CopyFile( const char *src, const char *dst );
qboolean FS_RemoveFile( const char *path );
qboolean FS_RenameFile( const char *from, const char *to );

char    *FS_CopyExtraInfo( const char *name, const fsFileInfo_t *info );

int		FS_FOpenFile( const char *filename, fileHandle_t *f, uint32 mode );
void	FS_FCloseFile( fileHandle_t hFile );

int		FS_LoadFile( const char *path, void  **buffer );
int		FS_LoadFileEx( const char *path, void **buffer, uint32 flags );
void    *FS_AllocTempMem( int length );
void	FS_FreeFile( void *buffer );
// a null buffer will just return the file length without loading
// a -1 length is not present

int		FS_Read( void *buffer, int len, fileHandle_t hFile );
int		FS_Write( const void *buffer, int len, fileHandle_t hFile );
// properly handles partial reads

void	FS_FPrintf( fileHandle_t f, const char *format, ... );

int		FS_Tell( fileHandle_t f );
int		FS_RawTell( fileHandle_t f );

int		FS_GetFileLength( fileHandle_t f );
int		FS_GetFileLengthNoCache( fileHandle_t f );

qboolean FS_WildCmp( const char *filter, const char *string );
qboolean FS_ExtCmp( const char *extension, const char *string );

char	**FS_ListFiles( const char *path, const char *extension, uint32 flags, int *numFiles );
void	FS_FreeFileList( char **list );

qboolean	FS_LastFileFromPak( void );

void	FS_CreatePath( const char *path );

//const char *FS_GetFileName( fileHandle_t f );
const char *FS_GetFileFullPath( fileHandle_t f );

char	*FS_Gamedir( void );
char	*FS_NextPath( char *prevpath );

void FS_FillAPI( fsAPI_t *api );

extern cvar_t	*fs_game;


/*
==============================================================

MDFOUR

==============================================================
*/

typedef struct mdfour {
	uint32 A, B, C, D;
	uint32 totalN;
} mdfour_t;

void mdfour_begin( struct mdfour *md );
void mdfour_update( struct mdfour *md, uint8 *in, int n );
void mdfour_result( struct mdfour *md, uint8 *out );

uint32	Com_BlockChecksum( void *buffer, int length );

/*
==============================================================

MISC

==============================================================
*/

typedef struct {
	const char	*name;
	void	(* const func)( void );
} ucmd_t;

static inline const ucmd_t *Com_Find( const ucmd_t *u, const char *c ) {
	for( ; u->name; u++ ) {
		if( !strcmp( c, u->name ) ) {
			return u;
		}
	}
    return NULL;
}

void		Com_BeginRedirect (int target, char *buffer, int buffersize, void (*flush));
void		Com_EndRedirect (void);

void		Com_LevelPrint( comPrintType_t type, const char *str );
void		Com_LevelError( comErrorType_t code, const char *str ) q_noreturn;

void		Com_FillAPI( commonAPI_t *api );

void 		Com_Quit (void);

byte		COM_BlockSequenceCRCByte (byte *base, int length, int sequence);

void		Com_ProcessEvents( void );

const char *Com_FileNameGenerator( const char *path, const char *ext, const char *partial,
								  qboolean stripExtension, int state );
const char *Com_FileNameGeneratorByFilter( const char *path, const char *filter, const char *partial,
										  qboolean stripExtension, int state );

/* may return pointer to static memory */
char    *Cvar_CopyString( const char *in );

void	Z_Free( void *ptr );
void	*Z_TagMalloc( size_t size, memtag_t tag ) q_malloc;
void    *Z_TagMallocz( size_t size, memtag_t tag ) q_malloc;
char    *Z_TagCopyString( const char *in, memtag_t tag ) q_malloc;
void	Z_FreeTags( memtag_t tag );
void	Z_LeakTest( memtag_t tag );
void	Z_Check( void );

void    Z_TagReserve( size_t size, memtag_t tag );
void    *Z_ReservedAlloc( size_t size ) q_malloc;
void    *Z_ReservedAllocz( size_t size ) q_malloc;
char    *Z_ReservedCopyString( const char *in ) q_malloc;

#define Z_Malloc( size )    Z_TagMalloc( size, TAG_GENERAL )
#define Z_Mallocz( size )    Z_TagMallocz( size, TAG_GENERAL )
#define Z_Reserve( size )   Z_TagReserve( size, TAG_GENERAL )
#define Z_CopyString( string )   Z_TagCopyString( string, TAG_GENERAL )

extern	cvar_t	*developer;
extern	cvar_t	*dedicated;
extern	cvar_t	*host_speeds;

extern	cvar_t	*mvd_running;
extern	cvar_t	*sv_running;
extern	cvar_t	*sv_paused;
extern	cvar_t	*cl_running;
extern	cvar_t	*cl_paused;
extern	cvar_t	*com_timedemo;
extern	cvar_t	*com_sleep;

extern	FILE *log_stats_file;

// host_speeds times
extern	int		time_before_game;
extern	int		time_after_game;
extern	int		time_before_ref;
extern	int		time_after_ref;

extern uint32	com_eventTime; /* system time of the last event */
extern uint32   com_localTime; /* milliseconds since Q2 startup */
extern uint32	com_framenum;
extern qboolean com_initialized;

extern fileHandle_t	com_logFile;

void Qcommon_Init( char *commandLine );
void Qcommon_Frame( void );
void Qcommon_Shutdown( qboolean fatalError );

// this is in the client code, but can be used for debugging from server
void SCR_DebugGraph (float value, int color);

/*
==============================================================

NON-PORTABLE SYSTEM SERVICES

==============================================================
*/

// loads the dll and returns entry pointer
void    *Sys_LoadLibrary( const char *path, const char *sym, void **handle );
void	Sys_FreeLibrary( void *handle );
void    *Sys_GetProcAddress( void *handle, const char *sym );

int		Sys_Milliseconds( void );
uint32	Sys_Realtime( void );
char	*Sys_GetClipboardData( void );
void	Sys_SetClipboardData( const char *data );

void	Hunk_Begin( mempool_t *pool, int maxsize );
void	*Hunk_Alloc( mempool_t *pool, int size );
void	Hunk_Free( mempool_t *pool );

void	Sys_Init( void );
void	Sys_FillAPI( sysAPI_t *api );
void    Sys_AddDefaultConfig( void );

void    Sys_RunConsole( void );
void	Sys_ConsoleOutput( const char *string );
void	Sys_Error( const char *error, ... ) q_noreturn q_printf( 1, 2 );
void	Sys_Quit( void );

char	**Sys_ListFiles( const char *path, const char *extension, uint32 flags, int *numFiles );
void	Sys_FreeFileList( char **list );

void	Sys_Mkdir( const char *path );
qboolean Sys_RemoveFile( const char *path );
qboolean Sys_RenameFile( const char *from, const char *to );
qboolean Sys_GetFileInfo( const char *path, fsFileInfo_t *info );

char	*Sys_GetCurrentDirectory( void );

void	Sys_DebugBreak( void );

#ifdef USE_ANTICHEAT
qboolean Sys_GetAntiCheatAPI( void );
#endif

extern cvar_t   *sys_basedir;
extern cvar_t   *sys_libdir;
extern cvar_t   *sys_refdir;
extern cvar_t   *sys_homedir;

/*
==============================================================

CLIENT / SERVER SYSTEMS

==============================================================
*/

extern	cvar_t *allow_download;
extern	cvar_t *allow_download_players;
extern	cvar_t *allow_download_models;
extern	cvar_t *allow_download_sounds;
extern	cvar_t *allow_download_maps;
extern	cvar_t *allow_download_demos;
extern	cvar_t *allow_download_other;

void CL_PumpEvents( void );
void CL_PacketEvent( neterr_t ret );
void CL_MouseEvent( int dx, int dy );
void CL_Init (void);
void CL_Disconnect( comErrorType_t type, const char *text );
void CL_Shutdown (void);
void CL_Frame (int msec);
void Con_Print( const char *text );
void Con_Printf( const char *fmt, ... );
void Con_SetMaxHeight( float frac );
void SCR_BeginLoadingPlaque (void);
void SCR_ModeChanged( void );
void CL_LocalConnect( void );
void CL_InputFrame( void );
void CL_AppActivate( qboolean active );
void CL_UpdateUserinfo( cvar_t *var, cvarSetSource_t source );

void	Key_Init( void );
void	Key_Event( uint32 key, qboolean down, uint32 time );
void	Key_CharEvent( int key );
void	Key_WriteBindings( fileHandle_t f );

typedef enum {
	ss_dead,			// no map loaded
	ss_loading,			// spawning level edicts
	ss_game,			// actively running
	ss_broadcast
} server_state_t;

typedef enum {
	KILL_RESTART,
	KILL_DISCONNECT,
	KILL_DROP
} killtype_t;

void SV_PacketEvent( neterr_t ret );
void SV_Init (void);
void SV_Shutdown( const char *finalmsg, killtype_t type );
void SV_Frame (int msec);
qboolean MVD_GetDemoPercent( int *percent, int *bufferPercent );




#ifndef DISCORD_RPC_MINIMAL_H
#define DISCORD_RPC_MINIMAL_H

typedef struct DiscordUser {
    const char* userId;
    const char* username;
    const char* discriminator;
    const char* avatar;
} DiscordUser;

typedef void (*DiscordReadyHandler)(const DiscordUser* connectedUser);
typedef void (*DiscordDisconnectedHandler)(int errcode, const char* message);
typedef void (*DiscordErrorHandler)(int errcode, const char* message);
typedef void (*DiscordJoinGameHandler)(const char* joinSecret);
typedef void (*DiscordSpectateGameHandler)(const char* spectateSecret);
typedef void (*DiscordJoinRequestHandler)(const DiscordUser* request);

typedef struct DiscordEventHandlers {
    DiscordReadyHandler ready;
    DiscordDisconnectedHandler disconnected;
    DiscordErrorHandler errored;
    DiscordJoinGameHandler joinGame;
    DiscordSpectateGameHandler spectateGame;
    DiscordJoinRequestHandler joinRequest;
} DiscordEventHandlers;

#define DISCORD_REPLY_NO 0
#define DISCORD_REPLY_YES 1
#define DISCORD_REPLY_IGNORE 2

void Discord_Initialize(const char* applicationId, DiscordEventHandlers* handlers, int autoRegister, const char* optionalSteamId);
void Discord_Shutdown(void);
void Discord_RunCallbacks(void);
void Discord_UpdatePresence(void* presence);
void Discord_ClearPresence(void);
void Discord_Respond(const char* userid, int reply);

typedef struct DiscordRichPresence {
    const char* state;
    const char* details;
    long long startTimestamp;
    long long endTimestamp;
    const char* largeImageKey;
    const char* largeImageText;
    const char* smallImageKey;
    const char* smallImageText;
    const char* partyId;
    int partySize;
    int partyMax;
    const char* matchSecret;
    const char* joinSecret;
    const char* spectateSecret;
    int instance;
} DiscordRichPresence;

#endif

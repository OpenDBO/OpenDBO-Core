# OpenDBO

Dragon Ball Online client, server and tools: the full source of the game client (x64), the server cluster that runs it, and the editors used to build its content.

All copyrighted material belongs to its owners. This is a non-profit fan project for game preservation.

## Contents

- [What's in the repository](#whats-in-the-repository)
- [Getting started](#getting-started)
  - [Compiling the client](#compiling-the-client)
  - [Setting up the server](#setting-up-the-server)
  - [Seeding test accounts](#seeding-test-accounts)
  - [Using the DBOLauncher tool](#using-the-dbolauncher-tool)
- [GM commands](#gm-commands)
- [Acknowledgements](#acknowledgements)

## What's in the repository

| Folder | Contents |
|---|---|
| `DboClient/` | The game client (`Client`), its libraries (presentation, simulation, GUI, sound) and the content tools (`Tool/`: world, model, GUI and quest editors, PackEditor, DBOLauncher). `DboClient.sln` builds them. |
| `DboClient/DragonBall/` | Where the client's game files go (not tracked). The built `Client.exe` runs from here. |
| `DboServer/Server/` | The servers: `AuthServer`, `CharServer`, `ChatServer`, `GameServer` (one process per channel), `MasterServer` and `QueryServer` (the only one that talks to the database). `DboServer.sln` builds them. |
| `DboServer/Database/` | `dbo_acc.sql`, `dbo_char.sql` and `dbo_log.sql` create the three databases. `scripts/` holds update scripts and test data. |
| `DboServer/ExecutionEnv/` | Where the servers run from: `config/` (the `.ini` files), `resource/server_data/` (tables, world play scripts) and the `start_*.bat` launch scripts. |
| `DboShared/` | Code both sides share: packet definitions, game tables, triggers. |
| `NtlLib/` | The engine's base libraries (network, database, utilities). |

All required third party tools are in [our 3rd party repository](https://github.com/OpenDBO/OpenDBO-3rdParty/releases).

## Getting started

### Compiling the client

<details>
 <summary>Step by step</summary>

1. **Download / install the necessary files:**
    - DirectX 9.
        - If you get error S1023, run these as administrator:
            ```
            MsiExec.exe /passive /X{F0C3E5D1-1ADE-321E-8167-68EF0DE699A5}
            MsiExec.exe /passive /X{1D8E6291-B0D5-35EC-8441-6616F567A0F7}
            ```
    - The 3rd party tools from [our 3rd party repository](https://github.com/OpenDBO/OpenDBO-3rdParty/releases).
    - The client files from the [Assets repository](https://github.com/OpenDBO/OpenDBO-Assets). Get the Base Client from its Releases section, then replace the files that the repository itself has newer versions of.
    - The v142 build tools (C++ MFC), through the Visual Studio Installer.

2. **Extract Xtreme ToolkitPro and the GFx SDK 3.3** somewhere.

3. **Set the environment variables:**
   - Open the Control Panel, go to System and Security, and click System.
   - Search for "System Environment Variables" and open "Edit the system environment variables".
   - Click "Environment Variables...".
   - Under "System Variables", click "New...":
     - Name `GFXSDK_DIR`, and use "Browse Directory..." to pick the GFx SDK 3.3 folder.
     - Do the same with the name `XtremeToolkitPro_Dir` for Xtreme ToolkitPro.

4. **In the GFx SDK 3.3 folder**, go to `GFx SDK 3.3\3rdParty\jpeg-6b\gfx_projects\Win32` and rename `Msvc80` to `Msvc142`.

5. **Open `Msvc142\libjpeg.sln`.**

6. **Set the solution configuration to Release / Win32** if it isn't already.

7. **Configure the libjpeg project:**
    - Right-click libjpeg → Properties, and set Windows SDK Version to the latest (10.0).
    - Change `Msvc80` to `Msvc142` in both Output Directory and Intermediate Directory.
    - Set Platform Toolset to v142.
    - Right-click libjpeg → Build.

8. **Put the client files in `OpenDBO-Core\DboClient\DragonBall`.** The client archive contains a `DragonBall` folder: copy its contents into that path.

9. **Open `OpenDBO-Core\DboClient\DboClient.sln`.**

10. **In Solution Explorer, open `DBO\Client.vcxproj` and repeat step 7 for it.**

11. **Unload the tool projects:** expand the `Tool` solution folder, then right-click each tool project (2DParticleEditor, DTEditor, GUIEditor, GuiDevTool, ModelTool2, NaviTool, NtlWE, PackEditor, SkillCustomizeViewer, TSEvtMaker, TSTool, Venus, WorldEditor, WorldPatchProcess) and choose **Unload project**. The client doesn't need them and they can break the build.

12. **Right-click `Client.vcxproj` → Build.** The only error should be a missing `libjpeg.lib`.

13. **Fix the library paths:** right-click `Client.vcxproj` → Properties → Linker → General → Additional Library Directories, and point these at your system:
    - `$(GFXSDK_DIR)\Lib\$(PlatformName)\Msvc80\Release`
    - `$(GFXSDK_DIR)\3rdParty\jpeg-6b\lib\$(PlatformName)\Msvc142\Release`

14. **Right-click `Client.vcxproj` → Rebuild.** The client is now built.

The RenderWare libraries (the `Renderware` solution folder) build automatically: building the client first builds any of them that are missing or out of date, so the first build takes longer. The list of libraries the client waits for is `DboClient\Renderware\Renderware.References.props`.

</details>

### Setting up the server

<details>
 <summary>Step by step</summary>

1. **Requirements:** Windows 10 and 8 GB of RAM.

2. **Install [Visual Studio 2019](https://my.visualstudio.com/Downloads?q=visual%20studio%202019)** (the free Community edition):
    - Under "Workloads", install "Desktop development with C++" and "Game development with C++".
    - Under "Individual components", install "C++ Clang-cl for v142 build tools (x64/x86)".

3. **Get the code:** download the [OpenDBO-Core repository](https://github.com/OpenDBO/OpenDBO-Core) (Code → Download ZIP) and extract it to a folder named `OpenDBO-Core`.

4. **Set up the 3rd party files:** do steps 1 and 2 of the client guide if you haven't yet.

5. **Build the servers:**
    - Open `OpenDBO-Core\DboServer\DboServer.sln`.
    - Select **Release** and **x64**.
    - In Solution Explorer, under `Server`, right-click each server (AuthServer, CharServer, ChatServer, GameServer, MasterServer, QueryServer) and choose Build.

6. **Install a database.** XAMPP is used below. You can install MariaDB directly instead and skip the XAMPP-specific steps.
    - Disable User Account Control: search for `msconfig`, open the Tools tab and select "Disable UAC".
    - Install [XAMPP](https://www.apachefriends.org/index.html).
    - Open `C:/xampp/apache/conf/httpd.conf` in Notepad: change the port to `Listen 8080`, and `ServerName localhost:` to `ServerName localhost:8080`.
    - In the XAMPP Control Panel, start Apache and MySQL.
    - Make sure your antivirus allows XAMPP.

7. **Create the databases** at `localhost:8080/phpmyadmin/`:
    - Click "New", create a database named `dbo_acc`, click Import and select `OpenDBO-Core/DboServer/Database/dbo_acc.sql`. Click Start.
    - Do the same for `dbo_char` and `dbo_log`.
    - In `dbo_acc` → `accounts`, click Insert and fill in a username.
    - For `password_hash`, put the SHA3-256 hash of your password (for example from [this generator](https://emn178.github.io/online-tools/sha3_256.html)).
    - For GM rights, see [GM commands](#gm-commands). In short: set `is_gm` to `10` on the account and `game_master` to `1` on the character.
    - Or import the seed script for 12 ready-to-play GM accounts (see [Seeding test accounts](#seeding-test-accounts)).

8. **Edit the server configs** in `OpenDBO-Core/DboServer/ExecutionEnv/config`: open `AuthServer.ini`, `CharServer.ini` and the others, make sure every IP is `127.0.0.1`, and set the database password (`test` with the XAMPP defaults above).

9. **Start the servers** from `OpenDBO-Core/DboServer/ExecutionEnv`, in this order:
    1. `start_master_server.bat`
    2. `start_query_server.bat`, and wait until it's done. A missing `MSVCP120.dll` error is fixed by installing the Microsoft Visual C++ 2013 Redistributable.
    3. `start_char_server_0.bat`
    4. `start_auth_server.bat`
    5. `start_channel_0.bat`, and wait until it has finished loading (this can take a while; you may need to press Enter). Then close it.
    6. `start_chat_server.bat`, and wait until it's done.
    7. `start_channel_0.bat` again, `start_channel_1.bat` if you want a second channel, and `start_channel_9.bat`.

10. **Connect the client:** in the client folder, open `ConfigOptions.xml`, set every IP to `127.0.0.1` and save. Start the client and log in.

There's also a video of this process: https://www.youtube.com/watch?v=lWDffP81ACw

</details>

### Seeding test accounts

<details>
 <summary>12 ready-to-play GM accounts</summary>

`DboServer/Database/scripts/seeds.sql` creates 12 GM accounts for local testing, each with one level 70 character of a different master class. **Only run it on local/dev databases.**

1. **Import it** once `dbo_acc` and `dbo_char` exist, either from phpMyAdmin (Import → `seeds.sql`) or from the command line:
    ```
    mysql -u root -p < DboServer/Database/scripts/seeds.sql
    ```
2. **Restart QueryServer and ChatServer** so they pick up the new characters, items and guilds.
3. **Log in** with `admin1` … `admin12`, password `password`.

| Account | Character | Account | Character |
|---|---|---|---|
| admin1 | Fighter | admin7 | DendePriest |
| admin2 | Swordsman | admin8 | PokoPriest |
| admin3 | CraneHermit | admin9 | UltimateMajin |
| admin4 | TurtleHermit | admin10 | GrandChefMajin |
| admin5 | DarkWarrior | admin11 | PlasmaMajin |
| admin6 | ShadowKnight | admin12 | KarmaMajin |

**Every account** has full GM access (`admin` and `is_gm` = 10, character `game_master` = 1).

**Every character** has:
- **Gear:** Dark Soul armor set +15 (physical for fighter classes, energy for mystic classes), Flame weapon and sub weapon +15, Corona necklace and 2 Corona earrings, Octopus Alpha Defense and Alpha Attack rings, Perfect Scouter.
- **Bags:** the Basic Capsule Kit plus an HL Capsule Box Z32 in the other 4 bag slots.
- **Skills:** starting class skills, TLQ passives (Power Up, Guard, Dash, Counter Attack), Flight, master class mastery, race transformation and all 6 Dragon King's buffs. 69 SP left to train the rest.
- **Currency:** 1,000,000,000 Zeni, 10,000 Tokens and 10,000 WP.
- **Unlocks:** every teleport point, the whole warfog and every title.
- **Position:** spawn and bind point in Kokkara Village.

**Guilds:** two guilds with every guild and dojo function unlocked, 1,000,000 guild points and 1,000,000,000 Zeni in the guild bank:
- `OpenDBO`: admin1 … admin6, master admin1
- `OpenDBO2`: admin7 … admin12, master admin7

The script is safe to re-run: accounts are updated in place, and the seeded characters (ids `900001`–`900012`), their items (ids from `900000000`) and the two guilds (ids `900001`–`900002`) are wiped and recreated. Anything those characters did in game is lost on re-run.

</details>

### Using the DBOLauncher tool

<details>
 <summary>Building and running the launcher</summary>

`DBOLauncher` (`DboClient/Tool/DBOLauncher`) is the game's launcher: it can check a patch server for updates, then starts `Client.exe`. It's part of `DboClient.sln` (`Release4Tools|Win32`).

1. **Build it:** in `DboClient.sln`, right-click `DBOLauncher` under the `Tool` solution folder and build it as `Release4Tools|Win32`. The post-build step copies `LauncherConfig.xml`, `launcherdevmaster.xml`, `patchw32.dll` and the `patch/` folder from `Tool/DBOLauncher/Test/` into `DragonBall/`.

2. **Offline mode (default):** `Test/launcherdevmaster.xml` ships with `OFFLINE_MODE_ENABLE` set to `1`, so the launcher skips the patch server and goes straight to its idle screen, where **Start** launches `Client.exe`. No server setup needed; this is the quickest way to launch the client.

3. **Online mode (testing the real patch flow):** set `OFFLINE_MODE_ENABLE` to `0` in `Test/launcherdevmaster.xml` and rebuild. This needs a reachable HTTP patch server on **port 80** (hardcoded in the launcher). The server has to serve `Test/LauncherConfig.xml` (which points at it) and `Test/patch/` (`PatchServerConfig.XML` + `PatchList.xml`). These ship declaring "nothing to patch", so the launcher reports the client as up to date and goes to the idle screen. How you host them is up to you.

</details>

## GM commands

Type a command in the game chat, starting with `@`. Arguments are separated by spaces; `[ ]` marks an optional one.

### Who can use them

Every command has a level. A character can use a command when both of these hold:

- **The account's `is_gm`** (`dbo_acc.accounts`) is at least the command's level.
- **The character's `game_master`** (`dbo_char.characters`) is `1`. This is needed for every command above level 0.

| Level | Name |
|---|---|
| 0 | Everyone |
| 2 | Game master |
| 3 | Community manager |
| 4 | Dev server |
| 10 | Admin |

For full access, set `is_gm = 10` on the account and `game_master = 1` on the character, then log in again. The seeded test accounts already have this.

Most commands act on the GM's own character and the channel the GM is on. The ones that take a character name only find characters online on the same channel, unless the table says otherwise.

### Character

| Command | Level | What it does |
|---|---|---|
| `@setlevel LEVEL` | 10 | Sets your level and gives the SP for the levels gained. |
| `@setclass CLASS` | 10 | Changes your class (see [class ids](#class-ids)). |
| `@setadult 0\|1` | 2 | Switches between the child (`0`) and adult (`1`) model. |
| `@startgm CLASS LEVEL` | 2 | For a base-class character: becomes master class `CLASS` at level `60` or `70`, and gets that class's master passive, passive item and flight item. |
| `@addmasteritem CLASS` | 0 | For a base-class character: gives the class change item for master class `CLASS`. |
| `@addskill SKILL_ID` | 10 | Learns a skill. |
| `@addskill2` | 0 | Learns your master class passive if you're missing it. |
| `@addhtb [HTB_ID]` | 0 | Players get their class's HTB skill. A GM learns `HTB_ID`. |
| `@removeskillscd` | 10 | Resets every skill cooldown. |
| `@r` | 4 | Fully restores your LP and EP. |
| `@god` | 10 | Toggles god mode: immortal, maximum attack and defence, fast attacks and run speed 25. |
| `@invincible [SECONDS]` | 10 | Makes you invincible for `SECONDS` (default and maximum 3600). |
| `@setspeed SPEED` | 2 | Sets your walk and run speed. |
| `@hide` | 2 | Toggles invisibility. |
| `@exp on\|off` | 0 | Turns gaining EXP on or off. |
| `@resetexp` | 0 | Sets your current EXP to 0. |
| `@addtitle TITLE_ID [CHAR_ID]` | 2 | Gives a title to you, or to the online character with id `CHAR_ID`. |
| `@deltitle TITLE_ID [CHAR_ID]` | 2 | Removes a title from you, or from the online character with id `CHAR_ID`. |
| `@petexp [AMOUNT]` | 10 | Adds `AMOUNT` EXP to your summoned pet, capped at what its rank needs. With no amount, fills it up so it can be fused. |

### Items and currency

| Command | Level | What it does |
|---|---|---|
| `@additem ITEM_ID [COUNT]` | 10 | Puts an item in your bag (`COUNT` up to the item's stack size, default 1). |
| `@createloot ITEM_ID COUNT` | 10 | Drops `COUNT` (up to 100) of an item on the ground around you. |
| `@setzenny AMOUNT` | 10 | **Adds** `AMOUNT` zeni. |
| `@addmudosa AMOUNT [NAME]` | 10 | Adds mudosa points to you or to the character `NAME`. |
| `@upgrade GRADE` | 10 | Sets the upgrade grade (0–15) of all your equipped items. |
| `@setitemrank RANK` | 10 | Sets the rank of all your equipped items (1 normal, 2 superior, 3 excellent, 4 rare, 5 legendary). |
| `@setitemduration SECONDS` | 10 | Sets the time left on the timed item in your first bag slot. |
| `@delallitems` | 2 | **Deletes every item you carry.** |

### Event rewards (event manager NPC)

A reward belongs to a whole account: any one character on it can take it once at the event manager NPC ("Event Reward"). These three work across channels and for offline players, because they write to the database. An account that already has the reward is left alone, so running a command twice doesn't give it twice.

| Command | Level | Gives the reward to |
|---|---|---|
| `@eventreward TBLIDX NAME` | 10 | The account of character `NAME`, online or offline. |
| `@eventrewardonline TBLIDX` | 10 | The accounts of every character online on this channel. |
| `@eventrewardall TBLIDX` | 10 | Every account. |

`TBLIDX` is a reward event from `table_event_system_data` (type 3):

| TBLIDX | Reward | Item |
|---|---|---|
| 10026 | Krillin Founder Pack | 11190002 |
| 10027 | Goku Founder Pack | 11190003 |
| 10028 | Super Saiyan Founder Pack | 11190004 |

```
@eventreward 10026 Fighter
@eventrewardonline 10027
@eventrewardall 10028
```

You get a confirmation in chat. Online receivers on your channel are told to visit the event manager, and their next visit shows the reward.

### Movement

| Command | Level | What it does |
|---|---|---|
| `@go X Y Z` | 2 | Teleports you to a position in the current world. |
| `@teleport PLACE` | 2 | Teleports you to a fixed place (see [teleport places](#teleport-places)). |
| `@world WORLD_ID` | 2 | Teleports you to a world's default spawn, creating the world if needed. |
| `@warp NAME` | 2 | Teleports you to a character. |
| `@call NAME` | 2 | Teleports a character to you. Doesn't work for dungeons (`@warp` either). |
| `@bind` | 2 | Makes your current position your bind point. |
| `@unstuck` | 0 | Sends you to your bind point. Not while fainted, fighting, in a PvP zone, a dungeon or a free battle, carrying a scramble dragonball, on the dojo channel, or while the call-back skill is on cooldown. |
| `@warfog` | 2 | Reveals the whole map of the current world. |

### Monsters and NPCs

| Command | Level | What it does |
|---|---|---|
| `@addmob MOB_ID` | 10 | Spawns a monster next to you. |
| `@addmobgroup GROUP_ID` | 10 | Spawns a monster spawn group of the current world at its table positions. |
| `@addnpc NPC_ID` | 10 | Spawns an NPC next to you. |
| `@purge` | 3 | Kills every monster around you. |

### Players and moderation

| Command | Level | What it does |
|---|---|---|
| `@notice TYPE TEXT` | 2 | Sends a server notice to every channel. `TYPE`: 0 chat, 1 screen, 2 chat and screen, 3 from the Tori NPC, 4 caution, 5 territory. |
| `@pm NAME TEXT` | 2 | Sends a message to one character, shown by the Tori NPC. |
| `@mute NAME MINUTES REASON` | 2 | Mutes a character's chat. |
| `@unmute NAME` | 2 | Removes a mute. |
| `@dc NAME` | 2 | Disconnects a character. |
| `@kill NAME` | 10 | Makes a character faint. |
| `@bann NAME DAYS REASON` | 2 | Bans the account of an online character. `DAYS` 255 is permanent. |
| `@dbann ACCOUNT_ID DAYS REASON` | 2 | Bans an account by id, online or not. `DAYS` 255 is permanent. |
| `@cancelah CHAR_ID` | 2 | Cancels every auction house listing of a character. |
| `@deleteguild NAME` | 3 | Deletes a guild. |

### Server events

| Command | Level | What it does |
|---|---|---|
| `@starthoneybee HOURS` | 3 | Starts the honey bee event. |
| `@stophoneybee` | 3 | Stops the honey bee event. |
| `@fairyevent [start [HOURS] \| stop]` | 10 | Shows the fairy event state, starts it (default 3 hours, max 12) or stops it. |
| `@dynamicfield [add COUNT \| start \| stop]` | 10 | Shows the world boss event state, adds `COUNT` kills to its counter (all channels), or spawns or ends the bosses (this channel). |
| `@dbscramble [start \| stop]` | 10 | Shows, starts (2 hours, scramble channel only) or stops the dragonball scramble. |
| `@dojowar [start \| next \| stop]` | 10 | Shows the dojo war state, opens every owned dojo for challenges now, moves every war to its next step, or ends the event. |
| `@shutdown` | 10 | Starts the server shutdown countdown. |

### Reference

#### Class ids

| Id | Class | Id | Class | Id | Class |
|---|---|---|---|---|---|
| 0 | Human Fighter | 7 | Street Fighter | 14 | Shadow Knight |
| 1 | Human Mystic | 8 | Swordsman | 15 | Dende Priest |
| 2 | Human Engineer | 9 | Crane Hermit | 16 | Poko Priest |
| 3 | Namekian Warrior | 10 | Turtle Hermit | 17 | Ultimate Majin |
| 4 | Namekian Dragon Clan | 11 | Gun Mania | 18 | Grand Chef Majin |
| 5 | Mighty Majin | 12 | Mech Mania | 19 | Plasma Majin |
| 6 | Wonder Majin | 13 | Dark Warrior | 20 | Karma Majin |

`@startgm` and `@addmasteritem` take the master class id (7–20) that the character's base class can become.

#### Teleport places

| Id | Place | Id | Place |
|---|---|---|---|
| 0 | Yahoi | 10 | Mermaid |
| 1 | Yureka | 11 | Gannet |
| 2 | Dalpang | 12 | Emerald |
| 3 | Dragon | 13 | Tembarin |
| 4 | Baee | 14 | Cell |
| 5 | Ajirang | 15 | Buu |
| 6 | Karinga 1 | 16 | CC |
| 7 | Karinga 2 | 17 | Mushroom |
| 8 | Great Tree | 18 | Papaya |
| 9 | Karinga 3 | | |

## Acknowledgements

- **DBOG**, whose source code this project started from.
- **The [OpenDBO](https://github.com/OpenDBO) contributors**, who keep it going: the x64 port, the fixes, and the features ported from the retail clients.

If you use this work, credit the OpenDBO project.

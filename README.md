# OpenDBO
DBO Client, Server and Tools software.

## Setting everything up
All required third party tools can be obtained from [our 3rd party repository](https://github.com/OpenDBO/OpenDBO-3rdParty/releases).
<details>
 <summary>Compiling the Client</summary>

1. **Download / Install the necessary files:**
    - DirectX9.
        - In case of encountering error S1023, execute the following commands as administrator:
            ```
            MsiExec.exe /passive /X{F0C3E5D1-1ADE-321E-8167-68EF0DE699A5}
            MsiExec.exe /passive /X{1D8E6291-B0D5-35EC-8441-6616F567A0F7}
            ```
    - 3rd party tools that you downloaded from [our 3rd party repository](https://github.com/OpenDBO/OpenDBO-3rdParty/releases).
    - Client files: Download them from the [Assets repository](https://github.com/OpenDBO/OpenDBO-Assets). First get the Base Client from the Releases section and then replace all needed files with the updated ones on the repo itself.
    - Download v142 build tools (C++ MFC) via Visual Studio Installer.

2. **Extract Xtreme Toolkitpro and GFx SDK 3.3 somehwere**

3. **Set up environment variables:**
   - Open the Control Panel, go into System and Security, and click on System
   - In the search bar, type in "System Environment Variables" and open "Edit the system environment variables"
   - In the System Properties window that opened, click "Environment Variables..."
   - In the "System Variables" section, click on "New..."
     - In "Variable name:", write "GFXSDK_DIR"
     - Click "Browse Directory..." and locate where you extracted the GFx SDK 3.3 folder.
     - Do the same for Xtreme Toolkitpro except in "Variable name:" write "XtremeToolkitPro_Dir" 

4. **Access the GFx SDK 3.3 folder you just moved:**
    - Navigate to `GFx SDK 3.3\3rdParty\jpeg-6b\gfx_projects\Win32`
    - Rename the folder named "Msvc80" to "Msvc142."

5. **Enter the Msvc142 folder and open the project:** "libjpeg.sln"

6. **Ensure the solution configuration in VS 2019 is set to:** "Release and Win32." If not, adjust it.

7. **Configure the libjpeg project settings:**
    - Right-click on libjpeg, go to Properties, and under Windows SDK Version, set it to the latest (10.0).
    - Change both Output Directory and Intermediate Directory's Msvc80 to Msvc142.
    - Change the Platform Toolset to (v142).
    - Right-click on libjpeg -> Build.

8. **Navigate to folder:** `source\repos\OpenDBO-Core\DboClient\DragonBall` and extract the client's contents.
    - The downloaded RAR file should contain a folder named DragonBall; copy and paste its contents into the specified path.
    - This RAR file is from the "Client" link provided above.

9. **Go to folder:** `source\repos\OpenDBO-Core\DboClient` and open "DboClient.sln"

10. **In Solution Explorer, go to DBO\Client.vcxproj and repeat step 7.**

11. **In Solution Explorer, navigate to Tools\2DParticleEditor, right-click, and unload it.**

12. **Right-click on Client.vcxproj -> Build**
    - You should encounter only one error after compiling: 'libjpeg.lib'

13. **Right-click on Client.vcxproj then go to properties:**
    - Navigate to Linker -> General -> Additional Library Directories.
    - Update the paths to match your system: 
        - `$(GFXSDK_DIR)\Lib\$(PlatformName)\Msvc80\Release`
        - `$(GFXSDK_DIR)\3rdParty\jpeg-6b\lib\$(PlatformName)\Msvc142\Release`

14. **Right-click on Client.vcxproj -> Rebuild**

15. **The Client should have compiled successfully.**

</details>

<details>
 <summary>Setting up the Server</summary>

1. **Requirements:**
    - Windows 10
    - 8 GB of RAM

2. **Download and install Visual Studio 2019:**
    - [Visual Studio 2019](https://my.visualstudio.com/Downloads?q=visual%20studio%202019)
    - Select the "Community - Free Download" option to obtain the installer app.
    - Open the installer app and follow these steps:
        - Navigate to the "Workloads" section and install both "Desktop development with C++" and "Game development with C++."
        - In the "Individual components" section, install "C++ Clang-cl for v142 build tools (x64/x86)."
    - Click "Install while downloading" and wait for the process to complete.

3. **Download and extract the OpenDBO-Core Repository:**
    - [OpenDBO-Core Repository](https://github.com/OpenDBO/OpenDBO-Core). Click "Code" -> Download ZIP.
    - Extract the ZIP file to the main folder named "OpenDBO-Core."

4. **Download and install/extract additional programs: it's recommended to set up the Client repository now. Otherwise perform steps 1 and 2 from the Client guide (if you already did it you can skip this step)**

5. **Compile the private server using Visual Studio 2019:**
    - Navigate to the "OpenDBO-Core" main folder, then to the "Dboserver" subfolder.
    - Double-click "Dboserver.sln" to open it in Visual Studio 2019.
    - Ensure "Release" and "x64" are selected from the top bar.
    - In the "Solution Explorer," under "Server," right-click each server (AuthServer, CharServer, etc.) and click "Build" one by one.
    - Wait for the server executable files to finish compiling.

6. **Download and install XAMPP (you can also install the latest version of MariaDB directly, if so, you can skip this step and some steps of 7.):**
    - Deactivate User Account Control (UAC) by searching for "msconfig" in Windows, accessing the "tools" tab, and selecting "Disable UAC."
    - Download and install XAMPP from [XAMPP](https://www.apachefriends.org/index.html)
    - Navigate to "C:/xampp/apache/conf/httpd.conf" in Explorer.
    - Open "httpd.conf" in Notepad, change the port to "Listen 8080," and change "ServerName localhost:" to "ServerName localhost:8080."
    - In the XAMPP Control Panel, start "Apache" and "MySQL."
    - Ensure your antivirus accepts XAMPP.

7. **Open your browser and type: "localhost:8080/phpmyadmin/"**
    - Click "New" in the left column.
    - Add a database named "dbo_acc" and click "make."
    - Click "import" at the top, choose file, and select "dbo_acc.sql" from "OpenDBO-Core/DboServer/Database."
    - Click "Start" at the bottom.
    - Repeat this for "dbo_char" and "dbo_log."
    - Within "dbo_acc," find "accounts," click "insert," and fill in a username and password.
    - Generate an MD5 Hash for the password using [MD5 Hash Generator](https://www.md5hashgenerator.com), paste it next to "Password_hash," and click "Start" at the bottom.
    - If you want GM permissions make sure to set `admin` and `isgm` to `10` in the `accounts` table and `gamemaster` to `10` in the `characters` tabel from the `dbo_char` database.

8. **Change the server .ini files:**
    - Navigate to "OpenDBO-Core/DboServer/ExecutionEnv/Config."
    - Open "AuthServer.ini," "CharServer.ini," etc., with Notepad.
    - Ensure all IP addresses are "127.0.0.1" and change the password to "test."

9. **Run the server executables:**
    - Go to "OpenDBO-Core/DboServer/ExecutionEnv."
    - Run `start_master_server.bat`
    - Run `start_query_server.bat` and wait until its done (if you get missing `MSVCP120.dll` error you can fix it by installing Microsoft Visual C++ Redistributable 2013).
    - Run `start_char_server_0.bat`
    - Run `start_auth_server.bat`
    - Run `start_channel_0.bat` and wait until it's finished (can take a lot of time, might need to press ENTER). Then close it.
    - Run `start_chat_server.bat` and wait until its done.
    - Run `start_channel_0.bat` again, `start_channel_1.bat` (if you need 2 channels) and `start_channel_9.bat`

10. **Download and extract the DBO Client Files:**
    - Development Client/Server Access
    - In the main folder, locate "ConfigOptions.xml," change the IP to "127.0.0.1" on each line, and save.
    - Run the client, enter your username and password, and you’re ready to go.

</details>
Alternatively there's a video showcasing this process: https://www.youtube.com/watch?v=lWDffP81ACw

## Acknowledgements
All and any copyrighted material belongs to their respective owners, this is just a non-profit fan project aiming for game preservation. Thanks to DBOG for providing the base for this source code.

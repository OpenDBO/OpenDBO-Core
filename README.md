# OpenDBO
DBO Client, Server and Tools software.

# Setting everything up
<details>
 <summary>Compiling the Client</summary>

1. **Download the necessary files:**
    - DirectX9: [Download Link](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
        - In case of encountering error S1023, execute the following commands:
            ```
            MsiExec.exe /passive /X{F0C3E5D1-1ADE-321E-8167-68EF0DE699A5}
            MsiExec.exe /passive /X{1D8E6291-B0D5-35EC-8441-6616F567A0F7}
            ```
    - Tools: [Google Drive Link](https://drive.google.com/drive/folders/1eITPtDifloOwp1q63H2HEeau6UqGNdfD?usp=sharing)
    - Client: [Google Drive Link](https://drive.google.com/drive/folders/1eITPtDifloOwp1q63H2HEeau6UqGNdfD?usp=sharing)
    - Download v142 build tools (C++ MFC) via Visual Studio Installer

2. **Navigate to the following path:** `source\repos\OpenDBO-Core` and extract the Tools there.
    - The RAR file should contain a folder named "3rd party."
    - Extract the RAR files inside "3rd Party" and keep the Xtreme Toolkit within it.

3. **Move the GFx SDK 3.3 folder into:** `source\repos\OpenDBO-Core\DBOClient\Lib\NtlFlasher`

4. **Access the GFx SDK 3.3 folder you just moved:**
    - Navigate to `source\repos\OpenDBO-Core\DBOClient\Lib\NtlFlasher\GFx SDK 3.3\3rdParty\jpeg-6b\gfx_projects\Win32`
    - Rename the folder named "Msvc80" to "Msvc142."

5. **Enter the Msvc142 folder and open the project:** "libjpeg.sln"

6. **Ensure the solution configuration in VS 2019 is set to:** "Release and Win32." If not, adjust it.

7. **Configure the libjpeg project settings:**
    - Right-click on libjpeg, go to Properties, and under Windows SDK Version, set it to the latest (10.0).
    - Change the Platform Toolset to (v142).
    - Right-click on libjpeg -> Build.

8. **Navigate to folder:** `source\repos\OpenDBO-Core\DboClient\DragonBall` and extract the client's contents.
    - The downloaded RAR file should contain a folder named DragonBall; copy and paste its contents into the specified path.
    - This RAR file is from the "Client" link provided above.

9. **Go to folder:** `source\repos\OpenDBO-Core\DboClient" and open "DboClient.sln"`

10. **In Solution Explorer, go to DBO\Client.vcxproj and repeat step 7.**

11. **In Solution Explorer, navigate to Tools\2DParticleEditor, right-click, and unload it.**

12. **Right-click on Client.vcxproj -> Build**
    - You should encounter only one error after compiling: 'libjpeg.lob'

13. **Right-click on Client.vcxproj then go to properties:**
    - Navigate to Linker -> General -> Additional Library Directories.
    - Update the paths to match your system: 
        - `$(SolutionDir)Lib\NtlFlasher\GFx SDK 3.3\Lib\$(PlatformName)\Msvc80\Release`
        - `$(SolutionDir)Lib\NTlFlasher\GFx SDK 3.3\3rdParty\jpeg-6b\lib\$(PlatformName)\Msvc142\Release`

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

4. **Download and install/extract additional programs:**
    - DirectX9 SDK: [DirectX9 SDK](https://www.microsoft.com/en-u…oad/details.aspx?id=23549)
    - GFx SDK: GFx SDK 3.3 -> Extract the "GFx SDK" folder into "OpenDBO-Core/DboClient/Lib/NtlFlasher"
    - Xtreme ToolkitPro: Xtreme ToolkitPro v15.2.1 -> Extract the folder into "OpenDBO-Core/3rdParty"

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

7. **Open your browser** and type: "localhost:8080/phpmyadmin/"
    - Click "New" in the left column.
    - Add a database named "dbo_acc" and click "make."
    - Click "import" at the top, choose file, and select "dbo_acc.sql" from "OpenDBO-Core/DboServer/Database."
    - Click "Start" at the bottom.
    - Repeat this for "dbo_char" and "dbo_log."
    - Within "dbo_acc," find "accounts," click "insert," and fill in a username and password.
    - Generate an MD5 Hash for the password using [MD5 Hash Generator](https://www.md5hashgenerator.com), paste it next to "Password_hash," and click "Start" at the bottom.
    - If you want GM permissions make sure to set `admin` and `isgm` to `10` in the `accounts` table and `gamemaster` to `10` in the `characters` tabel from the `dbo_char` database.

8. **Change the server *ini files:**
    - Navigate to "OpenDBO-Core/DboServer/ExecutionEnv/Config."
    - Open "AuthServer.ini," "CharServer.ini," etc., with Notepad.
    - Ensure all IP addresses are "127.0.0.1" and change the password to "test."

9. **Run the server executables:**
    - Go to "OpenDBO-Core/DboServer/ExecutionEnv."
    - Run "start_master_server.bat," "start_query_server.bat," "start_char_server_0.bat," "start_auth_server.bat," "start_channel_0.bat," "start_chat_server.bat," etc., waiting for each to finish.

10. **Download and extract the DBO Client Files:**
    - Development Client/Server Access
    - In the main folder, locate "Configoptions.xml," change the IP to "127.0.0.1" on each line, and save.
    - Run the client, enter your username and password, and you’re ready to go.

</details>
Alternatively there's a video showcasing this process: https://www.youtube.com/watch?v=lWDffP81ACw

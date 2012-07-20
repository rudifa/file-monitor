
!define APP_DIR "$%APP_COMPANY_NAME%\$%APP_NAME%"
!define UNINSTALL_DIR "Software\Microsoft\Windows\CurrentVersion\Uninstall\$%APP_NAME%"

SetCompressor /FINAL /SOLID lzma

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------

; The name of the installer
Name "$%APP_NAME%"

; The file to write
OutFile "$%APP_INSTALLER_NAME%"

; The default installation directory
InstallDir "$LOCALAPPDATA\${APP_DIR}"

; Registry key to check for directory (so if you install again, it will
; overwrite the old one automatically)
InstallDirRegKey HKCU "Software\${APP_DIR}" "Install_Dir"

;This will prevent UAC from asking the user for admin rights to install.
RequestExecutionLevel user

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------

;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\${APP_DIR}" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------

; Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_LICENSE "license.txt"
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH
  
  ;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English" ;first language is the default language
  !insertmacro MUI_LANGUAGE "French"
  !insertmacro MUI_LANGUAGE "German"
  !insertmacro MUI_LANGUAGE "Spanish"
  !insertmacro MUI_LANGUAGE "SpanishInternational"
  !insertmacro MUI_LANGUAGE "SimpChinese"
  !insertmacro MUI_LANGUAGE "TradChinese"
  !insertmacro MUI_LANGUAGE "Japanese"
  !insertmacro MUI_LANGUAGE "Korean"
  !insertmacro MUI_LANGUAGE "Italian"
  !insertmacro MUI_LANGUAGE "Dutch"
  !insertmacro MUI_LANGUAGE "Danish"
  !insertmacro MUI_LANGUAGE "Swedish"
  !insertmacro MUI_LANGUAGE "Norwegian"
  !insertmacro MUI_LANGUAGE "NorwegianNynorsk"
  !insertmacro MUI_LANGUAGE "Finnish"
  !insertmacro MUI_LANGUAGE "Greek"
  !insertmacro MUI_LANGUAGE "Russian"
  !insertmacro MUI_LANGUAGE "Portuguese"
  !insertmacro MUI_LANGUAGE "PortugueseBR"
  !insertmacro MUI_LANGUAGE "Polish"
  !insertmacro MUI_LANGUAGE "Ukrainian"
  !insertmacro MUI_LANGUAGE "Czech"
  !insertmacro MUI_LANGUAGE "Slovak"
  !insertmacro MUI_LANGUAGE "Croatian"
  !insertmacro MUI_LANGUAGE "Bulgarian"
  !insertmacro MUI_LANGUAGE "Hungarian"
  !insertmacro MUI_LANGUAGE "Thai"
  !insertmacro MUI_LANGUAGE "Romanian"
  !insertmacro MUI_LANGUAGE "Latvian"
  !insertmacro MUI_LANGUAGE "Macedonian"
  !insertmacro MUI_LANGUAGE "Estonian"
  !insertmacro MUI_LANGUAGE "Turkish"
  !insertmacro MUI_LANGUAGE "Lithuanian"
  !insertmacro MUI_LANGUAGE "Slovenian"
  !insertmacro MUI_LANGUAGE "Serbian"
  !insertmacro MUI_LANGUAGE "SerbianLatin"
  !insertmacro MUI_LANGUAGE "Arabic"
  !insertmacro MUI_LANGUAGE "Farsi"
  !insertmacro MUI_LANGUAGE "Hebrew"
  !insertmacro MUI_LANGUAGE "Indonesian"
  !insertmacro MUI_LANGUAGE "Mongolian"
  !insertmacro MUI_LANGUAGE "Luxembourgish"
  !insertmacro MUI_LANGUAGE "Albanian"
  !insertmacro MUI_LANGUAGE "Breton"
  !insertmacro MUI_LANGUAGE "Belarusian"
  !insertmacro MUI_LANGUAGE "Icelandic"
  !insertmacro MUI_LANGUAGE "Malay"
  !insertmacro MUI_LANGUAGE "Bosnian"
  !insertmacro MUI_LANGUAGE "Kurdish"
  !insertmacro MUI_LANGUAGE "Irish"
  !insertmacro MUI_LANGUAGE "Uzbek"
  !insertmacro MUI_LANGUAGE "Galician"
  !insertmacro MUI_LANGUAGE "Afrikaans"
  !insertmacro MUI_LANGUAGE "Catalan"
  !insertmacro MUI_LANGUAGE "Esperanto"

;--------------------------------
;Reserve Files
  
  ;If you are using solid compression, files that are required before
  ;the actual installation should be stored first in the data block,
  ;because this will make your installer start faster.
  
  !insertmacro MUI_RESERVEFILE_LANGDLL

;--------------------------------

; Installer sections

Section "$%APP_NAME% (required)" SecApplication

  SectionIn RO
 
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
 
  ; Put file there
  File "$%PROJECT_NAME%.exe"
  File "libstdc++-6.dll"
  File "libgcc_s_dw2-1.dll"
  File "mingwm10.dll"
  File "QtCore4.dll"
  File "QtGui4.dll"
  File "QtSvg4.dll"
  File "QtWebKit4.dll"
  File "QtNetwork4.dll"
  File "QtXml4.dll"
  File "QtXmlPatterns4.dll"
  File "phonon4.dll"
  File "license.txt"
  File "readme.txt"
  File "$%PROJECT_NAME%.ico"
 
  ; Write the installation path into the registry
  WriteRegStr HKCU "SOFTWARE\${APP_DIR}" "Install_Dir" "$INSTDIR"
  
  !include "FileFunc.nsh"
  ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2  ; Get size of all files in the install dir.
  IntFmt $0 "0x%08X" $0  ; Convert decimal to DWORD.
 
  ; Write the uninstall keys for Windows
  WriteRegStr HKCU "${UNINSTALL_DIR}" "DisplayName" "$%APP_NAME%"
  WriteRegStr HKCU "${UNINSTALL_DIR}" "Publisher" "$%APP_COMPANY_NAME%"
  WriteRegStr HKCU "${UNINSTALL_DIR}" "RegOwner" "$%APP_COPYRIGHT_HOLDER%"
  WriteRegStr HKCU "${UNINSTALL_DIR}" "DisplayIcon" '"$INSTDIR\$%PROJECT_NAME%.ico"'
  WriteRegStr HKCU "${UNINSTALL_DIR}" "DisplayVersion" "$%APP_VERSION%"
  WriteRegStr HKCU "${UNINSTALL_DIR}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegStr HKCU "${UNINSTALL_DIR}" "InstallLocation" "$INSTDIR"
  WriteRegStr HKCU "${UNINSTALL_DIR}" "Readme" "$INSTDIR\readme.txt"
  WriteRegStr HKCU "${UNINSTALL_DIR}" "URLInfoAbout" "$%APP_LINK%"
  WriteRegStr HKCU "${UNINSTALL_DIR}" "HelpLink" "$%APP_LINK%"
  WriteRegStr HKCU "${UNINSTALL_DIR}" "URLUpdateInfo" "$%APP_DOWNLOAD_LINK%"
  WriteRegDWORD HKCU "${UNINSTALL_DIR}" "EstimatedSize" "$0"
  WriteRegDWORD HKCU "${UNINSTALL_DIR}" "NoModify" 1
  WriteRegDWORD HKCU "${UNINSTALL_DIR}" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
 
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts (required)" SecStartMenu

   SectionIn RO

  CreateDirectory "$SMPROGRAMS\${APP_DIR}"
  CreateShortCut "$SMPROGRAMS\${APP_DIR}\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\${APP_DIR}\readme.lnk" "$INSTDIR\readme.txt" "" "$INSTDIR\readme.txt" 0
  CreateShortCut "$SMPROGRAMS\${APP_DIR}\$%APP_NAME%.lnk" "$INSTDIR\$%PROJECT_NAME%.exe" "" "$INSTDIR\$%PROJECT_NAME%.exe" 0

SectionEnd

; Optional section
Section "Source Files" SecSource

   File /r "source"

SectionEnd

; Optional section (can be disabled by the user)
Section "Desktop Shortcut" SecDesktop

    CreateShortCut "$DESKTOP\$%APP_NAME%.lnk" "$INSTDIR\$%PROJECT_NAME%.exe" "" "$INSTDIR\$%PROJECT_NAME%.exe" 0
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Quick Launch Shortcut" SecQuick

    CreateShortCut "$QUICKLAUNCH\$%APP_NAME%.lnk" "$INSTDIR\$%PROJECT_NAME%.exe" "" "$INSTDIR\$%PROJECT_NAME%.exe" 0
  
SectionEnd

;--------------------------------
;Installer Functions

Function .onInit

  ;!insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd

;--------------------------------

; Uninstaller Sections

Section "Uninstall"
 
  ; Remove registry keys
  DeleteRegKey HKCU "${UNINSTALL_DIR}"
  DeleteRegKey HKCU "SOFTWARE\${APP_DIR}"

  ; Remove files and uninstaller
  Delete $INSTDIR\$%PROJECT_NAME%.exe
  Delete $INSTDIR\libgcc_s_dw2-1.dll
  Delete $INSTDIR\mingwm10.dll
  Delete $INSTDIR\QtCore4.dll
  Delete $INSTDIR\QtGui4.dll
  Delete $INSTDIR\QtSvg4.dll
  Delete $INSTDIR\QtWebKit4.dll
  Delete $INSTDIR\QtNetwork4.dll
  Delete $INSTDIR\QtXmlPatterns4.dll
  Delete $INSTDIR\phonon4.dll
  Delete $INSTDIR\license.txt
  Delete $INSTDIR\readme.txt
  Delete $INSTDIR\$%PROJECT_NAME%.ico
  Delete "$DESKTOP\$%APP_NAME%.lnk"
  Delete "$QUICKLAUNCH\$%APP_NAME%.lnk"
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\${APP_DIR}\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\${APP_DIR}"
  RMDir /r "$INSTDIR"
  
  ; If there are no other directories in the company shortcut directory, remove the company shortcut directory.
  ${If} ${FileExists} "$SMPROGRAMS\$%APP_COMPANY_NAME%\"
  ${Else}
    RMDir "$SMPROGRAMS\$%APP_COMPANY_NAME%"
  ${EndIf}
  
  ; If there are no other directories in the company application directory, remove the company application directory.
  ${If} ${FileExists} "$LOCALAPPDATA\$%APP_COMPANY_NAME%\"
  ${Else}
    RMDir "$LOCALAPPDATA\$%APP_COMPANY_NAME%"
  ${EndIf}

SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd


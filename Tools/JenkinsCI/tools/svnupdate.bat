@echo off
SET MG_SOURCES_DIR=C:\working\sources
svn update "%MG_SOURCES_DIR%\fdo-3.9"
svn update "%MG_SOURCES_DIR%\fdo-trunk"
svn update "%MG_SOURCES_DIR%\mg-2.6\Installer"
svn update "%MG_SOURCES_DIR%\mg-2.6\MgDev"
svn update "%MG_SOURCES_DIR%\mg-trunk\Installer"
svn update "%MG_SOURCES_DIR%\mg-trunk\MgDev"
svn update "%MG_SOURCES_DIR%\MgInstantSetup"

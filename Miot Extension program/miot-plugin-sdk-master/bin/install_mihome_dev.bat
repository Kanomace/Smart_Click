@echo off

goto listMenu

:installBinary
    REM cls
    REM FOR /F "tokens=* USEBACKQ" %%F IN (`node -v`) DO (SET installedVersion=%%F)
    REM  if not x%installedVersion:�����ڲ����ⲿ����=%==x%installedVersion% (
    REM      SET /P reInstall=��ǰ�Ѿ���װ %installedVersion%�����°�װ?(Y/[N])
    REM      IF /I "%reInstall%" NEQ "Y" goto listMenu
    REM  )
    cls
    echo ��ʾ��
    echo  Ϊ���ⱻ����Ϊ��������ֹ������ǰ�� Windows ��ȫ���� ����������в���� ���������ùر� ʵʱ����
    echo  ��װ��ɺ��ٿ�������
    echo  ������� MSVCP140 ��Ϊ���ϵͳ��װVisual_Studio_2015
    pause

    cls
    echo ����Ϊ����װ %2
    cd %userprofile%\
    IF not exist AppData\Local\mi (mkdir AppData\Local\mi)
    cd AppData\Local\mi
        echo ������...
        IF exist file.zip ( del file.zip )
        certutil -urlcache -split -f %1 file.zip
        echo ��ѹ��...
        IF exist %2 ( rmdir /Q /S %2)
        tar xf file.zip
        del file.zip
        echo ������...
        setlocal enabledelayedexpansion
        setx PATH "%userprofile%\AppData\Local\mi\%2;%~dp0;%PATH%"
    echo ����
    "%3"
    pause
    goto listMenu

:fixEnviroument
    cls
    SET /P needDownload=��ʾ��������Դ�������غ������ֶ�������Դ������ٶȺͳɹ��ʣ��Ƿ��ֶ�����?(Y/[N])
    set needDownloadY = false
    IF "%needDownload%" == "Y" (set needDownloadY=true)
    IF "%needDownload%" == "y" (set needDownloadY=true)
    IF "%needDownloadY%"=="true" (
        start "" https://pan.mioffice.cn:443/link/78B865E2AFE0B2BE2A49E1A086FA79F0
        echo ���ڴ򿪵���ҳ�ֶ�����zip������ѹ���ǵ�SDK��Ŀ¼
        pause
    )

    cls
    cd ..\
    set sdkdirok=true
    IF not exist miot-sdk (
       set sdkdirok=false
    )
    IF not exist projects (
       set sdkdirok=false
    )
    IF not exist package.json (
       set sdkdirok=false
    )
    IF "%sdkdirok%"=="false" (
         echo ��ȷ��SDKĿ¼�ṹ���������ҽű���binĿ¼��, �������ű�
         pause
        REM goto listMenu
    )
    echo ������...
    FOR /F "tokens=* USEBACKQ" %%F IN (`npm cache verify`) DO (
      SET npmVerify=%%F
    )
    echo %npmVerify%
    FOR /F "tokens=* USEBACKQ" %%F IN (`npm cache clean --force`) DO (
      SET npmClean=%%F
    )
    echo ��װ��...
    FOR /F "tokens=* USEBACKQ" %%F IN (`npm install`) DO (
      SET npmInstall=%%F
    )
    goto exit


:listMenu
    cls
    echo 1 ��װ node ��������
    echo 2 ��װ watchman ��������
    echo 3 �޸� node_modules ��Դ
    SET /P FUNC=��ѡ��
    IF "%FUNC%" == "1" CALL :installBinary https://npm.taobao.org/mirrors/node/v12.16.1/node-v12.16.1-win-x64.zip  node-v12.16.1-win-x64   "node -v"
    IF "%FUNC%" == "2" CALL :installBinary http://cdn.cnbj0.fds.api.mi-img.com/miio.files/commonfile_zip_895012f81cb3668260b0e8bec291b5f9.zip watchman-v2020.08.17.00-windows/bin "watchman -v"
    IF "%FUNC%" == "3" CALL :fixEnviroument


:exit
    echo ����
    pause
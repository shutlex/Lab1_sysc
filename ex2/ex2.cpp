#include <windows.h>
#include <iostream>
#include <string>

int main() {
    LPCWSTR programName = L"C:\\Windows\\System32\\notepad.exe"; 

    STARTUPINFOW startupInfo;
    PROCESS_INFORMATION processInfo;

    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    ZeroMemory(&processInfo, sizeof(processInfo));

    if (CreateProcessW(
        programName,          
        NULL,                 
        NULL,                 
        NULL,                 
        FALSE,                
        0,                    
        NULL,                 
        NULL,                 
        &startupInfo,         
        &processInfo          
    )) {
        std::wcout << L"Процес створено успішно!" << std::endl;

        std::wcout << L"Очікуємо завершення процесу..." << std::endl;
        WaitForSingleObject(processInfo.hProcess, INFINITE);

        DWORD exitCode;
        if (GetExitCodeProcess(processInfo.hProcess, &exitCode)) {
            std::wcout << L"Процес завершено. Код завершення: " << exitCode << std::endl;

            if (exitCode == 0) {
                std::wcout << L"Статус завершення: успіх" << std::endl;
            }
            else {
                std::wcout << L"Статус завершення: помилка" << std::endl;
            }
        }
        else {
            std::wcerr << L"Помилка отримання коду завершення процесу." << std::endl;
        }

        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    }
    else {
        DWORD errorCode = GetLastError();
        std::wcerr << L"Не вдалося створити процес. Код помилки: " << errorCode << std::endl;

        LPWSTR errorMessage = NULL;
        FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&errorMessage,
            0,
            NULL
        );
        std::wcerr << L"Опис помилки: " << errorMessage << std::endl;
        LocalFree(errorMessage);
    }

    return 0;
}

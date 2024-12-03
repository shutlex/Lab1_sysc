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

    
    const DWORD TIMEOUT_MS = 10000;

    
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

        DWORD waitResult = WaitForSingleObject(processInfo.hProcess, TIMEOUT_MS);

        if (waitResult == WAIT_TIMEOUT) {
            std::wcerr << L"Процес виконується занадто довго. Завершення процесу..." << std::endl;

            if (TerminateProcess(processInfo.hProcess, 1)) {
                std::wcout << L"Процес успішно завершено примусово." << std::endl;
            }
            else {
                std::wcerr << L"Не вдалося завершити процес." << std::endl;
            }
        }
        else if (waitResult == WAIT_OBJECT_0) {
            std::wcout << L"Процес завершився самостійно." << std::endl;

            DWORD exitCode;
            if (GetExitCodeProcess(processInfo.hProcess, &exitCode)) {
                std::wcout << L"Код завершення процесу: " << exitCode << std::endl;
            }
            else {
                std::wcerr << L"Помилка отримання коду завершення процесу." << std::endl;
            }
        }
        else {
            std::wcerr << L"Помилка під час очікування завершення процесу." << std::endl;
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



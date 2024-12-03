#include <windows.h>
#include <iostream>
#include <vector>
#include <string>


struct ProcessData {
    std::wstring programName;
    STARTUPINFOW startupInfo;
    PROCESS_INFORMATION processInfo;
};

int main() {
    
    std::vector<std::wstring> programs = {
        L"C:\\Windows\\System32\\notepad.exe",
        L"C:\\Windows\\System32\\calc.exe"
    };

    
    HANDLE semaphore = CreateSemaphoreW(
        NULL,              
        1,                 
        1,                 
        NULL               
    );

    if (!semaphore) {
        std::wcerr << L"Не вдалося створити семафор. Код помилки: " << GetLastError() << std::endl;
        return 1;
    }

    std::vector<ProcessData> processList;

    
    for (const auto& program : programs) {
        ProcessData processData = {};
        ZeroMemory(&processData.startupInfo, sizeof(processData.startupInfo));
        processData.startupInfo.cb = sizeof(processData.startupInfo);

        if (CreateProcessW(
            program.c_str(),      
            NULL,                 
            NULL,                 
            NULL,                 
            FALSE,                
            0,                    
            NULL,                 
            NULL,                 
            &processData.startupInfo, 
            &processData.processInfo  
        )) {
            std::wcout << L"Процес створено: " << program << std::endl;
            processList.push_back(processData);
        }
        else {
            std::wcerr << L"Не вдалося створити процес: " << program
                << L". Код помилки: " << GetLastError() << std::endl;
        }
    }

    for (auto& processData : processList) {
        std::wcout << L"Очікування завершення процесу: " << processData.programName << std::endl;

        DWORD waitResult = WaitForSingleObject(processData.processInfo.hProcess, 10000); 

        if (waitResult == WAIT_TIMEOUT) {
            std::wcerr << L"Процес виконується занадто довго: " << processData.programName << std::endl;

            WaitForSingleObject(semaphore, INFINITE);

            if (TerminateProcess(processData.processInfo.hProcess, 1)) {
                std::wcout << L"Процес примусово завершено: " << processData.programName << std::endl;
            }
            else {
                std::wcerr << L"Не вдалося завершити процес: " << processData.programName << std::endl;
            }

            ReleaseSemaphore(semaphore, 1, NULL);
        }
        else if (waitResult == WAIT_OBJECT_0) {
            std::wcout << L"Процес завершився: " << processData.programName << std::endl;
        }

        CloseHandle(processData.processInfo.hProcess);
        CloseHandle(processData.processInfo.hThread);
    }

        CloseHandle(semaphore);

    return 0;
}

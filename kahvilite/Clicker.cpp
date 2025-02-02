#include "clicker.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <mutex>
#include <cstdlib>
#include <unordered_map>

#include "string"
#include "Logs.h"

HWND g_targetWindow = NULL;

namespace clicker {
    bool enabled = false;
    int cps = 10;
}

void sendClickToWindow(HWND hwnd, int button, int sleep1, int sleep2) {

    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleep1 - rand() % 2)));

    if (button == 1)
        PostMessage(hwnd, WM_LBUTTONDOWN, 0, 0);

    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleep2 * 0.33 + rand() % 2)));

    if (button == 1) 
        PostMessage(hwnd, WM_LBUTTONUP, 0, 0);
}

BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam) {
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);

    if (processId == static_cast<DWORD>(lParam)) {
        g_targetWindow = hwnd;
        return FALSE;
    }
    return TRUE;
}

DWORD getJavawProcessId() {
    DWORD javawProcessId = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to take a snapshot of processes." << std::endl;
        return 0;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32)) {
        do {
            // Use _wcsicmp for wide-character comparison
            if (_wcsicmp(pe32.szExeFile, L"javaw.exe") == 0) {
                javawProcessId = pe32.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return javawProcessId;
}

HWND getJavawWindow() {
    DWORD javawProcessId = getJavawProcessId();
    if (javawProcessId == 0) {
        //addLog("Failed to get Javaw PiD");
        return NULL;
    }

    EnumWindows(enumWindowsProc, static_cast<LPARAM>(javawProcessId));

    //addLog("Found Javaw window for process ID: " + std::to_string(javawProcessId));
    return g_targetWindow;
}

bool isKeyPressed(int key) {
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

int getRand() {
    int finalCps = clicker::cps;

    double time = static_cast<double>(std::time(nullptr));

    double frequency = 0.5 + 0.5 * std::sin(time / 10);
    finalCps += static_cast<int>(2 * std::sin(time * frequency));

    finalCps += rand() % 5 - 2;

    if (rand() % 30 < 1)
        finalCps += rand() % 3 + 1;

    if (rand() % 30 < 1)
        finalCps += rand() % 3 + 1;

    if (rand() % 200 < 1)
        finalCps += rand() % 3 + 1;

    if (rand() % 200 < 1)
        finalCps += rand() % 3 + 1;

    return finalCps / 1.5;

}
void doClicker() {
    HWND hwnd = getJavawWindow();
    while (true) {
        if (isKeyPressed(VK_LBUTTON) && clicker::enabled) {
            if (hwnd) {
                int sleepTime = 1000 / getRand();

                sleepTime -= rand() % 10;

                sleepTime += rand() % 10;

                sendClickToWindow(hwnd, 1, sleepTime * 0.67, sleepTime * 0.33);
            }
            else {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
}

void startClicker() {
        std::thread t(doClicker);
        t.join();
}
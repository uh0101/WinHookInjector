#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

static void ExitWithMessage(const char* message) {
    std::cerr << "[ERROR] " << message << '\n';
    system("pause");
    exit(1);
}

static DWORD FindProcessId(const wchar_t* processName) {
    PROCESSENTRY32 entry{ entry.dwSize = sizeof(PROCESSENTRY32) };

    HANDLE snapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
    if (Process32First(snapshot, &entry)) {
        do {
            if (_wcsicmp(entry.szExeFile, processName) == 0) {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        } while (Process32Next(snapshot, &entry));
    } CloseHandle(snapshot);

    return 0;
}

static HWND FindWindowFromProcessId(DWORD processId) {
    HWND hwnd{ nullptr };
    do {
        hwnd = FindWindowExW(nullptr, hwnd, nullptr, nullptr) ;
        DWORD pid{ 0 };
        GetWindowThreadProcessId(hwnd, &pid);
        if (pid == processId) return hwnd;
    } while (hwnd != nullptr);

    return nullptr;
}

int wmain(int argc, wchar_t* argv[]) {
    if (argc < 3) ExitWithMessage("Usage: WinHookInjector <DLL> <Process>");

    wchar_t* dllPath{ argv[1] };
    if (GetFileAttributes(dllPath) == INVALID_FILE_ATTRIBUTES) ExitWithMessage("DLL not found.");

    HWND hwnd{ FindWindowFromProcessId(FindProcessId(argv[2])) };
    if (!hwnd) ExitWithMessage("Failed to find Window.");

    DWORD threadId{ GetWindowThreadProcessId(hwnd, nullptr) };
    if (!threadId) ExitWithMessage("Failed to get ThreadID.");

    HMODULE dll{ LoadLibraryEx(dllPath, nullptr, DONT_RESOLVE_DLL_REFERENCES) };
    if (!dll) ExitWithMessage("Error while loading DLL.");

    HOOKPROC addr{ (HOOKPROC)GetProcAddress(dll, "NextHook") };
    if (!addr) ExitWithMessage("NextHook not found.");

    HHOOK handle{ SetWindowsHookEx(WH_GETMESSAGE, addr, dll, threadId) };
    if (!handle) ExitWithMessage("Failed to set hook.");

    PostThreadMessage(threadId, WM_NULL, 0, 0);

    std::cout << "[SUCCESS] Hook set and triggered.\n";
    std::cout << "[INFO] Press any key to unhook (This will unload the DLL).\n";
    system("pause > nul");

    if (!UnhookWindowsHookEx(handle)) ExitWithMessage("Failed to unhook.");
    return 0;
}
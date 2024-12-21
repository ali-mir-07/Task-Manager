#include <iostream>
#include <windows.h>
#include <tlhelp32.h>

using namespace std;

void listProcesses() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        cerr << "Failed to take a snapshot of processes.\n";
        return;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &processEntry)) {
        cout << "PID\tProcess Name\n";
        do {
            cout << processEntry.th32ProcessID << "\t" 
                 << processEntry.szExeFile << "\n";
        } while (Process32Next(hSnapshot, &processEntry));
    } else {
        cerr << "Failed to retrieve process information.\n";
    }

    CloseHandle(hSnapshot);
}

void terminateProcess(DWORD processID) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
    if (hProcess) {
        if (TerminateProcess(hProcess, 0)) {
            cout << "Process terminated successfully.\n";
        } else {
            cerr << "Failed to terminate the process.\n";
        }
        CloseHandle(hProcess);
    } else {
        cerr << "Unable to open process for termination.\n";
    }
}

int main() {
    int choice;
    DWORD processID;

    while (true) {
        cout << "\n--- Process Manager ---\n";
        cout << "1. List all processes\n";
        cout << "2. Terminate a process\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                listProcesses();
                break;
            case 2:
                cout << "Enter Process ID to terminate: ";
                cin >> processID;
                terminateProcess(processID);
                break;
            case 3:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

#include <iostream>
#include <fstream>

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

LRESULT CALLBACK WindProcedure(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);
void Encrypt(int);
void Decrypt(int);
HMENU hMenu;
HWND hKey;

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInstance;
    wc.lpszClassName = L"myWindClass";
    wc.lpfnWndProc = WindProcedure;

    if (!RegisterClassW(&wc))
        return -1;
    
    CreateWindowW(L"myWindClass", L"EncApp", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500,
                    NULL, NULL, NULL, NULL);

    MSG msg = { 0 };

    while (GetMessage(&msg, NULL, NULL, NULL) ) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg)
    {
    case WM_COMMAND:
        switch (wp) {
        case 2:
            wchar_t temp[32];
            int key;
            GetWindowTextW(hKey, temp, 32);
            if ((int)temp)
                key = (int)temp;
            else key = 12;
            Encrypt(key);
            break;
        
        case 3:
            wchar_t detemp[32];
            int dekey;
            GetWindowTextW(hKey, detemp, 32);
            if ((int)detemp)
                dekey = (int)detemp;
            else dekey = 12;
            Decrypt(dekey);
            break;
        
        case 4:
            DestroyWindow(hWnd);
            break;
        }
    case WM_CREATE:
        AddMenus(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
}

void AddMenus(HWND hWnd) {
    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    AppendMenu(hFileMenu, MF_STRING, 2, L"Enc");
    AppendMenu(hFileMenu, MF_STRING, 3, L"Dec");
    AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hFileMenu, MF_STRING, 4, L"Exit");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    
    SetMenu(hWnd, hMenu);

    CreateWindowW(L"static", L"The key is 1.", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 100, 100, 50,
        hWnd, NULL, NULL, NULL);
    /*hKey = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 160, 100, 50,
        hWnd, NULL, NULL, NULL);*/
    CreateWindowW(L"static", L"Go to file, Enc/Dec. Use it on respective files.", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 220, 100, 80,
        hWnd, NULL, NULL, NULL);
}


void Encrypt(int key) {
    wchar_t filename[MAX_PATH];

    OPENFILENAME ofn;

    ZeroMemory(&filename, sizeof(filename));
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = L"Any File\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = L"Select a File, yo!";
    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn))
    {
        char c;
        ifstream fin;
        ofstream fout;
        fin.open(filename, ios::binary);
        string outname = "encrypted";
        fout.open(outname, ios::binary);
        while (true) {
            fin >> noskipws >> c;
            int temp = (c + 1);
            fout << (char)temp;
            if (fin.eof()) break;
        }
        fin.close();
        fout.close();
    }
    else
    {
        //User cancellation of browsing the file
    }
}

void Decrypt(int key) {
    wchar_t filename[MAX_PATH];

    OPENFILENAME ofn;

    ZeroMemory(&filename, sizeof(filename));
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = L"Any File\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = L"Select a File, yo!";
    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn))
    {
        char c;
        ifstream fin;
        ofstream fout;
        fin.open(filename, ios::binary);
        string outname = "recovered";
        fout.open(outname, ios::binary);
        while (true) {
            fin >> noskipws >> c;
            int temp = (c - 1);
            fout << (char)temp;
            if (fin.eof()) break;
        }
        fin.close();
        fout.close();
    }
    else
    {
        //User cancellation of browsing the file
    }
}
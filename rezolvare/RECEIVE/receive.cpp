#undef UNICODE
#include <windows.h>
#include <string.h>
#include <iostream>
#include <map>

using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#define MAXLEN 100

static char szBuffer[MAXLEN];
COPYDATASTRUCT cs;
string receivedMessage = "";


string convertToMorse(const string& text) {
    map<char, string> morse = {
        {'A', "10111"}, {'B', "111010101"}, {'C', "11101011101"},
        {'D', "1110101"}, {'E', "1"}, {'F', "101011101"},
        {'G', "111011101"}, {'H', "1010101"}, {'I', "101"},
        {'J', "1011101110111"}, {'K', "111010111"}, {'L', "101110101"},
        {'M', "1110111"}, {'N', "11101"}, {'O', "11101110111"},
        {'P', "10111011101"}, {'Q', "1110111010111"}, {'R', "1011101"},
        {'S', "10101"}, {'T', "111"}, {'U', "1010111"},
        {'V', "101010111"}, {'W', "101110111"}, {'X', "11101010111"},
        {'Y', "1110101110111"}, {'Z', "11101110101"}
    };

    string codMorse = "";

    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];
        if (c == ' ') {
            codMorse += "00000";
        }
        else if (morse.find(toupper(c)) != morse.end()) {
            codMorse += morse[toupper(c)];


            if (i + 1 < text.size() && text[i + 1] != ' ') {
                codMorse += "000";
            }
        }
    }

    return codMorse;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{
    static char szAppName[] = "Receive";
    HWND        hwnd;
    MSG         msg;
    WNDCLASSEX  wndclass; // obiectul instantiat din structura WNDCLASSEX

    wndclass.cbSize = sizeof(wndclass);//Dimensiunea structurii
    wndclass.style = CS_HREDRAW | CS_VREDRAW; //Stilul ferestrei
    wndclass.lpfnWndProc = WndProc; // Procedura de fereastra care trateaza mesajele
    // Doua câmpuri care permit rezervarea de spatiu suplimentar in structura class, respectiv structura window.
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance; //Identificatorul instantei ferestrei
    //Stabileste o pictograma pentru toate ferestrele create pe baza clasei window
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    //Stabileste o pictograma pentru cursor
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    //Specifica culoarea de fundal a ferestrei
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL; // Specifica meniul feresterei
    wndclass.lpszClassName = szAppName; // Specifica numele clasei feresterei
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    //Inregistrarea clasei de fereastra
    RegisterClassEx(&wndclass);

    // Crearea ferestrei pe baza clasei de fereastra
    hwnd = CreateWindow(szAppName,         // window class name
        "ReceiveMessage",     // window caption
        WS_OVERLAPPEDWINDOW,     // window style
        CW_USEDEFAULT,           // initial x position
        CW_USEDEFAULT,           // initial y position
        600,           // initial x size
        200,           // initial y size
        NULL,                    // parent window handle
        NULL,                    // window menu handle
        hInstance,               // program instance handle
        NULL);		             // creation parameters

    ShowWindow(hwnd, iCmdShow); // afiseaza fereastra pe ecran
    //transmite catre WndProc un mesaj de tip WM_PAINT, care are ca rezultat
    //redesenarea ferestrei, adica reactualizarea zonei client a ferestrei
    UpdateWindow(hwnd); // 

    // preia si distribuie mesaje pana se primeste WM_QUIT
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg); // traduce mesajul
        DispatchMessage(&msg); // transmite mesajul catre procedura de fereastra
    }
    return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc; //contextul grafic
    PAINTSTRUCT ps; // o variabila la structura de desenare
    RECT rect; //obiect dreptunghi

    switch (iMsg)
    {

    case WM_CREATE://operatii ce se executa la crearea ferestrei
        receivedMessage = "Astept text pentru codificat...";
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;


    case WM_COPYDATA:
        {
        PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT)lParam;
        strcpy_s(szBuffer, (char*)pcds->lpData);

        receivedMessage = "Mesajul receptionat de la Send este: ";
        receivedMessage += szBuffer;

        string morseText = convertToMorse(szBuffer);

        cs.cbData = morseText.size() + 1;
        cs.lpData = (void*)morseText.c_str();

        SendMessage((HWND)wParam, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&cs);
        InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;

    case WM_PAINT://operatii ce se executa la desenarea ferestrei
        hdc = BeginPaint(hwnd, &ps);//Obtinerea contextului grafic
        GetClientRect(hwnd, &rect); //Obtinerea suprafetei de desenare
        //Scrierea unui text în fereastr?
        DrawText(hdc, receivedMessage.c_str(), -1, &rect,
            DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hwnd, &ps);//Eliberarea contextului grafic
        return 0;

    case WM_DESTROY://operatii ce se executa la distrugerea ferestrei
        PostQuitMessage(0);// insereaza un mesaj de incheiere in coada de mesaje
        return 0;
    }
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
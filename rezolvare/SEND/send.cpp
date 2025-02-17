#undef UNICODE
#include <windows.h>
#include <string.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#define MAXLEN 100

const int ID_BUTTON = 1; 
const int ID_EDIT = 2; 
const int ID_OUTPUT = 3; 

TEXTMETRIC tm;
HWND hwndReceive;
static int cxChar, cyChar;
static HWND hEdit, hButtonOk, hOutput;
static char szBuffer[MAXLEN];
COPYDATASTRUCT cs;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{
    static char szAppName[] = "Send";
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
        "SendMessage",     // window caption
        WS_OVERLAPPEDWINDOW,     // window style
        CW_USEDEFAULT,           // initial x position
        CW_USEDEFAULT,           // initial y position
        575,           // initial x size
        600,           // initial y size
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
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    switch (iMsg) {

    case WM_CREATE://operatii ce se executa la crearea ferestrei
        hdc = GetDC(hwnd);
        SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
        GetTextMetrics(hdc, &tm);
        cxChar = tm.tmAveCharWidth;
        cyChar = tm.tmHeight + tm.tmExternalLeading;
        ReleaseDC(hwnd, hdc);

        hButtonOk = CreateWindow("button", "OK", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            23 * cxChar, 16 * cyChar, 25 * cxChar, 2 * cyChar,
            hwnd, (HMENU)ID_BUTTON,
            ((LPCREATESTRUCT)lParam)->hInstance, NULL);

        hEdit = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
            10 * cxChar, 8 * cyChar, 50 * cxChar, 2 * cyChar,
            hwnd, (HMENU)ID_EDIT,
            ((LPCREATESTRUCT)lParam)->hInstance, NULL);

        hOutput = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER | ES_MULTILINE,
            10 * cxChar, 20 * cyChar, 50 * cxChar, 15 * cyChar,
            hwnd, (HMENU)ID_OUTPUT,
            ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        return 0;


    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BUTTON) {
           
            GetDlgItemText(hwnd, ID_EDIT, szBuffer, MAXLEN);
            bool isValid = true;
            for (int i = 0; szBuffer[i] != '\0'; i++) {
                if (!isalpha(szBuffer[i]) && szBuffer[i] != ' ') {
                    isValid = false;
                    break;
                }
            }
    
            if (!isValid) {
                MessageBox(hwnd, "Input invalid! Introduceti doar litere.", "Eroare", MB_OK | MB_ICONERROR);
                SetDlgItemText(hwnd, ID_EDIT, ""); 
                return 0;
            }

            if (szBuffer[0] == '\0') {
                MessageBox(hwnd, "Trebuie sa intoduceti text!", "Eroare", MB_OK | MB_ICONERROR);
                return 0;
            }
          
            hwndReceive = FindWindow("Receive", "ReceiveMessage");
            if (hwndReceive != NULL) {
                cs.cbData = strlen(szBuffer) + 1;
                cs.lpData = szBuffer;
                SendMessage(hwndReceive, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&cs);
            }
            else {
                MessageBox(hwnd, "Nu am gasit partenerul Receive!!", "Eroare", MB_OK);
            }
        }
        return 0;


    case WM_COPYDATA:
    {
        PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT)lParam;
        SetDlgItemText(hwnd, ID_OUTPUT, (LPCSTR)pcds->lpData);
    }
        return 0;


    case WM_DESTROY://operatii ce se executa la distrugerea ferestrei
        PostQuitMessage(0);// insereaza un mesaj de incheiere in coada de mesaje
        return 0;

    }
    // Alte mesaje sunt trimise catre functia implicita de tratare
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

#include <windows.h>
#include "resource.h"
#include "passwordgenerator.h"
#include <tchar.h>
#include <string>
#include <commctrl.h>

#define UPDOWN_CLASSA "msctls_updown32"
#define UPDOWN_CLASSW L"msctls_updown32"

#ifdef UNICODE
#define UPDOWN_CLASS UPDOWN_CLASSW
#else
#define UPDOWN_CLASS UPDOWN_CLASSA
#endif


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
int OnCreate(const HWND,CREATESTRUCT*);

HWND CreateButton(const HWND,const HINSTANCE,DWORD,const RECT&,const int,
                  const char *);
HWND chbUpper;
HWND chbLower;
HWND chbNumber;
HWND chbSpecChar;
HWND lblPassLength;
HWND lblPassword;
HWND tbPassword;
HWND tbPasswordLength;
HWND nudPasswordLength;
HWND btnGenerate;

int pwlength;

/*  Make the class name into a global variable  */
char szClassName[ ] = "PWGen";

enum {
  IDBC_UPPERCHECKBOX=202,
  IDBC_LOWERCHECKBOX,
  IDBC_NUMBERCHECKBOX,
  IDBC_SPECCHARCHECKBOX,
  IDBC_BTNGENERATE,
  IDBC_TBPASSWORD,
  IDBC_TBPASSWORDLENGTH,
  IDBC_NUDLENGTH
};

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon    = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_APP_ICON));
    wincl.hIconSm  = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_APP_ICON), IMAGE_ICON, 16, 16, 0);
    wincl.hCursor  = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_WINDOW; //APPWORKSPACE

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Password Generator :: by Sektor",       /* Title Text */
           WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           370,                 /* The programs width */
           153,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    int xPos = (GetSystemMetrics(SM_CXSCREEN) - 370)/2;
    int yPos = (GetSystemMetrics(SM_CYSCREEN) - 153)/2;

    SetWindowPos( hwnd, HWND_TOPMOST, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE );

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            return OnCreate(hwnd,reinterpret_cast<CREATESTRUCT*>(lParam));
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_COMMAND:
            if( LOWORD(wParam) == IDBC_UPPERCHECKBOX ){
                HKEY hKey;

                if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\PasswordGenerator",0,KEY_SET_VALUE,&hKey) != ERROR_SUCCESS){
                    RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\PasswordGenerator",0,NULL,REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
                }

                std::string upper;
                if( (SendMessage(chbUpper, BM_GETCHECK, 0, 0) == BST_CHECKED) ){
                    upper = "True";
                }
                else{
                    upper = "False";
                }

                RegSetValueEx(hKey,"useUpperCase",0,REG_SZ,(const unsigned char*)upper.c_str(),5);
                RegCloseKey(hKey);
            }

            if( LOWORD(wParam) == IDBC_LOWERCHECKBOX ){
                HKEY hKey;

                if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\PasswordGenerator",0,KEY_SET_VALUE,&hKey) != ERROR_SUCCESS){
                    RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\PasswordGenerator",0,NULL,REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
                }

                std::string lower;
                if( (SendMessage(chbLower, BM_GETCHECK, 0, 0) == BST_CHECKED) ){
                    lower = "True";
                }
                else{
                    lower = "False";
                }

                RegSetValueEx(hKey,"useLowerCase",0,REG_SZ,(const unsigned char*)lower.c_str(),5);
                RegCloseKey(hKey);
            }

            if( LOWORD(wParam) == IDBC_NUMBERCHECKBOX ){
                HKEY hKey;

                if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\PasswordGenerator",0,KEY_SET_VALUE,&hKey) != ERROR_SUCCESS){
                    RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\PasswordGenerator",0,NULL,REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
                }

                std::string num;
                if( (SendMessage(chbNumber, BM_GETCHECK, 0, 0) == BST_CHECKED) ){
                    num = "True";
                }
                else{
                    num = "False";
                }

                RegSetValueEx(hKey,"useNumbers",0,REG_SZ,(const unsigned char*)num.c_str(),5);
                RegCloseKey(hKey);
            }

            if( LOWORD(wParam) == IDBC_SPECCHARCHECKBOX ){
                HKEY hKey;

                if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\PasswordGenerator",0,KEY_SET_VALUE,&hKey) != ERROR_SUCCESS){
                    RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\PasswordGenerator",0,NULL,REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
                }

                std::string spec;
                if( (SendMessage(chbSpecChar, BM_GETCHECK, 0, 0) == BST_CHECKED) ){
                    spec = "True";
                }
                else{
                    spec = "False";
                }

                RegSetValueEx(hKey,"useSpecialChars",0,REG_SZ,(const unsigned char*)spec.c_str(),5);
                RegCloseKey(hKey);
            }

            if( LOWORD(wParam) == IDBC_BTNGENERATE){
                PasswordGenerator pg((SendMessage(chbUpper, BM_GETSTATE, 0, 0) == BST_CHECKED),
                                     (SendMessage(chbLower, BM_GETSTATE, 0, 0) == BST_CHECKED),
                                     (SendMessage(chbNumber, BM_GETSTATE, 0, 0) == BST_CHECKED),
                                     (SendMessage(chbSpecChar, BM_GETSTATE, 0, 0) == BST_CHECKED),
                                     pwlength);
                SendMessage(tbPassword, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)pg.GeneratePassword().c_str());
            }
            if (LOWORD(wParam) == IDBC_TBPASSWORDLENGTH){
                char s[5];
                GetWindowText(tbPasswordLength, s, sizeof(s));
                pwlength = atoi(s);

                HKEY hKey;

                if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\PasswordGenerator",0,KEY_SET_VALUE,&hKey) != ERROR_SUCCESS){
                    RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\PasswordGenerator",0,NULL,REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
                }

                std::string len;
                if(strcmp(s,"") != 0){
                    if(pwlength > 999){
                        SendMessage(tbPasswordLength, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)"999");
                        break;
                    }
                    else if(pwlength < 1){
                        SendMessage(tbPasswordLength, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)"1");
                        break;
                    }
                }

                if( strcmp(s,"") != 0 && pwlength <= 0 ){
                    len = "20";
                }
                else{
                    len = s;
                }
                if(strcmp(len.c_str(),"")!=0){
                    RegSetValueEx(hKey,"passwordLendth",0,REG_SZ,(const unsigned char*)len.c_str(),5);
                }
                RegCloseKey(hKey);
            }

            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

int OnCreate(const HWND hwnd,CREATESTRUCT *cs)
{
    bool bUpper, bLower, bNum, bSpec;
    unsigned char up[5];
    unsigned char low[5];
    unsigned char num[5];
    unsigned char spec[5];
    unsigned char value[5];
    DWORD cb = sizeof(value);
    DWORD dwType = REG_SZ;
    HKEY hKey;

    if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\PasswordGenerator",0,KEY_QUERY_VALUE,&hKey) == ERROR_SUCCESS){
        if(RegQueryValueEx(hKey, "useUpperCase", NULL, &dwType, up, &cb) == ERROR_SUCCESS){
            if(strcmp((const char*)up,"True") == 0){
                bUpper=true;
            }
            else{
                bUpper=false;
            }
        }
        else{
            bUpper=false;
        }
        if(RegQueryValueEx(hKey, "useLowerCase", NULL, &dwType, low, &cb) == ERROR_SUCCESS){
            if(strcmp((const char*)low,"True") == 0){
                bLower=true;
            }
            else{
                bLower=false;
            }
        }
        else{
            bLower=false;
        }
        if(RegQueryValueEx(hKey, "useNumbers", NULL, &dwType, num, &cb) == ERROR_SUCCESS){
            if(strcmp((const char*)num,"True") == 0){
                bNum=true;
            }
            else{
                bNum=false;
            }
        }
        else{
            bNum=false;
        }
        if(RegQueryValueEx(hKey, "useSpecialChars", NULL, &dwType, spec, &cb) == ERROR_SUCCESS){
            if(strcmp((const char*)spec,"True") == 0){
                bSpec=true;
            }
            else{
                bSpec=false;
            }
        }
        else{
            bSpec=false;
        }
        if(RegQueryValueEx(hKey, "passwordLendth", NULL, &dwType, value, &cb) == ERROR_SUCCESS){
            if(atoi((char*)value) <= 0){
                strcpy((char*)value,"20");
            }
        }

    }

    RegCloseKey(hKey);

    HFONT textFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE|DEFAULT_PITCH, "Microsoft Sans Serif");
    if(textFont == 0) MessageBox(hwnd, "Failed to load \"Microsoft Sans Serif\" font!", "Error", MB_OK|MB_ICONERROR);

    //Checkboxes
    RECT rc={15,10,75,20};
    chbUpper = CreateButton(hwnd,cs->hInstance,BS_AUTOCHECKBOX,rc,IDBC_UPPERCHECKBOX,"Upper case");
    SendMessage(chbUpper, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(FALSE, 0));
    SendMessage(chbUpper, BM_SETCHECK, (bUpper ? 1 : 0), 0);

    rc={105,10,75,20};
    chbLower = CreateButton(hwnd,cs->hInstance,BS_AUTOCHECKBOX,rc,IDBC_LOWERCHECKBOX,"Lower case");
    SendMessage(chbLower, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(FALSE, 0));
    SendMessage(chbLower, BM_SETCHECK, (bLower ? 1 : 0), 0);

    rc={195,10,60,20};
    chbNumber = CreateButton(hwnd,cs->hInstance,BS_AUTOCHECKBOX,rc,IDBC_NUMBERCHECKBOX,"Numbers");
    SendMessage(chbNumber, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(FALSE, 0));
    SendMessage(chbNumber, BM_SETCHECK, (bNum ? 1 : 0), 0);

    rc={270,10,80,20};
    chbSpecChar = CreateButton(hwnd,cs->hInstance,BS_AUTOCHECKBOX,rc,IDBC_SPECCHARCHECKBOX, "Special chars");
    SendMessage(chbSpecChar, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(FALSE, 0));
    SendMessage(chbSpecChar, BM_SETCHECK, (bSpec ? 1 : 0), 0);
    //checkboxes end

    lblPassLength = CreateWindowEx( WS_EX_TRANSPARENT, "STATIC", "Password length:",
                                    WS_CHILD | WS_VISIBLE | SS_LEFT | WS_SYSMENU ,
                                    15, 40, 85, 15, hwnd, NULL, cs->hInstance, NULL);
    SendMessage(lblPassLength, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(FALSE, 0));

    tbPasswordLength = CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("Edit"), TEXT((char*)value),
                     WS_VISIBLE | WS_CHILD | ES_NUMBER,
                     100, 35, 250, 20,
                     hwnd, (HMENU)IDBC_TBPASSWORDLENGTH, cs->hInstance, NULL);
    SendMessage(tbPasswordLength, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(FALSE, 0));

    nudPasswordLength = CreateWindow(UPDOWN_CLASS, "IDBC_NUDLENGTH",
                   WS_CHILD | WS_VISIBLE | UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_SETBUDDYINT,
                   100, 35, 205, 20, hwnd, reinterpret_cast<HMENU>(static_cast<INT_PTR>(IDBC_NUDLENGTH)), cs->hInstance, NULL);

    SendMessage(nudPasswordLength, UDM_SETBUDDY, (LONG)tbPasswordLength, 0L);
    SendMessage(nudPasswordLength, UDM_SETRANGE, 0L, MAKELONG(999, 1));

    lblPassword = CreateWindowEx( WS_EX_TRANSPARENT, "STATIC", "Password:",
                                    WS_CHILD | WS_VISIBLE | SS_LEFT | WS_SYSMENU ,
                                    15, 65, 55, 15, hwnd, reinterpret_cast<HMENU>(static_cast<INT_PTR>(IDBC_TBPASSWORD)), cs->hInstance, NULL);
    SendMessage(lblPassword, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(FALSE, 0));

    tbPassword = CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("Edit"), TEXT(""),
                     WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
                     75, 62, 275, 20,
                     hwnd, NULL, cs->hInstance, NULL);
    SendMessage(tbPassword, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(FALSE, 0));

    rc={15,92,336,25};
    btnGenerate = CreateButton(hwnd,cs->hInstance,BS_PUSHBUTTON,rc,IDBC_BTNGENERATE,"Generate password");
    SendMessage(btnGenerate, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(FALSE, 0));

    char s[5];
    GetWindowText(tbPasswordLength, s, sizeof(s));
    pwlength = atoi(s);

    if(strcmp(s,"") != 0){
        if(pwlength > 999){
            SendMessage(tbPasswordLength, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)"999");
        }
        else if(pwlength < 1){
            SendMessage(tbPasswordLength, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)"1");
        }
    }

    return 0;
}

HWND CreateButton(const HWND hParent,const HINSTANCE hInst,DWORD dwStyle,
                  const RECT& rc,const int id,const char * caption)
{
dwStyle|=WS_CHILD|WS_VISIBLE;
return CreateWindowEx(0,                            //extended styles
                      _T("button"),                 //control 'class' name
                      caption,              //control caption
                      dwStyle,                      //control style
                      rc.left,                      //position: left
                      rc.top,                       //position: top
                      rc.right,                     //width
                      rc.bottom,                    //height
                      hParent,                      //parent window handle
                      //control's ID
                      reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
                      hInst,                        //application instance
                      0);                           //user defined info
}

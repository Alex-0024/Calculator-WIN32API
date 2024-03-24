#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

//#define DEBUG

#define bt(A) bt_##A
#define numIfCom(A) if((HWND)lParam == bt(A)) {num = num * 10.0 + (double)(A);}
#define NUMIFCOM numIfCom(0); numIfCom(1); numIfCom(2); numIfCom(3); numIfCom(4);\
                 numIfCom(5); numIfCom(6); numIfCom(7); numIfCom(8); numIfCom(9)

#define nIfCom(A) if((HWND)lParam == bt(A)) {littleNum += (double)(A) / pow(10.0, countRazr); countRazr++;}
#define LITTLENUMIFCOM nIfCom(0); nIfCom(1); nIfCom(2); nIfCom(3); nIfCom(4);\
                       nIfCom(5); nIfCom(6); nIfCom(7); nIfCom(8); nIfCom(9)

#define numIfKey(B) if( (wParam == 0x30 + (B)) || (wParam == 0x60 + (B)) ) {num = num * 10.0 + (double)B;}
#define NUMIFKEY numIfKey(0); numIfKey(1); numIfKey(2); numIfKey(3); numIfKey(4); numIfKey(5);\
                 numIfKey(6); numIfKey(7); numIfKey(8); numIfKey(9)

#define nIfKey(B) if( (wParam == 0x30 + (B)) || (wParam == 0x60 + (B)) ) {littleNum += (double)(B) / pow(10.0, countRazr); countRazr++;}
#define LITTLENUMIFKEY nIfKey(0); nIfKey(1); nIfKey(2); nIfKey(3); nIfKey(4);\
                       nIfKey(5); nIfKey(6); nIfKey(7); nIfKey(8); nIfKey(9)

#define MAXNUMSIZE 200 // even elements for storing numbers, odd for mathematical operations (200 is 100 mathematical operations in a row)

HWND outputScreen, inputScreen;

HWND bt_C, bt_back, bt_percent, bt_division, bt_7, bt_8, bt_9, bt_multi,
     bt_4, bt_5, bt_6, bt_minus, bt_1, bt_2, bt_3, bt_plus,
     bt_plusAndMinus, bt_0, bt_point, bt_equals;

PAINTSTRUCT ps;
HDC hdc;

double result = 0;
double masNum[MAXNUMSIZE] = {};
double num = 0;
double littleNum = 0;
double countRazr = 1;
int countMasNum = 0;

char strInputScreen[20] = {};
char strNum[20] = {};
char strOutputScreen[MAXNUMSIZE * 3] = {};
char masMath[][10] = {" + ", " - ", " / ", " * "};

enum {plus, minus, division, multi} math;

BOOL flagMinus = FALSE;
BOOL flagPoint = FALSE;
BOOL flagDivideZero = FALSE;

LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ShowOutputScreen();
typedef double (*TFunc)(double, double);
double funcPlus(double a, double b);
double funcMinus(double a, double b);
double funcDivision(double a, double b);
double funcMulti(double a, double b);
TFunc Func[4] = {funcPlus, funcMinus, funcDivision, funcMulti};
double funcMath(double num1, double num2, TFunc fn);
void ShowMasNum();
void CalcAndShow();

int main()
{
    const char CLASS_NAME[] = "My Window";
    const char WINDOW_NAME[] = "Calculator";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(CLASS_NAME, WINDOW_NAME, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                             200, 200, 360, 540, NULL, NULL, NULL, NULL);
    outputScreen = CreateWindow("static", "0", WS_VISIBLE | WS_BORDER | WS_CHILD | ES_RIGHT, 10, 20, 335, 100, hwnd, NULL, NULL, NULL);
    inputScreen = CreateWindow("static", "0", WS_VISIBLE | WS_CHILD | ES_RIGHT, 10, 130, 335, 50, hwnd, NULL, NULL, NULL);

    bt_C = CreateWindow("button", "C", WS_VISIBLE | WS_BORDER | WS_CHILD, 10, 200, 80, 50, hwnd, NULL, NULL, NULL);
    bt_back = CreateWindow("button", "back", WS_VISIBLE | WS_BORDER | WS_CHILD, 95, 200, 80, 50, hwnd, NULL, NULL, NULL);
    bt_percent = CreateWindow("button", "%", WS_VISIBLE | WS_BORDER | WS_CHILD, 180, 200, 80, 50, hwnd, NULL, NULL, NULL);
    bt_division = CreateWindow("button", "/", WS_VISIBLE | WS_BORDER | WS_CHILD, 265, 200, 80, 50, hwnd, NULL, NULL, NULL);
    bt_7 = CreateWindow("button", "7", WS_VISIBLE | WS_BORDER | WS_CHILD, 10, 260, 80, 50, hwnd, NULL, NULL, NULL);
    bt_8 = CreateWindow("button", "8", WS_VISIBLE | WS_BORDER | WS_CHILD, 95, 260, 80, 50, hwnd, NULL, NULL, NULL);
    bt_9 = CreateWindow("button", "9", WS_VISIBLE | WS_BORDER | WS_CHILD, 180, 260, 80, 50, hwnd, NULL, NULL, NULL);
    bt_multi = CreateWindow("button", "X", WS_VISIBLE | WS_BORDER | WS_CHILD, 265, 260, 80, 50, hwnd, NULL, NULL, NULL);
    bt_4 = CreateWindow("button", "4", WS_VISIBLE | WS_BORDER | WS_CHILD, 10, 320, 80, 50, hwnd, NULL, NULL, NULL);
    bt_5 = CreateWindow("button", "5", WS_VISIBLE | WS_BORDER | WS_CHILD, 95, 320, 80, 50, hwnd, NULL, NULL, NULL);
    bt_6 = CreateWindow("button", "6", WS_VISIBLE | WS_BORDER | WS_CHILD, 180, 320, 80, 50, hwnd, NULL, NULL, NULL);
    bt_minus = CreateWindow("button", "-", WS_VISIBLE | WS_BORDER | WS_CHILD, 265, 320, 80, 50, hwnd, NULL, NULL, NULL);
    bt_1 = CreateWindow("button", "1", WS_VISIBLE | WS_BORDER | WS_CHILD, 10, 380, 80, 50, hwnd, NULL, NULL, NULL);
    bt_2 = CreateWindow("button", "2", WS_VISIBLE | WS_BORDER | WS_CHILD, 95, 380, 80, 50, hwnd, NULL, NULL, NULL);
    bt_3 = CreateWindow("button", "3", WS_VISIBLE | WS_BORDER | WS_CHILD, 180, 380, 80, 50, hwnd, NULL, NULL, NULL);
    bt_plus = CreateWindow("button", "+", WS_VISIBLE | WS_BORDER | WS_CHILD, 265, 380, 80, 50, hwnd, NULL, NULL, NULL);
    bt_plusAndMinus = CreateWindow("button", "+/-", WS_VISIBLE | WS_BORDER | WS_CHILD, 10, 440, 80, 50, hwnd, NULL, NULL, NULL);
    bt_0 = CreateWindow("button", "0", WS_VISIBLE | WS_BORDER | WS_CHILD, 95, 440, 80, 50, hwnd, NULL, NULL, NULL);
    bt_point = CreateWindow("button", ",", WS_VISIBLE | WS_BORDER | WS_CHILD, 180, 440, 80, 50, hwnd, NULL, NULL, NULL);
    bt_equals = CreateWindow("button", "=", WS_VISIBLE | WS_BORDER | WS_CHILD, 265, 440, 80, 50, hwnd, NULL, NULL, NULL);

    ShowWindow(hwnd, SW_NORMAL);

    MSG msg = {};

    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
        case WM_KEYDOWN:

            if ((HWND)lParam == bt_C || wParam == VK_ESCAPE)
            {
                num = littleNum = 0;
                countRazr = 1;
                countMasNum = 0;
                memset(masNum, 0, sizeof(masNum));
                flagMinus = FALSE;
                flagPoint = FALSE;
                flagDivideZero = FALSE;
            }

            if ((HWND)lParam == bt_plusAndMinus)
            {
                if(flagMinus)
                    flagMinus = FALSE;
                else
                    flagMinus = TRUE;
            }

            if ((HWND)lParam == bt_point || wParam == VK_DECIMAL)
                flagPoint = TRUE;

            if(flagPoint)
            {
                LITTLENUMIFCOM;
                LITTLENUMIFKEY;
                #ifdef DEBUG
                    printf("littleNum = %g\n", littleNum);
                #endif // DEBUG
            }
            else
            {
                NUMIFCOM;
                NUMIFKEY;
            }

            if(flagMinus)
                masNum[countMasNum] = - num - littleNum;
            else
                masNum[countMasNum] = num + littleNum;
            #ifdef DEBUG
                printf("FlagPoint = %d\n", flagPoint);
                printf("masNum[i] = %g\n", masNum[countMasNum]);
            #endif // DEBUG

            ShowMasNum();

            if((HWND)lParam == bt_percent && countMasNum >=2)
            {
                /* calculation of the sum of elements up to the element in which percentages will be indicated */
                if(countMasNum >= 4)
                {
                    double res = 0;
                    countMasNum -= 2;
                    for(int i = 0; i < countMasNum; i += 2)
                    {
                        if (i == 0)
                            res = funcMath(masNum[i], masNum[i+2], Func[(int)masNum[i+1]] );
                        else
                            res = funcMath(res, masNum[i+2], Func[(int)masNum[i+1]]);
                    }
                    countMasNum += 2;
                    /* calculation of calculations for all elements, taking into account the calculation of the last element as a percentage */
                    masNum[countMasNum] = res / 100.0 * masNum[countMasNum];
                    num = masNum[countMasNum];
                    littleNum = 0;
                    #ifdef DEBUG
                        printf("masNum = %g\n", masNum[countMasNum]);
                    #endif // DEBUG
                    CalcAndShow();

                }
                else
                {
                    /* calculation of calculations for all elements, taking into account the calculation of the last element as a percentage */
                    masNum[countMasNum] = masNum[countMasNum-2] / 100.0 * masNum[countMasNum];
                    num = masNum[countMasNum];
                    littleNum = 0;
                    #ifdef DEBUG
                        printf("masNum = %g\n", masNum[countMasNum]);
                    #endif // DEBUG
                    CalcAndShow();
                }
            }

            if ((HWND)lParam == bt_plus || (HWND)lParam == bt_minus || (HWND)lParam == bt_multi || (HWND)lParam == bt_division ||
                      wParam == VK_ADD  ||    wParam == VK_SUBTRACT ||    wParam == VK_MULTIPLY || wParam == VK_DIVIDE)
            {
                flagMinus = FALSE;
                flagPoint = FALSE;
                countMasNum++;
                num = littleNum = 0;
                countRazr = 1;
                SetWindowText(inputScreen, "0");
                masNum[countMasNum] = (HWND)lParam == bt_plus  ? (double)plus :
                                      (HWND)lParam == bt_minus ? (double)minus :
                                      (HWND)lParam == bt_multi ? (double)multi :
                                      wParam == VK_ADD      ? (double)plus :
                                      wParam == VK_SUBTRACT ? (double)minus :
                                      wParam == VK_MULTIPLY ? (double)multi : (double)division;
                #ifdef DEBUG
                    printf("masNum = %f\n", masNum[countMasNum]);
                #endif // DEBUG
                countMasNum++;
            }

            if ( ((HWND)lParam == bt_equals || wParam == VK_RETURN ) && countMasNum > 0)
            {
                flagDivideZero = FALSE;
                CalcAndShow();
            }

            if ((HWND)lParam == bt_back || wParam == VK_BACK)
            {
                flagMinus = FALSE;
                flagPoint = FALSE;
                flagDivideZero = FALSE;
                countRazr = 1;
                if (countMasNum >= 2)
                {
                    masNum[countMasNum] = 0;
                    countMasNum -= 2;
                    littleNum = 0;
                    num = masNum[countMasNum];
                    ShowMasNum();
                }
            }

            #ifdef DEBUG
                printf("countMasNum = %d\n", countMasNum);
            #endif // DEBUG

            ShowOutputScreen();

            SetFocus(hWnd);

            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            EndPaint(hWnd, &ps);
            break;

        default:
            return DefWindowProcA (hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

void ShowOutputScreen()
{
    for (int i = 0; i <= countMasNum; i++)
    {
        if (i%2 == 0)
        {
            snprintf(strNum, sizeof(strNum), "%g", masNum[i]);
            strcat(strOutputScreen, strNum);
        }
        else
        {
            strcat(strOutputScreen, masMath[(int)masNum[i]]);
            #ifdef DEBUG
                printf("show = %s\n", masMath[(int)masNum[i]]);
            #endif // DEBUG
        }
    }
    SetWindowText(outputScreen, strOutputScreen);
    memset(strOutputScreen, 0, sizeof(strOutputScreen));
}

double funcPlus(double a, double b)
{
    return a + b;
}

double funcMinus(double a, double b)
{
    return a - b;
}

double funcDivision(double a, double b)
{
    if(b != 0)
        return a / b;
    else
    {
        flagDivideZero = TRUE;
        return a / b;
    }
}

double funcMulti(double a, double b)
{
    return a * b;
}

double funcMath(double num1, double num2, TFunc fn)
{
    return fn(num1, num2);
}

void CalcAndShow()
{
    for(int i = 0; i < countMasNum; i += 2)
    {
        #ifdef DEBUG
            for(int j = 0; j <= countMasNum; j++)
            {
                printf("mas[%d] = %.2f ", j, masNum[j]);
            }
            printf("\n");
        #endif // DEBUG

        if (i == 0)
            result = funcMath(masNum[i], masNum[i+2], Func[(int)masNum[i+1]] );
        else
            result = funcMath(result, masNum[i+2], Func[(int)masNum[i+1]]);
        #ifdef DEBUG
            printf("result = %0.2f\n", result);
        #endif // DEBUG
    }
    snprintf(strNum, sizeof(strNum), "%g", result);
    memset(strInputScreen, 0, sizeof(strInputScreen));
    strcat(strInputScreen, "= ");
    strcat(strInputScreen, strNum);
    if(flagDivideZero)
        strcat(strInputScreen, "\nyou can't divide by zero / на ноль делить нельзя");
    SetWindowText(inputScreen, strInputScreen);
}

void ShowMasNum()
{
    snprintf(strInputScreen, sizeof(strInputScreen), "%g", masNum[countMasNum]);
    SetWindowText(inputScreen, strInputScreen);
}

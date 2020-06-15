// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <cstdio>
#include "Kszta³ty.h"

const int MAX_FIGUR = 8; // maksymalna liczba figur

const int GRANICA_LEWO = 150;
const int GRANICA_PRAWO = 1250;
const int GRANICA_GORA = 90;
const int GRANICA_DOL = 700;

const int SUFIT = 82;
const int PODLOGA = 720;

const int WIEZA_X = 1220; // od tej granicy sprawdza ukladanie wiezy

const int UDZWIG_HAK = 100;
const int MASA_KOLO = 40;
const int MASA_TROJKAT = 50;
const int MASA_KWADRAT = 60;

const int DLUGOSC_BOKU = 50;
const int WYMIAR_HAK = 16;


Hak * hak_wsk;
Ksztalty** wskaznik_na_ksztalty = new Ksztalty*[MAX_FIGUR];   
int ilosc_figur = 0;





#define MAX_LOADSTRING 100
#define TMR_1 1

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name


// buttons
HWND hwndKolo;
HWND hwndKwadrat;
HWND hwndTrojkat;
HWND hwndWszystkie;

RECT drawArea1 = { 101, 81, 1333, 720 };
RECT statyczne_tlo = { 0, 0, 1350, 780 };


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);


void repaintWindow(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea);

bool czy_w_bloku(Ksztalty* ksztalt, Hak* hak) // sprawdza czy hak znajduje sie wewnatrz bloku do podniesienia
{

	if (hak->pobierz_x() > ksztalt->pobierz_x() && ksztalt->pobierz_x() + DLUGOSC_BOKU > hak->pobierz_x() &&
		hak->pobierz_y() > ksztalt->pobierz_y() && ksztalt->pobierz_y() + DLUGOSC_BOKU > hak->pobierz_y())
		return true;


	return false;
}


void sprowadz_na_ziemie(Ksztalty* kogo) // bloki przesuwaja sie o dlugosc swojej sciany w dol
{
	if (kogo != nullptr) {
		if (kogo->pokaz_sasiada() != nullptr)
			sprowadz_na_ziemie(kogo->pokaz_sasiada());
		kogo->ustaw_X_Y(kogo->pobierz_x(), kogo->pobierz_y() + DLUGOSC_BOKU);
	}
}

void czy_jestem_wolny(Ksztalty** wszystkie_ksztalty, Ksztalty* zabieram)
{
	if (zabieram->pobierz_y() > PODLOGA - 2* DLUGOSC_BOKU) // czy blok jest na ziemi
	{
		sprowadz_na_ziemie(zabieram->pokaz_sasiada());
		zabieram->dodaj_sasiada(nullptr);
	}
	else // gdy lezy na innym bloku
	{
		for (int i = 0; i < MAX_FIGUR; i++) // sprawdzam po wszystkich figurach
		{
			if (wszystkie_ksztalty[i] != nullptr) {
				if (wszystkie_ksztalty[i]->pokaz_sasiada() == zabieram) { // jest figura pod ta ktora chce zabrac
					wszystkie_ksztalty[i]->dodaj_sasiada(zabieram->pokaz_sasiada()); // to ustaw jej wskaznik na ta ktora lezy na tej ktora chce zabrac
					zabieram->dodaj_sasiada(nullptr);
					sprowadz_na_ziemie(wszystkie_ksztalty[i]->pokaz_sasiada());
					break;
				}
			}
		}
	}
	
}

void podnies_ladunek(Ksztalty** wszystkie_ksztalty, Hak* hak, int ktory) // jezeli hak ma odpowiedni udzwig to dodaje mu ladunek
{
	if (hak->pokaz_udzwig() > wszystkie_ksztalty[ktory]->pobierz_mase())
	{
		hak->dodaj_ladunek(wszystkie_ksztalty[ktory]);
		czy_jestem_wolny(wszystkie_ksztalty, hak->pokaz_ladunek());
	}
}

void sprawdz_dol(Ksztalty** wszystkie_ksztalty, Ksztalty* nowy) // ustawia figury w wie¿y jedna na drugiej 
{
	Ksztalty* Bufor = nullptr;

	for (int i = 0; i < MAX_FIGUR; i++)
	{
		if (wszystkie_ksztalty[i] != nullptr)
		{
			if ((wszystkie_ksztalty[i]->pobierz_x() - DLUGOSC_BOKU < nowy->pobierz_x() && nowy->pobierz_x() < wszystkie_ksztalty[i]->pobierz_x() + DLUGOSC_BOKU) && wszystkie_ksztalty[i] != nowy)
			{ // gdy jest pod nowa juz jakas figura			
				if (Bufor == nullptr) // ustaw Bufor na szczycie wiezy
					Bufor = wszystkie_ksztalty[i];
				else if (Bufor->pobierz_y() > wszystkie_ksztalty[i]->pobierz_y() )
					Bufor = wszystkie_ksztalty[i];

			}
		}
	}

	if (Bufor == nullptr) // gdy nie znaleziono wiezy ustaw klocek na ziemi
		nowy->ustaw_X_Y(nowy->pobierz_x(), PODLOGA - DLUGOSC_BOKU - 1);
	else // inaczej ustaw na szczycie
	{
		int j = 0;
		while (wszystkie_ksztalty[j] != Bufor)
			j++;

		nowy->ustaw_X_Y(wszystkie_ksztalty[j]->pobierz_x(), wszystkie_ksztalty[j]->pobierz_y() - DLUGOSC_BOKU);
		
		wszystkie_ksztalty[j]->dodaj_sasiada(nowy);
	}
	

	Bufor = nullptr;
	delete Bufor;
}

void czysc_wieze(int id_figury, Ksztalty** wszystkie_ksztalty)
{
	for (int i = 0; i < MAX_FIGUR; i++) // czyszczenie wiezy
	{
		if (wszystkie_ksztalty[i] != nullptr) 
		{
			if (wszystkie_ksztalty[i]->pobierz_x() > WIEZA_X && wszystkie_ksztalty[i]->id() != id_figury) // gdy na wiezy jest niechciana figura
			{
				czy_jestem_wolny(wszystkie_ksztalty, wszystkie_ksztalty[i]); // zabierz z wiezy
				wszystkie_ksztalty[i]->ustaw_X_Y(200, wszystkie_ksztalty[i]->pobierz_y()); // ustaw na x = 200
				sprawdz_dol(wszystkie_ksztalty, wszystkie_ksztalty[i]);
			}
		}
	}
}

void wysokosc_wiezy(Ksztalty** wszystkie_ksztalty) // sprawdza czy wieza niej jest wy¿sza ni¿ 3 klocki
{
	Ksztalty* Bufor = nullptr;

	for (int i = 0; i < MAX_FIGUR; i++)
	{
		if (wszystkie_ksztalty[i] != nullptr)
		{
			if (wszystkie_ksztalty[i]->pobierz_x() > WIEZA_X)
			{
				if (wszystkie_ksztalty[i]->pobierz_y() < PODLOGA - 3* DLUGOSC_BOKU - 2)
				{
					czy_jestem_wolny(wszystkie_ksztalty, wszystkie_ksztalty[i]);
					wszystkie_ksztalty[i]->ustaw_X_Y(200, PODLOGA - DLUGOSC_BOKU - 1);
					sprawdz_dol(wszystkie_ksztalty, wszystkie_ksztalty[i]);
				}
			}
		}
	}

	Bufor == nullptr;
	delete Bufor;
}

void opusc_ladunek(Ksztalty** wszystkie_ksztalty, Hak* hak) 
{
	if (hak != nullptr)
	{
		hak->pokaz_ladunek()->ustaw_X_Y(hak->pokaz_ladunek()->pobierz_x(), PODLOGA - DLUGOSC_BOKU - 1);
		sprawdz_dol(wszystkie_ksztalty, hak->pokaz_ladunek());
		hak->opusc_ladunek();

		if (hak->pobierz_x() > WIEZA_X)
			wysokosc_wiezy(wszystkie_ksztalty);
	}
}

void ustaw_hak_na_poz(Ksztalty* wszystkie_ksztalty, Hak* hak, int ox, int oy, HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea) // do automatycznego ustawiania wiezy
{	
	while (hak->pobierz_y() < oy){
		hak->dodaj_y(1);
		Sleep(5);
		repaintWindow(hWnd, hdc, ps, drawArea);
	}
	while (hak->pobierz_y() > oy){
		hak->dodaj_y(-1);
		Sleep(5);
		repaintWindow(hWnd, hdc, ps, drawArea);
	}
	while (hak->pobierz_x() < ox) {
		hak->dodaj_x(1);
		Sleep(5);
		repaintWindow(hWnd, hdc, ps, drawArea);
	}
	while (hak->pobierz_x() > ox) {
		hak->dodaj_x(-1);
		Sleep(5);
		repaintWindow(hWnd, hdc, ps, drawArea);
	}
}


void automatyczna_wieza(Ksztalty** wszystkie_ksztalty, Hak* hak, int podstawa, int srodek, int szczyt, HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea)
{
	int ktory = 0; // 0=>podstawa, 1=>srodek, 2=>szczyt
	for (int i = 0; i < MAX_FIGUR; i++) // szuka po wszystkich figurach
	{
		if (wszystkie_ksztalty[i] != nullptr)
		{
			if ((wszystkie_ksztalty[i]->id() == podstawa) && (ktory == 0)) { // szukamy potrzebnej figury
				ustaw_hak_na_poz(wszystkie_ksztalty[i], hak, wszystkie_ksztalty[i]->pobierz_x() + DLUGOSC_BOKU/2, wszystkie_ksztalty[i]->pobierz_y() + DLUGOSC_BOKU/2,  hWnd, hdc, ps, drawArea);
				
				podnies_ladunek(wszystkie_ksztalty, hak, i);
				ustaw_hak_na_poz(wszystkie_ksztalty[i], hak, GRANICA_PRAWO, 600, hWnd, hdc, ps, drawArea);
				opusc_ladunek(wszystkie_ksztalty, hak);
			
				i = -1;
				ktory++;
			}
			else if ((wszystkie_ksztalty[i]->id() == srodek) && (ktory == 1)) {
				ustaw_hak_na_poz(wszystkie_ksztalty[i], hak, wszystkie_ksztalty[i]->pobierz_x() + DLUGOSC_BOKU/2, wszystkie_ksztalty[i]->pobierz_y() + DLUGOSC_BOKU/2,  hWnd, hdc, ps, drawArea);
		
				podnies_ladunek(wszystkie_ksztalty, hak, i);
				ustaw_hak_na_poz(wszystkie_ksztalty[i], hak, GRANICA_PRAWO, 600, hWnd, hdc, ps, drawArea);
				opusc_ladunek(wszystkie_ksztalty, hak);
			
				i = -1;
				ktory++;
			}
			else if ((wszystkie_ksztalty[i]->id() == szczyt) && (ktory == 2)) {
				ustaw_hak_na_poz(wszystkie_ksztalty[i], hak, wszystkie_ksztalty[i]->pobierz_x() + DLUGOSC_BOKU/2, wszystkie_ksztalty[i]->pobierz_y() + DLUGOSC_BOKU/2, hWnd, hdc, ps, drawArea);
			
				podnies_ladunek(wszystkie_ksztalty, hak, i);
				ustaw_hak_na_poz(wszystkie_ksztalty[i], hak, GRANICA_PRAWO, 600, hWnd, hdc, ps, drawArea);
				opusc_ladunek(wszystkie_ksztalty, hak);

				break;
			}
		}
	}
	repaintWindow(hWnd, hdc, ps, drawArea);
}



void Rysuj_tlo(HWND hWnd, HDC hdc, PAINTSTRUCT& ps ) // rysuje dzwig i go wiecej nie odswieza
{
	InvalidateRect(hWnd, &statyczne_tlo, TRUE); // repaint all
	hdc = BeginPaint(hWnd, &ps);

	Graphics graphics(hdc);

	Pen pen(Color(0,0,0)); // czarny
	graphics.DrawRectangle(&pen, 1, 1, 1348, 770); // Ramka
	
	pen.SetColor(Color(115, 250, 56)); // zielony
	graphics.DrawRectangle(&pen, 1, 721, 1347, 48); // Pod³oga
	pen.SetColor(Color(0,0,0));
	graphics.DrawLine(&pen, WIEZA_X, 721, 1347, 721);
	pen.SetColor(Color(255, 255, 50)); // zolty
	graphics.DrawRectangle(&pen, 20, 10, 80, 710); // dzwig
	graphics.DrawRectangle(&pen, 5, 30, 1335, 50);
	
	hak_wsk->rysuj(hdc, SUFIT);
	
	EndPaint(hWnd, &ps);
}



void MyOnPaint(HDC hdc, HWND hWnd, PAINTSTRUCT& ps)
{
	hak_wsk->rysuj(hdc, SUFIT); // rysuj hak i line
	
	for (int i = 0; i < MAX_FIGUR; i++) // sprawdz wszystkie figury, jezeli istnieje to ja narysuj
		if (wskaznik_na_ksztalty[i] != nullptr)
			wskaznik_na_ksztalty[i]->rysuj(hdc);
}

void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	
	InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	MyOnPaint(hdc, hWnd, ps);
	EndPaint(hWnd, &ps);
}

int OnCreate(HWND window)
{
	hak_wsk = new Hak(200, 200, UDZWIG_HAK, WYMIAR_HAK); // stworz hak
	

	for (int i = 0; i < MAX_FIGUR; i++) // ustaw brak figur
		wskaznik_na_ksztalty[i] = nullptr;
	
	return 0;
}


// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	//value = 0;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}



	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;


	hInst = hInstance; // Store instance handle (of exe) in our global variable

	// main window
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, 1850, 850, NULL, NULL, hInstance, NULL);


	// create button and store the handle  	
	hwndKwadrat = CreateWindow(TEXT("button"), TEXT("kwadrat"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		1450, 200, 100, 30, hWnd, (HMENU)ID_KWADRAT, GetModuleHandle(NULL), NULL);

	hwndTrojkat = CreateWindow(TEXT("button"), TEXT("trojkat"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		1450, 245, 100, 30, hWnd, (HMENU)ID_TROJKAT, GetModuleHandle(NULL), NULL);

	hwndKolo = CreateWindow(TEXT("button"), TEXT("kolo"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		1450, 290, 100, 30, hWnd, (HMENU)ID_KOLO, GetModuleHandle(NULL), NULL);
	hwndWszystkie = CreateWindow(TEXT("button"), TEXT("wszystkie"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		1450, 335, 100, 30, hWnd, (HMENU)ID_WSZYSTKIE, GetModuleHandle(NULL), NULL);
	SendMessage(hwndWszystkie, BM_SETCHECK, BST_CHECKED, 0);
	
	
	OnCreate(hWnd);
	

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window (low priority)
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;


	
	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// MENU & BUTTON messages
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			delete hak_wsk;

			for (int i = 0; i < MAX_FIGUR; i++)
			{
				delete[] wskaznik_na_ksztalty[i];
				wskaznik_na_ksztalty[i] = nullptr;
			}

			delete[] wskaznik_na_ksztalty;
			wskaznik_na_ksztalty = nullptr;

			DestroyWindow(hWnd);
			break;

		case ID_KWADRAT:
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			break;
		case ID_KOLO:
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			break;
		case ID_TROJKAT:
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			break;
		
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);
		Rysuj_tlo(hWnd, hdc, ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
	{
		switch ((int)wParam)
		{
		case VK_RIGHT:
			if (hak_wsk->pobierz_x() < GRANICA_PRAWO) {
				hak_wsk->dodaj_x(3);
				repaintWindow(hWnd, hdc, ps, &drawArea1);
			}
			break;

		case VK_LEFT:
			if (hak_wsk->pobierz_x() > GRANICA_LEWO) {
				hak_wsk->dodaj_x(-3);
				repaintWindow(hWnd, hdc, ps, &drawArea1);
			}
			break;

		case VK_UP:
			if (hak_wsk->pobierz_y() > GRANICA_GORA) {
				hak_wsk->dodaj_y(-3);
				repaintWindow(hWnd, hdc, ps, &drawArea1);
			}
			break;

		case VK_DOWN:
			if (hak_wsk->pobierz_y() < GRANICA_DOL) {
				hak_wsk->dodaj_y(3);
				repaintWindow(hWnd, hdc, ps, &drawArea1);
			}
			break;

		case VK_RETURN: // tworzy nowa figure
			if (ilosc_figur < MAX_FIGUR && IsDlgButtonChecked(hWnd, ID_WSZYSTKIE) == BST_UNCHECKED)
			{
				ilosc_figur++;
				if (IsDlgButtonChecked(hWnd, ID_KOLO) == BST_CHECKED) {
					wskaznik_na_ksztalty[ilosc_figur] = new Kolo(hak_wsk->pobierz_x(), PODLOGA - DLUGOSC_BOKU - 1, MASA_KOLO, DLUGOSC_BOKU);	
				}
				else if (IsDlgButtonChecked(hWnd, ID_KWADRAT) == BST_CHECKED) {
					wskaznik_na_ksztalty[ilosc_figur] = new Kwadrat(hak_wsk->pobierz_x(), PODLOGA - DLUGOSC_BOKU - 1, MASA_KWADRAT, DLUGOSC_BOKU);
				}
				else if (IsDlgButtonChecked(hWnd, ID_TROJKAT) == BST_CHECKED) {
					wskaznik_na_ksztalty[ilosc_figur] = new Trojkat(hak_wsk->pobierz_x(), PODLOGA - DLUGOSC_BOKU - 1, MASA_TROJKAT, DLUGOSC_BOKU);
				}
				
				sprawdz_dol(wskaznik_na_ksztalty, wskaznik_na_ksztalty[ilosc_figur]); // ustawia klocek na odpowiedniej wysokosci

				if (hak_wsk->pobierz_x() > WIEZA_X) // jezeli hak jest w obszarze budowania wiezy
					wysokosc_wiezy(wskaznik_na_ksztalty);
					
				repaintWindow(hWnd, hdc, ps, &drawArea1);
				
			}
			break;

		case VK_SPACE:
			if (!hak_wsk->czy_ladunek()) { // gdy nie ma ladunku na haku
				for (int i = 0; i < MAX_FIGUR; i++){
					if (wskaznik_na_ksztalty[i] != nullptr) {

						if(czy_w_bloku(wskaznik_na_ksztalty[i], hak_wsk)){

							if (((IsDlgButtonChecked(hWnd, ID_KOLO) == BST_CHECKED && ID_KOLO == wskaznik_na_ksztalty[i]->id()) || IsDlgButtonChecked(hWnd, ID_WSZYSTKIE) == BST_CHECKED)) {
								podnies_ladunek(wskaznik_na_ksztalty, hak_wsk, i);
								break;
							}
							else if (((IsDlgButtonChecked(hWnd, ID_KWADRAT) == BST_CHECKED && ID_KWADRAT == wskaznik_na_ksztalty[i]->id() ) || IsDlgButtonChecked(hWnd, ID_WSZYSTKIE) == BST_CHECKED)) {
								podnies_ladunek(wskaznik_na_ksztalty, hak_wsk, i);
								break;
							}
							else if (((IsDlgButtonChecked(hWnd, ID_TROJKAT) == BST_CHECKED && ID_TROJKAT == wskaznik_na_ksztalty[i]->id()) || IsDlgButtonChecked(hWnd, ID_WSZYSTKIE) == BST_CHECKED)) {
								podnies_ladunek(wskaznik_na_ksztalty, hak_wsk, i);
								break;
							}
						}
						
					}
				}
			}
			else { // gdy jest ladunek na haku
				opusc_ladunek(wskaznik_na_ksztalty, hak_wsk);
				repaintWindow(hWnd, hdc, ps, &drawArea1);		
			}
			break;
		case 0x31:
		case VK_NUMPAD1:// ustawia kwadrat
			SendMessage(hwndKwadrat, BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(hwndKolo, BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(hwndTrojkat, BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(hwndWszystkie, BM_SETCHECK, BST_UNCHECKED, 0);

			czysc_wieze(ID_KWADRAT, wskaznik_na_ksztalty);
			break;
		case 0x32:
		case VK_NUMPAD2:// ustawia trojkat
			SendMessage(hwndTrojkat, BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(hwndKwadrat, BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(hwndKolo, BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(hwndWszystkie, BM_SETCHECK, BST_UNCHECKED, 0);

			czysc_wieze(ID_TROJKAT, wskaznik_na_ksztalty);
			break;
		case 0x33:
		case VK_NUMPAD3:// ustawia kolo
			SendMessage(hwndKolo, BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(hwndKwadrat, BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(hwndTrojkat, BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(hwndWszystkie, BM_SETCHECK, BST_UNCHECKED, 0);

			czysc_wieze(ID_KOLO, wskaznik_na_ksztalty);
			break;
		case 0x34:
		case VK_NUMPAD4: // ustawia wszystkie ksztalty
			SendMessage(hwndWszystkie, BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(hwndKwadrat, BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(hwndKolo, BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(hwndTrojkat, BM_SETCHECK, BST_UNCHECKED, 0);

			break;
		case 0x38:
		case VK_NUMPAD8: // po wcisnieciu 8 ustawia automatycznie: kwadrat, trojkat, kolo
			if (IsDlgButtonChecked(hWnd, ID_WSZYSTKIE) == BST_CHECKED) {
				czysc_wieze(0, wskaznik_na_ksztalty);
				automatyczna_wieza(wskaznik_na_ksztalty, hak_wsk, ID_KWADRAT, ID_TROJKAT, ID_KOLO, hWnd, hdc, ps, &drawArea1);
			}
			break;
		case 0x39:
		case VK_NUMPAD9: // po wcisnieciu 9 ustawia automatycznie: kolo, kwadrat, trojkat
			if (IsDlgButtonChecked(hWnd, ID_WSZYSTKIE) == BST_CHECKED) {
				czysc_wieze(0, wskaznik_na_ksztalty);
				automatyczna_wieza(wskaznik_na_ksztalty, hak_wsk, ID_KOLO, ID_KWADRAT, ID_TROJKAT, hWnd, hdc, ps, &drawArea1);
			}
			break;
		}
	}


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

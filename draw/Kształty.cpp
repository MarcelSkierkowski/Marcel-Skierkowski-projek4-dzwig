#include "stdafx.h"
#include "Kszta³ty.h"

//   KSZTALTY KOLO   //
Kolo::Kolo(int x, int y, int kg, int wymiar)
{
	poz_x = x; 
	poz_y = y;
	masa = kg;
	bok = wymiar;
	ID = 210; 
	sasiad = nullptr;
}

Kolo::~Kolo()
{
	delete sasiad;
}

Ksztalty* Kolo:: pokaz_sasiada()
{
	return sasiad;
}

int Kolo::pobierz_x()
{
	return poz_x;
}

int Kolo::pobierz_y()
{
	return poz_y;
}

 int Kolo::id()
 {
	 return ID;
 }

 int Kolo::pobierz_mase()
 {
	 return masa;
 }

 void Kolo::dodaj_x(int ile)
 {
	 poz_x = poz_x + ile;
 }

 void Kolo::dodaj_y(int ile)
 {
	 poz_y = poz_y + ile;
 }

 void Kolo::ustaw_X_Y(int x, int y)
 {
	 poz_x = x;
	 poz_y = y;
 }

 void Kolo::dodaj_sasiada(Ksztalty* wsk)
 {
	 sasiad = wsk;
 }

 void Kolo::rysuj(HDC hdc)
 {
	 Graphics graphics(hdc);
	 Pen pen(Color(0, 0, 0));
	 graphics.DrawEllipse(&pen, poz_x, poz_y, bok, bok);
 }

 // KSZTALTY KWADRAT  // 

 Kwadrat::Kwadrat(int x, int y, int kg, int wymiar)
 {
	 poz_x = x;
	 poz_y = y;
	 masa = kg;
	 bok = wymiar;
	 ID = 201;
	 sasiad = nullptr;
 }

 Kwadrat::~Kwadrat()
 {
	 delete sasiad;
 }

 Ksztalty* Kwadrat::pokaz_sasiada()
 {
	 return sasiad;
 }

 int Kwadrat::pobierz_x()
 {
	 return poz_x;
 }

 int Kwadrat::pobierz_y()
 {
	 return poz_y;
 }

 int Kwadrat::id()
 {
	 return ID;
 }

 int Kwadrat::pobierz_mase()
 {
	 return masa;
 }

 void Kwadrat::dodaj_x(int ile)
 {
	 poz_x = poz_x + ile;
 }

 void Kwadrat::dodaj_y(int ile)
 {
	 poz_y = poz_y + ile;
 }

 void Kwadrat::ustaw_X_Y(int x, int y)
 {
	 poz_x = x;
	 poz_y = y;
 }

 void Kwadrat::dodaj_sasiada(Ksztalty* wsk)
 {
	 sasiad = wsk;
 }

 void Kwadrat::rysuj(HDC hdc)
 {
	 Graphics graphics(hdc);
	 Pen pen(Color(0, 0, 0));
	 graphics.DrawRectangle(&pen, poz_x, poz_y, bok, bok);
 }

 // KSZTALTY  TROJKAT  ///

 Trojkat::Trojkat(int x, int y, int kg, int wymiar)
 {
	 poz_x = x;
	 poz_y = y;
	 masa = kg;
	 bok = wymiar;
	 ID = 200;
	 sasiad = nullptr;
 }

 Trojkat::~Trojkat()
 {
	 delete sasiad;
 }

 Ksztalty* Trojkat::pokaz_sasiada()
 {
	 return sasiad;
 }

 int Trojkat::pobierz_x()
 {
	 return poz_x;
 }

 int Trojkat::pobierz_y()
 {
	 return poz_y;
 }

 int Trojkat::id()
 {
	 return ID;
 }

 int Trojkat::pobierz_mase()
 {
	 return masa;
 }

 void Trojkat::dodaj_x(int ile)
 {
	 poz_x = poz_x + ile;
 }

 void Trojkat::dodaj_y(int ile)
 {
	 poz_y = poz_y + ile;
 }

 void Trojkat::ustaw_X_Y(int x, int y)
 {
	 poz_x = x;
	 poz_y = y;
 }

 void Trojkat::dodaj_sasiada(Ksztalty* wsk)
 {
	 sasiad = wsk;
 }

 void Trojkat::rysuj(HDC hdc)
 {
	 Graphics graphics(hdc);
	 Pen pen(Color(0, 0, 0));
	 graphics.DrawLine(&pen, poz_x, poz_y + bok, poz_x + bok, poz_y + bok);
	 graphics.DrawLine(&pen, poz_x, poz_y + bok, poz_x + bok / 2, poz_y);
	 graphics.DrawLine(&pen, poz_x + bok / 2, poz_y, poz_x + bok, poz_y + bok);
 }

 //  HAK ///

 Hak::Hak(int x, int y, int kg, int wymiar)
 {
	 poz_x = x;
	 poz_y = y;
	 udzwig = kg;
	 bok = wymiar;
	 wsk = nullptr;
 }

 Hak::~Hak()
 {
	 delete wsk;
 }

 Ksztalty* Hak::pokaz_ladunek()
 {
	 return wsk;
 }

 int Hak::pobierz_x()
 {
	 return poz_x;
 }

 int Hak::pobierz_y()
 {
	 return poz_y;
 }

 int Hak::pokaz_udzwig()
 {
	 return udzwig;
 }

 void Hak::dodaj_x(int ile)
 {
	 poz_x = poz_x + ile;
	 if (wsk != nullptr)
		wsk->dodaj_x(ile); 
 }

 void Hak::dodaj_y(int ile)
 {
	 poz_y = poz_y + ile; 
	 if (wsk != nullptr)
		wsk->dodaj_y(ile); 
 }

 void Hak::dodaj_ladunek(Ksztalty* nowy_ladunek)
 {
	 wsk = nowy_ladunek;
 }

 void Hak::opusc_ladunek()
 {
	 wsk = nullptr;
 }

 bool Hak::czy_ladunek()
 {
	 if (wsk != nullptr)
	   return true; 
	 else 
	   return false; 
 }

 void Hak::rysuj(HDC hdc, int SUFIT)
 {
	 Graphics graphics(hdc);
	 Pen pen(Color(255, 0, 0));
	 graphics.DrawRectangle(&pen, poz_x, poz_y, bok, bok);  // hak

	 pen.SetColor(Color(0, 0, 0)); // czarny
	 graphics.DrawLine(&pen, poz_x + bok / 2, SUFIT, poz_x + bok / 2, poz_y); // lina

 }
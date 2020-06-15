#pragma once

class Ksztalty
{
public:
	virtual Ksztalty* pokaz_sasiada() = 0;
	virtual int pobierz_x() = 0;
	virtual int pobierz_y() = 0;
	virtual int id() = 0;
	virtual int pobierz_mase() = 0;
	virtual void dodaj_x(int) = 0; // do aktualnego x dodaje wartosc int
	virtual void dodaj_y(int) = 0; 
	virtual void ustaw_X_Y(int, int) = 0;
	virtual void dodaj_sasiada(Ksztalty* wsk) = 0;
	virtual void rysuj(HDC) = 0; // funkcja rysujaca ksztalt
};

class Kolo : public Ksztalty
{
public:
	Kolo(int x, int y, int kg, int wymiar);
	~Kolo();

	virtual Ksztalty* pokaz_sasiada();
	virtual int pobierz_x();
	virtual int pobierz_y();
	virtual int id();
	virtual int pobierz_mase();
	virtual void dodaj_x(int ile);
	virtual void dodaj_y(int ile);
	virtual void ustaw_X_Y(int x, int y);
	virtual void dodaj_sasiada(Ksztalty* wsk);
	virtual void rysuj(HDC hdc);
	
private:
	int ID;
	int poz_x;
	int poz_y;
	int masa;
	int bok;
	Ksztalty* sasiad;
};

class Kwadrat : public Ksztalty
{
public:
	Kwadrat(int x, int y, int kg, int wymiar);
	~Kwadrat();

	virtual Ksztalty* pokaz_sasiada();
	virtual int pobierz_x();
	virtual int pobierz_y();
	virtual int id();
	virtual int pobierz_mase();
	virtual void dodaj_x(int ile);
	virtual void dodaj_y(int ile);
	virtual void dodaj_sasiada(Ksztalty* wsk);
	virtual void ustaw_X_Y(int x, int y);
	virtual void rysuj(HDC hdc);

private:
	int ID;
	int poz_x;
	int poz_y;
	int masa;
	int bok;
	Ksztalty* sasiad;
};

class Trojkat : public Ksztalty
{
public:
	Trojkat(int x, int y, int kg, int wymiar);
	~Trojkat();

	virtual Ksztalty* pokaz_sasiada();
	virtual int pobierz_x();
	virtual int pobierz_y();
	virtual int id();
	virtual int pobierz_mase();
	virtual void dodaj_x(int ile);
	virtual void dodaj_y(int ile);
	virtual void dodaj_sasiada(Ksztalty* wsk);
	virtual void ustaw_X_Y(int x, int y);
	virtual void rysuj(HDC hdc);
	
private:
	int ID;
	int poz_x;
	int poz_y;
	int masa;
	int bok;
	Ksztalty* sasiad;
};

class Hak
{
public:
	Hak(int x, int y, int kg, int wymiar);
	~Hak();

	Ksztalty* pokaz_ladunek();
	int pobierz_x();
	int pobierz_y();
	int pokaz_udzwig();
	void dodaj_x(int ile);
	void dodaj_y(int ile);
	void dodaj_ladunek(Ksztalty* nowy_ladunek);
	void opusc_ladunek();
	bool czy_ladunek();
	void rysuj(HDC hdc, int SUFIT);
	
private:
	int bok;
	int poz_x;
	int poz_y;
	int udzwig;
	Ksztalty* wsk;
};

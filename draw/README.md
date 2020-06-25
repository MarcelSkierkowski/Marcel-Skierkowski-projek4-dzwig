========================================================================

Opis działania
========================================================================


Strzałki -> poruszanie hakiem

Spacja -> łapanie lub opuszczanie bloków

Enter -> Tworzenie wybranego bloku (zaznaczonego po prawej)

Numpad 1 do 4 -> wybieranie bloku

Numpad 8, Numpad 9 -> automatyczne ustawianie wieży


Sprawdzanie wysokości oraz poprawności wieży odbywa się na czarnym obszarze po prawej


========================================================================
    WIN32 APPLICATION : draw2 Project Overview
========================================================================

draw.vcxproj
    Jest to główny plik projektu dla projektów VC ++.
	Zawiera informacje o wersji Visual C ++, która wygenerowała plik, oraz
    informacje o platformach, konfiguracjach i funkcjach projektu 

draw.vcxproj.filters
    To jest plik filtrów dla projektów VC ++ wygenerowanych za pomocą Kreatora aplikacji.
    Zawiera informacje o powiązaniu między plikami w projekcie

draw2.cpp
  Główny plik z kodem źródłowym

Ksztalty.h
 Plik nagłówkowy zawierający opis  obiekty zwiazane z tworzeniem figur
 
 Ksztalty.cpp
 Plik z kodem opisujacym kształty
 


/////////////////////////////////////////////////////////////////////////////
AppWizard utworzył następujące zasoby:

draw2.rc
    To jest lista wszystkich zasobów Microsoft Windows, do
    zastosowania w programie. Zawiera przechowywane ikony, mapy bitowe i kursory

Resource.h
     Jest to standardowy plik nagłówka, który definiuje nowe identyfikatory zasobów.
     Microsoft Visual C ++ czyta i aktualizuje ten plik.

draw2.ico
     Jest to plik ikon, które są używane jako ikona aplikacji (32 x 32).
     Ta ikona jest uwzględniona w głównym pliku zasobów draw.rc.

small.ico
     To jest plik ikon, który zawiera mniejszą wersję (16 x 16)
     ikony aplikacji. Ta ikona jest uwzględniona przez główny zasób
     plik draw.rc.

/////////////////////////////////////////////////////////////////////////////
Inne standardowe pliki:

StdAfx.h, StdAfx.cpp
     Pliki te są używane do budowy wstępnie skompilowanego pliku nagłówka (PCH)
     o nazwie draw.pch i wstępnie skompilowany plik typów o nazwie StdAfx.obj.

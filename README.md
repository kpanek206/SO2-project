# SO2-project
## P1 - Rozwiązanie problemu ucztujących filozofów
## P2 - Symulator kopalni i jubilera:
### Wątki:
*	Dostęp do nowych złóż kopalni (jeden wątek)
*	Pracownicy kopalni – wydobycie zasobów (kilka wątków)
*	Transport zasobów (dwa wątki)
*	Złotnik – przerobienie wydobytych zasobów na nadające się dla jubilera (kilkanaście wątków)
*	Jubiler – wytwarza produkt do sprzedaży (kilkanaście wątków)
*	Klient – kupuje produkt od jubilera (ok. trzy wątki)
### Zasoby:
*	Złoża kopalni
*	Wydobyte zasoby
*	Zasoby przerobione przez złotnika
*	Produkty sprzedawane przez jubilera
*	Pieniądze ze sprzedaży biżuterii 
### Opis działania:
Najpierw pozyskujemy dostęp do złóż kopalni. Następnie pracownicy kopalni wydobywają dostępne zasoby, żeby później przetransportować je do złotnika, który przerobi zasoby dla jubilera. Jubiler wytwarza z przerobionych zasobów biżuterię, a następnie sprzedaje ją klientowi. Z pieniędzy uzyskanych ze sprzedaży uzyskujemy dostęp do nowych złóż kopalni.

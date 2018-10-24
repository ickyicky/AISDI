# AISDI - projekt z algorytmów grafowych

*Mostem* w grafie spójnym nazywamy krawędź, której usunięcie powoduje jego rozspójnienie (powstały w wyniku usunięcia krawędzi graf jest niespójny). Na potrzeby niniejszego zadania, *mostem rozległym* w grafie spójnym będziemy nazywać taką krawędź, że usunięcię obu jej końców (wierzchołków) powoduje rozspójnienie grafu. Operacja usunięcia wierzchołka oznacza również usunięcie wszystkich krawędzi, których jest końcem.

Proszę napisać program, który:

1. Wczyta spójny graf nieskierowany ze standardowego wejścia.
2. Znajdzie i wyświetli na ekranie wszystkie mosty rozległe w tym grafie.

Przyjmujemy, że krawędzie grafu mogą być tylko jednokrotne, a wierzchołki nie mogą być połączone krawędzią same ze sobą. Graf pusty (bez wierzchołków) traktujemy jako spójny.

## Przykładowy sposób uruchomienia programu

    >./program < plik_z_opisem_grafu

## Format opisu grafu

W pierwszej linii znajduje się dodatnia liczba całkowita oznaczająca liczbę wszystkich wierzchołków w grafie. Poniżej, dla każdej krawędzi w grafie, znajduje się linia z dwoma nieujemnymi liczbami całkowitymi, oznaczającymi numery wierzchołków połączonych tą krawędzią. Numeracja wierzchołków zaczyna się od 0. Liczby w jednej linii oddzielone są znakiem spacji. Można założyć poprawność wczytywanego opisu.

## Wynik działania programu

Wynikiem działania programu powinno być wyświetlenie na ekranie wszystkich mostów rozległych. Każdy most rozległy (krawędź) powinien być wyświetlony w postaci odrębnej linii jak w opisie grafu (linia powinna zawierać numery końcowych wierzchołków krawędzi, oddzielone znakiem spacji). W przypadku, gdy w grafie nie ma mostów rozległych, program nie powinien niczego wyświetlać.

## Przykład 1

graf.txt:

    3
    0 1
    1 2
    2 0

wynik:

    >./program < graf.txt
    >

## Przykład 2

graf.txt:

    4
    0 1
    1 2
    2 3
    3 0
    0 2

wynik:

    >./program < graf.txt
    0 2
    >

lub:

    >./program < graf.txt
    2 0
    >

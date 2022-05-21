#include <stdlib.h>
#include <sys/stat.h>

#include <fmt/inc/core.h>
#include <fmt/inc/format.h>
#include <fmt/inc/format-inl.h>
#include <fmt/src/format.cc>
#include <Magick++.h>
#include <argparse.h>

using namespace std;
using namespace Magick;
using namespace argparse;

int main( int argc, const char * args[] ) {
    
    string arg_obraz = "";
    int arg_sila = 3, arg_algorytm = 1;
    ArgumentParser parser("szumovski", "- redukcja szumow ala Szymon Pokrywka");
    parser.add_argument("-i", "--obraz", "sciezka do obrazu", true);
    parser.add_argument("-a", "--algo", "zastosowany algorytm (0-1) = 0", false);
    parser.add_argument("-s", "--sila", "intensywnosc algorytmu (1-10) = 3",    false);
    parser.enable_help(); auto err = parser.parse( argc, args );
    
    if (parser.exists("help")) {
        parser.print_help(); return 0; }
    if (parser.exists("obraz")) {
        arg_obraz = parser.get<string>("obraz");
        struct stat bufor; // Sprawdz czy podana sciezka istnieje
        auto plik_istnieje = (stat( arg_obraz.c_str(), &bufor) == 0 ); 
        if ( plik_istnieje == 0 ) {
            fmt::print("# Nie podano poprawnej sciezki! #"); return 1;
        }
    } else { fmt::print("# Nie znaleziono sciezki! #"); return 1; }
    if (parser.exists("algo")) {
        arg_algorytm = parser.get<int>("algo");
    }
    if (parser.exists("sila")) {
        arg_sila = parser.get<int>("sila");
    }

    if (err) {
        fmt::print("# Wystapil blad w przetwarzaniu argumentow: [ {} ] #\n", err); return -1; }
    fmt::print("# Stosuje parametry: obraz = {}; algo = {}; sila = {}; #\n",
            arg_obraz, arg_algorytm, arg_sila);
    InitializeMagick( * args );
    Image obraz;
    try { obraz.read( arg_obraz );
        switch( arg_algorytm ) {
            default:
                obraz.adaptiveBlur( 2 );
                obraz.reduceNoise( arg_sila );
                obraz.adaptiveSharpen( arg_sila * 3);
                break;
            case 1:
                obraz.reduceNoise( arg_sila );
                obraz.adaptiveSharpen( arg_sila * 2 );
                break;
        }
        obraz.write( "szumovski_" + arg_obraz );
    } catch( Exception &blad ) {
        fmt::print("# Wystapil nieznany blad! #");
        return 1;
    }
    
    return 0;
}

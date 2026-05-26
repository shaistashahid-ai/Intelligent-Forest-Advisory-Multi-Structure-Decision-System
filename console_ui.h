#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

#ifdef _WIN32
  #include <windows.h>
  inline void enableANSI() 
  {
      HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
      DWORD mode = 0;
      GetConsoleMode(h, &mode);
      SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
  }

#else
  inline void enableANSI() {}   
#endif

// Raw ANSI codes
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"

// Text colours
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

// Bright variants
#define BRED        "\033[91m"
#define BGREEN      "\033[92m"
#define BYELLOW     "\033[93m"
#define BBLUE       "\033[94m"
#define BMAGENTA    "\033[95m"
#define BCYAN       "\033[96m"
#define BWHITE      "\033[97m"

// Background colours
#define BG_RED      "\033[41m"
#define BG_GREEN    "\033[42m"
#define BG_YELLOW   "\033[43m"
#define BG_BLUE     "\033[44m"
#define BG_CYAN     "\033[46m"

// Status symbols
#define SYM_OK      BGREEN  "[OK]"      RESET
#define SYM_WARN    BYELLOW "[WARN]"    RESET
#define SYM_ERR     BRED    "[FAIL]"    RESET
#define SYM_FIRE    BRED    "[FIRE]"    RESET
#define SYM_INFO    BCYAN   "[INFO]"    RESET
#define SYM_MON     BBLUE   "[MON]"     RESET
#define SYM_ARROW   CYAN    " --> "     RESET

// Banner / Section headers 
inline void printBanner(const std::string& title) 
{
    int width = 52;
    string line(width, '=');
    int pad = (width - (int)title.size() - 2) / 2;
    string spaces(pad > 0 ? pad : 0, ' ');

    cout << endl << BOLD << BCYAN << line << RESET << endl;
    cout << BOLD << BCYAN << " " << spaces << title << RESET << endl;
    cout << BOLD << BCYAN << line << RESET << endl;
}

inline void printSubHeader(const std::string& title) 
{
    cout << endl << BOLD << BYELLOW << "  >> " << title << RESET << endl;
    cout << YELLOW << "  " << std::string(title.size() + 5, '-') << RESET << endl;
}

// Divider line 
inline void printDivider() 
{
    cout << DIM << "  " << std::string(50, '-') << RESET << endl;
}

// Status badge helpers 
inline void printOK   (const std::string& msg) { std::cout << "  " << SYM_OK   << "  " << msg << endl; }
inline void printWarn (const std::string& msg) { std::cout << "  " << SYM_WARN << "  " << BYELLOW << msg << RESET << endl; }
inline void printError(const std::string& msg) { std::cout << "  " << SYM_ERR  << "  " << BRED    << msg << RESET << endl; }
inline void printFire (const std::string& msg) { std::cout << "  " << SYM_FIRE << "  " << BRED    << BOLD << msg << RESET << endl; }
inline void printInfo (const std::string& msg) { std::cout << "  " << SYM_INFO << "  " << msg << endl; }
inline void printMon  (const std::string& msg) { std::cout << "  " << SYM_MON  << "  " << BBLUE << msg << RESET << endl; }

// Simple one-row key/value line 
inline void printKV(const std::string& key, const std::string& val, const char* valColor = BWHITE) 
{
    cout << "  " << CYAN << std::left << std::setw(24) << key << RESET << valColor << val << RESET << endl;
}

// Table helpers 
//  printTableRow: prints one row of a fixed-width 2-column table
inline void printTableHeader(const std::string& c1, const std::string& c2, int w1 = 10, int w2 = 22)
{
    cout << endl << "  " << BOLD << BG_BLUE << WHITE << " " << std::left << std::setw(w1) << c1 << " | " << std::setw(w2) << c2 << " "  << RESET << endl;
    cout << "  " << CYAN << std::string(w1 + w2 + 5, '-') << RESET << endl;
}

inline void printTableRow(const std::string& c1, const std::string& c2, int w1 = 10, int w2 = 22, const char* c2color = BWHITE) 
{
    cout << "  " << DIM << "|" << RESET << " " << CYAN << left << std::setw(w1) << c1 << RESET << " " << DIM << "|" << RESET  << " " << c2color << left << setw(w2) << c2 << RESET << " " << DIM << "|" << RESET << endl;
}

inline void printTableFooter(int w1 = 10, int w2 = 22) 
{
    cout << "  " << CYAN << std::string(w1 + w2 + 5, '-') << RESET << endl;
}

// Zone status cell: colour-codes 1/0 
inline string zoneCell(int val)
{
    if (val == 1)
    {
        return string(BGREEN) + " 1 " + RESET;
    }

    return string(BRED) + " 0 " + RESET;
}

// Progress bar
inline void printBar(const std::string& label, int percent, int width = 20) 
{
    int filled = (percent * width) / 100;
    string bar(filled, '#');
    string empty(width - filled, '.');
    const char* col = percent > 75 ? BRED : (percent > 40 ? BYELLOW : BGREEN);
    cout << "  " << CYAN << std::left << std::setw(18) << label << RESET << "[" << col << bar << DIM << empty << RESET << "] " << col << percent << "%" << RESET << "\n";
}

#endif // CONSOLE_UI_H

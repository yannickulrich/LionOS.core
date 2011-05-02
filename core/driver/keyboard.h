
#ifndef DRIVER_KEYBOARD
#define DRIVER_KEYBOARD

void addKeyListener(void (*h)(char c, int scan, bool* supress));
void flush();
void installKeyboard();
void keyPressed_handler(struct regs* r);
char getChar();

void setEditorMode(bool value);
void USscanChar(char* c);
int scanString(char* hw);
int scanInt();
void USscanInt(int* p);
void USscanBool(bool* p);

unsigned int scan;                //Scancode get from Keyboar

//#define NUMOFKEYLISTENER

#define MAXKEYBUFFER 64           // max keyboard buffer
#define FetchAndAnalyzeScancode GetScancode
//#include "datatypes.h"

bool KeyPressed;          //Keydown?
bool ShiftKeyDown;                 //Is Shift down?
bool CapsKeyDown;                  //Is Shift down?
bool AltGRDown;                    //Is AltGr down?
unsigned int KBLayout;            //Layout of Keyboard
void *keylisteners;
unsigned char asciiNonShift[128]; //Content of Keyboard Layout, shift is up
unsigned char asciiShift[128];    //Content of Keyboard Layout, shift is down
unsigned char asciiAltGR[128];    //Content of Keyboard Layout, Alt Gr is down
char actualChar;
int pressedKeys[128];
bool readFromKeyBoard = false;
bool scanUse = false;
int backSpaceEnd = 0;

// Non-ASCII special scancodes
#define    KESC         1
#define    KF1          0x80
#define    KF2         (KF1 + 1)
#define    KF3         (KF2 + 1)
#define    KF4         (KF3 + 1)
#define    KF5         (KF4 + 1)
#define    KF6         (KF5 + 1)
#define    KF7         (KF6 + 1)
#define    KF8         (KF7 + 1)
#define    KF9         (KF8 + 1)
#define    KF10        (KF9 + 1)
#define    KF11        (KF10 + 1)
#define    KF12        (KF11 + 1)
// Cursor Keys
#define    KINS         0x90
#define    KDEL        (KINS + 1)
#define    KHOME       (KDEL + 1)
#define    KEND        (KHOME + 1)
#define    KPGUP       (KEND + 1)
#define    KPGDN       (KPGUP + 1)
#define    KLEFT       75
#define    KRIGHT      77
#define    KDOWN       80
#define    KUP         72
// "Meta" keys
#define    KMETA_ALT     0x0200                                // Alt is pressed
#define    KMETA_CTRL    0x0400                                // Ctrl is pressed
#define    KMETA_SHIFT   0x0800                                // Shift is pressed
#define    KMETA_ANY    (KMETA_ALT | KMETA_CTRL | KMETA_SHIFT)
#define    KMETA_CAPS    0x1000                                // CapsLock is on
#define    KMETA_NUM     0x2000                                // NumLock is on
#define    KMETA_SCRL    0x4000                                // ScrollLock is on
// Other keys
#define    KPRNT    ( KRT + 1 )
#define    KPAUSE   ( KPRNT + 1 )
#define    KLWIN    ( KPAUSE + 1 )
#define    KRWIN    ( KLWIN + 1 )
#define    KMENU    ( KRWIN + 1 )
//Controlling Keys
#define    KRLEFT_CTRL        0x1D
#define    KRRIGHT_CTRL       0x1D   
#define    KRLEFT_ALT         0x38
#define    KRRIGHT_ALT        0x38
//Switching Keys
#define    KRLEFT_SHIFT       0x2A
#define    KRRIGHT_SHIFT      0x36
#define    KRCAPS_LOCK        0x3A
#define    KRSCROLL_LOCK      0x46
#define    KRNUM_LOCK         0x45
#define    KRDEL              0x53
//Master Keys
#define NULL 0
#define ESC    27
#define BACKSPACE '\b'
#define TAB       '\t'
#define ENTER     '\n'
#define RETURN    '\r'
#define NEWLINE ENTER
#include "../syscall.h"
#include "keyboard.c"
#endif

#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

class Keyboard
{
    public:
        enum SpecKeys {KEY_BACKSPACE = 8, KEY_TAB = 9, KEY_ENTER = 10, KEY_ESCAPE = 27,
                        KEY_F1 = 11, KEY_F2 = 12, KEY_F3 = 13, KEY_F4 = 14, KEY_F5 = 15, KEY_F6 = 16,
                        KEY_F7 = 17, KEY_F8 = 18, KEY_F9 = 19, KEY_F10 = 20, KEY_F11 = 21, KEY_F12 = 22,
                        KEY_LEFT = 0, KEY_UP = 1, KEY_RIGHT = 2, KEY_DOWN = 3,
                        KEY_PGUP = 4, KEY_PGDN = 5, KEY_HOME = 6, KEY_END = 7,
                        KEY_INSERT = 28};

        static void init();
        static bool getState(int key);
        static void registerKeyDownCallback(void (*func)(unsigned char key, int x, int y)) {keyDownCallback = func;}
        static void registerKeyUpCallback(void (*func)(unsigned char key, int x, int y)) {keyUpCallback = func;}


    private:
        Keyboard();
        static bool state[128];
        static void (*keyDownCallback)(unsigned char key, int x, int y);
        static void (*keyUpCallback)(unsigned char key, int x, int y);

        static void keyDown(unsigned char key, int x, int y);
        static void keyUp(unsigned char key, int x, int y);
        static void specialKeyDown(int key, int x, int y);
        static void specialKeyUp(int key, int x, int y);
};

#endif // KEYBOARD_H_INCLUDED
#ifndef MOUSE_H_INCLUDED
#define MOUSE_H_INCLUDED

class Mouse
{
    public:
        static void init();
        static bool getState(int button);
        static void registerCallback(void (*func)()) {callbackFunc = func;}
        static int getX() {return posX;}
        static int getY() {return posY;}
        static int getLastX() {return lastX;}
        static int getLastY() {return lastY;}

    private:
        Mouse();
        static int posX;
        static int posY;
        static int lastX;
        static int lastY;
        static bool state[8];
        static void (*callbackFunc)();

        static void mouseFunc(int button, int mouseState, int x, int y);
        static void mouseMotionFunc(int x, int y);
};

#endif // MOUSE_H_INCLUDED
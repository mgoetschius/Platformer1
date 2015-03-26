#ifndef INPUT_H
#define INPUT_H


class Input
{
    public:
        Input();
        virtual ~Input();
        //const int NUM_KEYS = 256;
        enum keyValues
        {
           KEY_A = 65,
           KEY_B,
           KEY_C,
           KEY_D,
           KEY_E,
           KEY_F,
           KEY_G,
           KEY_H,
           KEY_I,
           KEY_J,
           KEY_K,
           KEY_L,
           KEY_M,
           KEY_N,
           KEY_O,
           KEY_P,
           KEY_Q,
           KEY_R,
           KEY_S,
           KEY_T,
           KEY_U,
           KEY_V,
           KEY_W,
           KEY_X,
           KEY_Y,
           KEY_Z
        };
        static void setKeyTrue(int key);
        static void setKeyFalse(int key);
        static bool getKey(int key);
        static bool getKeyOnce(int key);
    protected:
    private:
        static bool keys[];
        static bool keyonce[];

};

#endif // INPUT_H

#ifndef ANIMATION_H
#define ANIMATION_H


class Animation
{
    public:
        Animation();
        void Setup(int first=0, int last=0, int cur=0, double t = 0, double d = .8);
        void Update(double dt);
        virtual ~Animation();
        int GetCurFrame() {return curFrame;};
    protected:
    private:
        int firstFrame;
        int lastFrame;
        int curFrame;
        double time;
        double delay;
};

#endif // ANIMATION_H

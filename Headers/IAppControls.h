#ifndef IAPPCONTROLS_H
#define IAPPCONTROLS_H

class IAppControls
{
    public:
        virtual ~IAppControls() = default;

        virtual void play() = 0;
        virtual void pause() = 0;
        virtual void clearBoard() = 0;
        virtual void saveState(const char* filename) = 0;
        virtual void loadState(const char* filename) = 0;
        virtual void changeImplementation(int implIndex) = 0;
};

#endif //IAPPCONTROLS_H
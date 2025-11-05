#ifndef ICONWAYALGORITHM_H
#define ICONWAYALGORITHM_H
#include <string>

class IConwayAlgorithm
{
    public:
        virtual ~IConwayAlgorithm() = default;

        virtual void step() = 0;

        virtual bool getCellState(int x, int y) const = 0;
        virtual void setCellState(int x, int y, bool state) = 0;
        virtual void clearGrid() = 0;
        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;

        virtual bool saveToFile(const std::string& filename) const = 0;
        virtual bool loadFromFile(const std::string& filename) = 0;

        virtual const char* getImplementationName() const = 0;
};

#endif //ICONWAYALGORITHM_H

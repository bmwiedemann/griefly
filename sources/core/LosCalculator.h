#pragma once

#include "Interfaces.h"

#include <QVector>

class LosCalculator
{
public:
    LosCalculator();
    void Calculate(const MapInterface* map, VisiblePoints* retval, int posx, int posy, int posz = 0) const;
private:
    static int PosToCorner(int pos);
    static int CornerToPos(int corner);
    static int Sign(int value);
    static PosPoint CornerPointToPoint(PosPoint p);

    bool CheckCorner(PosPoint p) const;
    bool CheckBorders(int x, int y, int z) const;
    bool IsTransparent(PosPoint p) const;
    bool BresenX(PosPoint source, PosPoint target) const;
    bool BresenY(PosPoint source, PosPoint target) const;
    bool RayTrace(PosPoint source, PosPoint target) const;
    void MarkTilesOfCornerAsVisible(
        VisiblePoints* retlist,
        PosPoint at,
        PosPoint center,
        std::vector<char>* visibility) const;

    // TODO: Make this class as pure function, because the only member class is not needed
    mutable const MapInterface* map_;
};
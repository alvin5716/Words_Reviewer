#include "wordshowingmode.h"

WordShowingMode::Mode WordShowingMode::nextMode(WordShowingMode::Mode mode) {
    switch(mode) {
    case AllShowing:
        return WordShowing;
    case WordShowing:
        return MeaningNotShowing;
    case MeaningNotShowing:
        return MeaningShowing;
    case MeaningShowing:
        return WordNotShowing;
    case WordNotShowing:
        return PartNotShowing;
    case PartNotShowing:
        return AllShowing;
    default:
        qDebug() << "No such mode, nextMode() failed.";
        return AllShowing;
    }
}

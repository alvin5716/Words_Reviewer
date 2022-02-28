#ifndef WORDSHOWINGMODE_H
#define WORDSHOWINGMODE_H

#include <QDebug>

namespace WordShowingMode {
    enum Mode {
        AllShowing,
        WordShowing,
        MeaningNotShowing,
        MeaningShowing,
        WordNotShowing,
        PartNotShowing
    };
    Mode nextMode(Mode mode);
}


#endif // WORDSHOWINGMODE_H

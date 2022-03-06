#include "windowsettings.h"

void showOnTop(QWidget* window) {
    window->setWindowFlags(window->windowFlags() | Qt::WindowStaysOnTopHint);
    window->show();
}

void showOffTop(QWidget* window) {
    window->setWindowFlags(window->windowFlags() & ~Qt::WindowStaysOnTopHint);
    window->show();
}

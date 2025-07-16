#include "MainFrame.h"

void MainFrame::API_SRV_PostLayout()
{
    CallAfter([this]() {
        Enable(true);
        Raise();
        });
}

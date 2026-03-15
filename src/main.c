#include "pd_api.h"

PlaydateAPI* pd = NULL;

static int update_callback(void* userdata) {
    pd->graphics->clear(kColorBlack);

    PDButtons pushed;
    pd->system->getButtonState(NULL, &pushed, NULL);

// #if TARGET_PLAYDATE
    pd->system->drawFPS(0, 0);
// #endif // TARGET_PLAYDATE
    pd->graphics->markUpdatedRows(0, 239);

    return 1;
}

#ifdef _WINDLL
__declspec(dllexport)
#endif // _WINDLL
int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg) {
    (void)arg;

    if (event == kEventInit) {
        pd = playdate;

        pd->display->setRefreshRate(0);
        pd->system->setUpdateCallback(update_callback, NULL);
    }

    if (event == kEventTerminate) {
    }

    return 0;
}

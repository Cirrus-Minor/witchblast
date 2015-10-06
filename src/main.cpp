#include <SFML/Graphics.hpp>
#include "WitchBlastGame.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

int main()
{
#ifdef __APPLE__
    // -------------------------------------------------------------------
    // http://stackoverflow.com/a/520951/2038264
    // This makes relative paths work in C++ in Xcode by changing
    // directory to the Resources folder inside the .app bundle
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    chdir(path);
    // -------------------------------------------------------------------
#endif

    WitchBlastGame game;
    game.startGame();

    return 0;
}

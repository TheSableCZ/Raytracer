
#include "GLviewer/GLViewer.h"

void args(int argc, char **argv) {
    if (argc > 1) {
        AppSettings::defaultScene = std::stoi(argv[1]);
    }
    if (argc > 2) {
        AppSettings::defaultAccelerationDS = std::stoi(argv[2]);
    }
    if (argc > 3) {
        AppSettings::samplesLimit = std::stoi(argv[3]);
    }
    if (argc > 4) {
        AppSettings::treeLevelMax = std::stoi(argv[4]);
    }
    if (argc > 5) {
        AppSettings::treeLeafLimit = std::stoi(argv[5]);
    }
}

int main(int argc, char **argv) {
    args(argc, argv);

    auto glViewer = GLViewer();
    glViewer.run();

    return 0;
}

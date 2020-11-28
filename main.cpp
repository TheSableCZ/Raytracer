
#include "GLviewer/GLViewer.h"

int main() {
    auto glViewer = GLViewer();
    glViewer.run();

    /*Raytracer raytracer;
    ColorBuffer colBuff(AppSettings::imgWidth, AppSettings::imgHeight);

    createScene(raytracer.scene());

    raytracer.render(colBuff, AppSettings::imgWidth, AppSettings::imgHeight);

    colBuff.saveToPPM();*/

    return 0;
}

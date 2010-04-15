///////////////////////////////////////////////////////////////////////////////
// ModelGL.h
// =========
// Model component of OpenGL
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2008-10-02
// UPDATED: 2008-10-07
///////////////////////////////////////////////////////////////////////////////

#ifndef MODEL_GL_H
#define MODEL_GL_H

#include <GL/glu.h>
#include "Vectors.h"

class ModelGL
{
public:
    ModelGL();
    ~ModelGL();

    void init();                                    // initialize OpenGL states
    void quit();                                    // clean up OpenGL objects
    void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
    void draw();

    void setMouseLeft(bool flag)        { mouseLeftDown = flag; };
    void setMouseRight(bool flag)       { mouseRightDown = flag; };
    void setMousePosition(int x, int y) { mouseX = x; mouseY = y; };
    void setDrawMode(int mode);
    void setWindowSize(int width, int height);
    void setProjection(float l, float r, float b, float t, float n, float f);
    void setProjectionMode(int mode)    { projectionMode = mode; };
    int  getProjectionMode()            { return projectionMode; };

    void setProjectionLeft(float l)     { projectionLeft = l; }
    void setProjectionRight(float r)    { projectionRight = r; }
    void setProjectionBottom(float b)   { projectionBottom = b; }
    void setProjectionTop(float t)      { projectionTop = t; }
    void setProjectionNear(float n)     { projectionNear = n; }
    void setProjectionFar(float f)      { projectionFar = f; }
    float getProjectionLeft()           { return projectionLeft; }
    float getProjectionRight()          { return projectionRight; }
    float getProjectionBottom()         { return projectionBottom; }
    float getProjectionTop()            { return projectionTop; }
    float getProjectionNear()           { return projectionNear; }
    float getProjectionFar()            { return projectionFar; }

    void getProjectionMatrix(float* m);       // should have 16 elements

    void rotateCamera(int x, int y);
    void zoomCamera(int dist);


protected:

private:
    // member functions
    void initLights();                              // add a white light ti scene
    void setViewport(int x, int y, int width, int height);
    void setViewportSub(int left, int bottom, int width, int height, float nearPlane, float farPlane);
    void setViewportSub(int left, int bottom, int width, int height, float l, float r, float b, float t, float n, float f);
    void drawGrid(float size, float step);          // draw a grid on XZ plane
    void drawAxis(float size);
    void drawSub1();                                // draw upper window
    void drawSub2();                                // draw bottom window
    void drawFrustum(float fovy, float aspect, float near, float far);
    void drawFrustum(float l, float r, float b, float t, float n, float f);
    void drawSpheres();
    void computeFrustumVertices(float l, float r, float b, float t, float n, float f);

    // members
    int windowWidth;
    int windowHeight;
    bool windowSizeChanged;
    bool drawModeChanged;
    int drawMode;
    int projectionMode;
    bool mouseLeftDown;
    bool mouseRightDown;
    int mouseX;
    int mouseY;
    float projectionLeft;
    float projectionRight;
    float projectionBottom;
    float projectionTop;
    float projectionNear;
    float projectionFar;
    Vector3 frustumVertices[8];         // 8 vertices of frustum
    Vector3 frustumNormals[6];          // 6 face normals of frustum
    GLUquadricObj* quadricId;

    // these are for 3rd person view
    float cameraAngleX;
    float cameraAngleY;
    float cameraDistance;
    float bgColor[4];
    float matrixProjection[16];

};
#endif

#ifndef CAMERA_H
#define CAMERA_H

void setCamera3D(float fov,u32 bgCol, float fogDist, float fogDistFar);
void updateCamera3D(float xfrom, float yfrom, float zfrom, float xto, float yto, float zto);
void updateCamera25D(float x, float y, float z);
void setCamera2D();
#endif

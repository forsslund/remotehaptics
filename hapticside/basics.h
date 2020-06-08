#ifndef BASICS_H
#define BASICS_H

#include <chai3d.h>
using namespace chai3d;

class CommonMaterials {
public:
    // Create useful colors
    cMaterial ordinalColors[6];
    cMaterial color_a,color_b,color_c,color_d,color_e,color_f;
    cMaterial color_red;
    cMaterial color_green;
    cMaterial color_blue;

    CommonMaterials(){
        color_red.m_ambient.set(255.0,0.0,0.0,1.0);
        color_green.m_ambient.set(0.0,255.0,0.0,1.0);
        color_blue.m_ambient.set(0.0,0.0,255.0,1.0);

        color_a.m_ambient.set(178.0/255.0,24.0/255.0,43.0/255.0,1.0);
        color_b.m_ambient.set(239.0/255.0,138.0/255.0,98.0/255.0,1.0);
        color_c.m_ambient.set(253.0/255.0,219.0/255.0,199.0/255.0,1.0);
        color_d.m_ambient.set(209.0/255.0,229.0/255.0,240.0/255.0,1.0);
        color_e.m_ambient.set(103.0/255.0,169.0/255.0,207.0/255.0,1.0);
        color_f.m_ambient.set(33.0/255.0,102.0/255.0,172.0/255.0,1.0);
        ordinalColors[0] = color_a;
        ordinalColors[1] = color_b;
        ordinalColors[2] = color_c;
        ordinalColors[3] = color_d;
        ordinalColors[4] = color_e;
        ordinalColors[5] = color_f;
    }

};

class Orientation {
public:
    Orientation(){
        myMatrix.identity();
    }

    cVector3d getEuler(){
        // TODO: Implement me
        return cVector3d(0,0,0);
    }

    cMatrix3d getRotMatrix() const {
        return myMatrix;
    }

    Orientation(cMatrix3d rotMatrix) {
        myMatrix.copyfrom(rotMatrix);
    }

protected:

    cMatrix3d myMatrix;


};

class SixDofForce {

public:
    SixDofForce(cVector3d force, cVector3d torque):
        force(force), torque(torque){
    }

    SixDofForce(){
        zero();
    }

    void zero(){
        force.zero();
        torque.zero();
    }

    SixDofForce operator +(SixDofForce f){
        return SixDofForce(force+f.force, torque+f.torque);
    }

    cVector3d force;
    cVector3d torque;

protected:

};

struct SixDofPos {
    Orientation o;
    cVector3d p;
};



#endif // BASICS_H

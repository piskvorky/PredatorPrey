////////////////////////////////////////////////////////////////////////////////
// file: graze.cpp
// author: Radim Rehurek
// last update: 25.09.04
//
// grazing (flocking + food search)
//////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "graze.h"
#include "prey.h"
#include "virtualtime.h"
#include "demo.h"

Graze::Graze() {
    m_name      = "Graze"; // must match class name
    m_owner     = NULL;
}

void Graze::init ( IEOwner * owner ) {
    m_owner = dynamic_cast<CPrey *> (owner);
}

bool Graze::start () {
    m_state = Go;
    m_owner->setMaxSpeed(m_owner->getWalkSpeed());
    return false;
}

void outvec(const std::string &name, const Vector2d &vec) {
    std::cout << name << ": " << vec.m_x << " " << vec.m_y << std::endl;
}

bool Graze::update () {
    switch ( m_state ) {
    case Go:
        Vector2d separation = m_owner->getSeparationSteer(0.1f, 0.6f * M_PIf);
        //separation.normalize();
        Vector2d cohesion = m_owner->getCohesionSteer(0.1f, 0.6f * M_PIf);
        cohesion.normalize();
        Vector2d alignment = m_owner->getAlignmentSteer(0.1f, 0.5f * M_PIf);
        alignment.normalize();
        Vector2d food = m_owner->getFoodSteer(0.2f);
        food.normalize();

        Vector2d borders = m_owner->getBordersSteer(2);
//        borders.normalize();

        separation.scale(0.02f);
        cohesion.scale(2.0f);
        alignment.scale(1.3f);
        food.scale(1.3f);
        borders.scale(0.5f);

        Vector2d dir = separation + alignment + cohesion + food + borders;
        m_owner->setSteer(dir);

        break;
    }
    return true;
}

bool Graze::finish () {
    return true;
}

IEOwner * Graze::getOwner () {
    return m_owner;
}

const char * Graze::getName() {
    return m_name.c_str();
}

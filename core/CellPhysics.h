
#ifndef _LIBRARY_CELL_PHYSICS_H_
#define _LIBRARY_CELL_PHYSICS_H_

/* ************************************************************************ */

// C++
#include <memory>

// Bullet
#include <btBulletDynamicsCommon.h>

// Core
#include "Units.h"

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Cell physics.
 */
class CellPhysics
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param world
     * @param x
     * @param y
     */
    CellPhysics(World* world, MicroMeters x, MicroMeters y);


    /**
     * @brief Destructor.
     */
    virtual ~CellPhysics();


public:



public:



// Protected Data Members
protected:

    /// Object shape.
    std::unique_ptr<btCollisionShape> m_shape;

    /// Cell body.
    std::unique_ptr<btRigidBody> m_rigidBody;

    /// Cell motion state.
    std::unique_ptr<btDefaultMotionState> m_motionState;

};

/* ************************************************************************ */

#endif // _LIBRARY_CELL_PHYSICS_H_

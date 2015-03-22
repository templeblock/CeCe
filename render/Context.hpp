
#pragma once

#ifdef ENABLE_RENDER

/* ************************************************************************ */

// C++
#include <vector>
#include <cstdint>

// Render
#include "render/Camera.hpp"
#include "render/Position.hpp"
#include "render/Vector3.hpp"
#include "render/Color.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

/**
 * @brief Renderer context.
 */
class Context
{

// Public Accessors
public:


    /**
     * @brief Returns context camera.
     *
     * @return
     */
    Camera& getCamera() noexcept
    {
        return m_camera;
    }


    /**
     * @brief Get current image data.
     *
     * @return
     */
    std::vector<std::uint8_t> getData() const noexcept;


// Public Operations
public:


    /**
     * @brief Initialize context.
     */
    void init() noexcept;


    /**
     * @brief Setup camera view.
     *
     * @param width  View width.
     * @param height View height.
     */
    void setView(int width, int height) noexcept;


    /**
     * @brief Begin frame rendering.
     *
     * @param width  View width.
     * @param height View height.
     */
    void frameBegin(int width, int height) noexcept;


    /**
     * @brief End frame rendering.
     */
    void frameEnd() noexcept;


    /**
     * @brief Draw axis.
     *
     * @param pos   Axis position.
     * @param scale Axis scale.
     */
    void drawAxis(const Position& pos, float scale) noexcept;


    /**
     * @brief Draw plane.
     *
     * @param pos    Plane position.
     * @param width  Plane width.
     * @param height Plane height.
     * @param color  Plane color.
     */
    void drawPlane(const Position& pos, float width, float height, const Color& color) noexcept;


    /**
     * @brief Draw sphere shape.
     *
     * @param pos    Sphere position.
     * @param radius Sphere radius.
     * @param color  Sphere color.
     */
    void drawSphere(const Position& pos, float radius, const Color& color) noexcept;


    /**
     * @brief Draw barrier.
     *
     * @param barrier
     */
    //void drawBarrier(const simulator::Barrier& barrier) noexcept;


    /**
     * @brief Draw 3D grid.
     *
     * @param pos    Origin position.
     * @param size   Grid size.
     * @param count  Number of parts.
     * @param color  Grid color.
     */
    void drawGrid3d(const Position& pos, const Position& size,
                    const Vector3<unsigned>& count, const Color& color) noexcept;


// Private Data Members
private:

    /// Camera.
    Camera m_camera;

};

/* ************************************************************************ */

}

/* ************************************************************************ */

#endif

/* ************************************************************************ */

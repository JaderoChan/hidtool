#ifndef HIDTOOL_MOUSE_SIMULATOR_HPP
#define HIDTOOL_MOUSE_SIMULATOR_HPP

#include <hidtool/defines.hpp>
#include "mouse_event.hpp"

namespace hidt
{

class MouseSimulatorPrivate;

/**
 * @brief Mouse Input Simulator
 * @ingroup hid_simulators
 * @note Unless otherwise specified, all member functions of this class are reentrant.
 * In addition, except for \ref MouseSimulator::initialize() and \ref MouseSimulator::destroy(),
 * other member functions are reentrant.
 */
class HIDTOOL_API MouseSimulator
{
public:
    static MouseSimulator& getInstance();

    /**
     * @brief Get the coordinate range of absolute movement events sent in the current environment.
     * @note When the coordinates of the sent absolute movement event exceed this range, they will be clamped.
     * @note This API is affected by the environment's DPI policy.
     * On the **Windows** platform, the `HIDTOOL_FORCE_IN_PIXEL` compile option can be enabled to
     * force the use of physical pixels as units.
     * @details On the **Windows** and **MacOS** platforms, it corresponds to the range of the virtual screen space.
     * On the **Linux** platform, it always corresponds to {0, 65535, 0, 65535}.
     */
    static AbsolutePosRange getAbsoluteMoveRange();

    bool initialize();
    void destroy();
    bool isInitialized() const;

    bool sendEvent(const MouseEvent& event);
    size_t sendEvent(const MouseEvent* events, size_t count);

    /**
     * @defgroup mouse_simulator_convenient Mouse simulation utility functions
     * @brief Equivalent to send event or send a group events by \ref sendEvent().
     */

    /**
     * @ingroup mouse_simulator_convenient
     * @{
     */

    /**
     * @brief Mouse absolute movement
     * @note When an absolute move event exceeding the coordinate range is sent,
     * it will be clamped within the valid range.
     * @sa \ref MouseEvent::absPos
     */
    bool moveTo(const AbsolutePos& absPos);

    /**
     * @brief Mouse relative movement
     * @sa \ref MouseEvent::relPos
     */
    bool moveBy(const RelativePos& relPos);

    /**
     * @brief Mouse wheel scrolling
     * @param wheelDelta The unit amount is `120`.
     * When the value is positive, the scroll wheel moves away from the user;
     * when the value is negative, the scroll wheel moves toward the user.
     * @sa \ref MouseEvent::wheelDelta
     */
    bool wheel(int32_t wheelDelta);

    /**
     * @name Mouse button function
     * @brief Execute the button function at the current mouse pointer position.
     * @{
     */

    bool pressButton(MouseButton button);
    bool releaseButton(MouseButton button);
    bool clickButton(MouseButton button, size_t interval = 0);

    /**
     * @brief Double-click the mouse
     * @param interval1 The time interval between key press and release in a click event.
     * @param interval2 The time interval between two click events.
     */
    bool doubleClickButton(MouseButton button, size_t interval1 = 0, size_t interval2 = 10);

    /** @} */

    /**
     * @name Mouse function with coordinates
     * @brief Execute the function at the specified location.
     * @note Equivalent to a combination of absolute movement events and other events.
     * @{
     */

    bool wheel(const AbsolutePos& absPos, int32_t wheelDelta, size_t interval = 10);
    bool pressButton(const AbsolutePos& absPos, MouseButton button, size_t interval = 10);
    bool releaseButton(const AbsolutePos& absPos, MouseButton button, size_t interval = 10);

    /**
     * @param interval1 The time interval between the move event and the click event.
     * @param interval2 The time interval between key press and release in a click event.
     */
    bool clickButton(const AbsolutePos& absPos, MouseButton button,
        size_t interval1 = 10, size_t interval2 = 0);

    /**
     * @param interval1 The time interval between the move event and the click event.
     * @param interval2 The time interval between key press and release in a click event.
     * @param interval3 The time interval between two click events.
     */
    bool doubleClickButton(const AbsolutePos& absPos, MouseButton button,
        size_t interval1 = 10, size_t interval2 = 0, size_t interval3 = 10);

    /** @} */

    /**
     * @brief Mouse drag move
     * @note On **Windows** and **Linux** platforms, it is equivalent to an absolute move event,
     * and the `button` parameter will be discarded. On the **MacOS** platform, it is natively supported.
     * @sa \ref drag_combo_anchor
     */
    bool dragTo(const AbsolutePos& absPos, MouseButton button = MSBTN_LEFT);

    /**
     * @anchor drag_combo_anchor
     * @name Mouse drag operation combination
     * @brief Drag the mouse from the specified starting position to the specified ending position.
     * @details Equivalent to a combination of mouse button press events, drag events, and mouse button release events.
     * @{
     */

    /**
     * @param interval The interval between the key press, drag, and key release events during
     * a drag-and-drop operation.
     */
    bool dragCombo(const AbsolutePos& endPos,
        MouseButton button = MSBTN_LEFT, size_t interval = 0);

    /**
     * @param interval1 The time interval between the event of move to `startPos` and the drag event.
     * @param interval2 The interval between the key press, drag, and key release events during
     * a drag-and-drop operation.
     */
    bool dragCombo(const AbsolutePos& startPos, const AbsolutePos& endPos,
        MouseButton button = MSBTN_LEFT, size_t interval1 = 10, size_t interval2 = 0);

    /** @} */

    /** @} */

private:
    explicit MouseSimulator(MouseSimulatorPrivate&);
    ~MouseSimulator() = default;
    MouseSimulator(const MouseSimulator&) = delete;
    MouseSimulator& operator=(const MouseSimulator&) = delete;

    MouseSimulatorPrivate& pri_;
};

} // namespace hidt

#endif // !HIDTOOL_MOUSE_SIMULATOR_HPP

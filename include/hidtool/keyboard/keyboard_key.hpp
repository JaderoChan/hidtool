#ifndef HIDTOOL_KEYBOARD_KEY_HPP
#define HIDTOOL_KEYBOARD_KEY_HPP

#include <hidtool/defines.hpp>

namespace hidt
{

/** @brief Cross-platform keyboard key value enumeration */
enum KeyboardKey : uint32_t
{
    /**
     * @name Invalid key
     * @{
     */
    KBDKEY_NONE     = 0x0000,
    /** @} */

    /**
     * @name Numeric keys
     * @{
     */
    KBDKEY_0        = 0x0030,
    KBDKEY_1        = 0x0031,
    KBDKEY_2        = 0x0032,
    KBDKEY_3        = 0x0033,
    KBDKEY_4        = 0x0034,
    KBDKEY_5        = 0x0035,
    KBDKEY_6        = 0x0036,
    KBDKEY_7        = 0x0037,
    KBDKEY_8        = 0x0038,
    KBDKEY_9        = 0x0039,
    /** @} */

    /**
     * @name Letter keys
     * @{
     */
    KBDKEY_A        = 0x0041,
    KBDKEY_B        = 0x0042,
    KBDKEY_C        = 0x0043,
    KBDKEY_D        = 0x0044,
    KBDKEY_E        = 0x0045,
    KBDKEY_F        = 0x0046,
    KBDKEY_G        = 0x0047,
    KBDKEY_H        = 0x0048,
    KBDKEY_I        = 0x0049,
    KBDKEY_J        = 0x004a,
    KBDKEY_K        = 0x004b,
    KBDKEY_L        = 0x004c,
    KBDKEY_M        = 0x004d,
    KBDKEY_N        = 0x004e,
    KBDKEY_O        = 0x004f,
    KBDKEY_P        = 0x0050,
    KBDKEY_Q        = 0x0051,
    KBDKEY_R        = 0x0052,
    KBDKEY_S        = 0x0053,
    KBDKEY_T        = 0x0054,
    KBDKEY_U        = 0x0055,
    KBDKEY_V        = 0x0056,
    KBDKEY_W        = 0x0057,
    KBDKEY_X        = 0x0058,
    KBDKEY_Y        = 0x0059,
    KBDKEY_Z        = 0x005a,
    /** @} */

    /**
     * @name Whitespace character keys
     * @{
     */
    KBDKEY_TAB      = 0x8000,
    KBDKEY_SPACE,
    KBDKEY_ENTER,
    /** @} */

    /**
     * @name Navigation keys
     * @{
     */
    KBDKEY_HOME,
    KBDKEY_END,
    KBDKEY_PAGE_UP,
    KBDKEY_PAGE_DOWN,
    KBDKEY_LEFT,
    KBDKEY_UP,
    KBDKEY_RIGHT,
    KBDKEY_DOWN,
    /** @} */

    /**
     * @name Editing keys
     * @{
     */
    KBDKEY_BACKSPACE,
    KBDKEY_INSERT,
    KBDKEY_DELETE,
    KBDKEY_CLEAR,
    /** @} */

    /**
     * @name Function keys
     * @{
     */
    KBDKEY_F1,
    KBDKEY_F2,
    KBDKEY_F3,
    KBDKEY_F4,
    KBDKEY_F5,
    KBDKEY_F6,
    KBDKEY_F7,
    KBDKEY_F8,
    KBDKEY_F9,
    KBDKEY_F10,
    KBDKEY_F11,
    KBDKEY_F12,
    KBDKEY_F13,
    KBDKEY_F14,
    KBDKEY_F15,
    KBDKEY_F16,
    KBDKEY_F17,
    KBDKEY_F18,
    KBDKEY_F19,
    KBDKEY_F20,
    KBDKEY_F21,
    KBDKEY_F22,
    KBDKEY_F23,
    KBDKEY_F24,
    /** @} */

    /**
     * @name Numeric keys on keypad
     * @{
     */
    KBDKEY_NUMPAD_0,
    KBDKEY_NUMPAD_1,
    KBDKEY_NUMPAD_2,
    KBDKEY_NUMPAD_3,
    KBDKEY_NUMPAD_4,
    KBDKEY_NUMPAD_5,
    KBDKEY_NUMPAD_6,
    KBDKEY_NUMPAD_7,
    KBDKEY_NUMPAD_8,
    KBDKEY_NUMPAD_9,
    /** @} */

    /**
     * @name Arithmetic operator keys and enter key on keypad
     * @{
     */
    KBDKEY_NUMPAD_ADD,          ///< '+' on keypad
    KBDKEY_NUMPAD_SUBTRACT,     ///< '-' on keypad
    KBDKEY_NUMPAD_MULTIPLY,     ///< '*' or '×' on keypad
    KBDKEY_NUMPAD_DIVIDE,       ///< '/' or '÷' on keypad
    KBDKEY_NUMPAD_DECIMAL,      ///< '.' on keypad
    KBDKEY_NUMPAD_SEPARATOR,    ///< ',' on keypad
    KBDKEY_NUMPAD_EQUAL,        ///< ' =' on keypad
    KBDKEY_NUMPAD_ENTER,        ///< Enter on keypad
    /** @} */

    /**
     * @name Application keys
     * @{
     */
    KBDKEY_ESCAPE,
    KBDKEY_PAUSE,               ///< Not is \ref KBDKEY_MEDIA_PLAY_PAUSE
    KBDKEY_PLAY,                ///< Not is \ref KBDKEY_MEDIA_PLAY_PAUSE
    KBDKEY_HELP,
    KBDKEY_MENU,
    /** @} */

    /**
     * @name Multimedia keys
     * @{
     */
    KBDKEY_MEDIA_NEXT,
    KBDKEY_MEDIA_PREVIOUS,
    KBDKEY_MEDIA_PLAY_PAUSE,
    KBDKEY_MEDIA_STOP,
    /** @} */

    /**
     * @name Volume keys
     * @{
     */
    KBDKEY_VOLUME_UP,
    KBDKEY_VOLUME_DOWN,
    KBDKEY_VOLUME_MUTE,
    /** @} */

    /**
     * @name Lock keys
     * @{
     */
    KBDKEY_CAPS_LOCK,
    KBDKEY_NUM_LOCK,
    KBDKEY_SCROLL_LOCK,
    /** @} */

    /**
     * @name Device keys
     * @{
     */
    KBDKEY_PRINT_SCREEN,
    KBDKEY_SLEEP,
    /** @} */

    /**
     * @name OEM (Original Equipment Manufacturer) keys
     * @note The naming of the following keys may vary by device; the US standard keyboard layout is used here.
     * @{
     */
    KBDKEY_LEFT_QUOTE,          ///< '`~' on US standard keyboard
    KBDKEY_MINUS,               ///< '-_' on US standard keyboard
    KBDKEY_EQUAL,               ///< '=+' on US standard keyboard
    KBDKEY_LEFT_BRACKET,        ///< '[{' on US standard keyboard
    KBDKEY_RIGHT_BRACKET,       ///< ']}' on US standard keyboard
    KBDKEY_SEMICOLON,           ///< ';:' on US standard keyboard
    KBDKEY_APOSTROPHE,          ///< ''"' on US standard keyboard
    KBDKEY_COMMA,               ///< ',<' on US standard keyboard
    KBDKEY_PERIOD,              ///< '.>' on US standard keyboard
    KBDKEY_SLASH,               ///< '/?' on US standard keyboard
    KBDKEY_BACKSLASH,           ///< '\|' on US standard keyboard
    KBDKEY_ANGLE_BRACKET,       ///< '<>' or '\|' on RT 102-key keyboard
    /** @} */

    /**
     * @name Modifier keys
     * @{
     */
    KBDKEY_FUNCTION,
    KBDKEY_META,
    KBDKEY_META_LEFT,
    KBDKEY_META_RIGHT,
    KBDKEY_CTRL,
    KBDKEY_CTRL_LEFT,
    KBDKEY_CTRL_RIGHT,
    KBDKEY_ALT,
    KBDKEY_ALT_LEFT,
    KBDKEY_ALT_RIGHT,
    KBDKEY_SHIFT,
    KBDKEY_SHIFT_LEFT,
    KBDKEY_SHIFT_RIGHT,
    /** @} */

    /**
     * @name Key alias
     * @{
     */
    KBDKEY_RETURN               = KBDKEY_ENTER,
    KBDKEY_ESC                  = KBDKEY_ESCAPE,
    KBDKEY_NUMPAD_PLUS          = KBDKEY_NUMPAD_ADD,
    KBDKEY_NUMPAD_MINUS         = KBDKEY_NUMPAD_SUBTRACT,
    KBDKEY_NUMPAD_RETURN        = KBDKEY_NUMPAD_ENTER,
    KBDKEY_STANDBY              = KBDKEY_SLEEP,
    KBDKEY_GRAVE                = KBDKEY_LEFT_QUOTE,

    KBDKEY_FN                   = KBDKEY_FUNCTION,
    KBDKEY_OPTION               = KBDKEY_ALT,
    KBDKEY_OPTION_LEFT          = KBDKEY_ALT_LEFT,
    KBDKEY_OPTION_RIGHT         = KBDKEY_ALT_RIGHT
    /** @} */
};

/**
 * @brief Returns the platform-specific native key value corresponding to the given \ref KeyboardKey.
 * @return If the given \ref KeyboardKey value does not have a corresponding native key value, return `-1`;
 * otherwise, return its corresponding native key value.
 * @note On the **Windows** platform, the native key value it returns is `VK_*`.
 * @note On the **MacOS** platform, the native key value it returns is `kVK_*`.
 * @note On the **Linux** platform, the native key value it returns is `KEY_*`.
 */
HIDTOOL_API int32_t keyboardKeyToNativeKey(KeyboardKey key) noexcept;

/**
 * @brief Return the corresponding \ref KeyboardKey for the given platform-specific native key value.
 * @return If the given native key value does not have a corresponding \ref KeyboardKey,
 * return \ref KeyboardKey::KBDKEY_NONE, otherwise, return its corresponding \ref KeyboardKey.
 */
HIDTOOL_API KeyboardKey keyboardKeyFromNativeKey(int32_t nativeKey) noexcept;

} // namespace hidt

#endif // !HIDTOOL_KEYBOARD_KEY_HPP

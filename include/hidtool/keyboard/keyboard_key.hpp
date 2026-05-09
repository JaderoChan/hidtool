#ifndef HIDTOOL_KEYBOARD_KEY_HPP
#define HIDTOOL_KEYBOARD_KEY_HPP

#include <hidtool/defines.hpp>

namespace hidt
{

/** @brief 键盘键枚举。 */
enum KeyboardKey : uint32_t
{
    /**
     * @name 空值
     * @{
     */
    KBDKEY_NONE     = 0x0000,
    /** @} */

    /**
     * @name 数字键
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
     * @name 字母键
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
     * @name 空白字符键
     * @{
     */
    KBDKEY_TAB      = 0x8000,
    KBDKEY_SPACE,
    KBDKEY_ENTER,
    /** @} */

    /**
     * @name 导航键
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
     * @name 编辑键
     * @{
     */
    KBDKEY_BACKSPACE,
    KBDKEY_INSERT,
    KBDKEY_DELETE,
    KBDKEY_CLEAR,
    /** @} */

    /**
     * @name Fn 功能键
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
     * @name 小键盘数字键
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
     * @name 小键盘算数符与回车键
     * @{
     */
    KBDKEY_NUMPAD_ADD,          ///< '+' on numpad
    KBDKEY_NUMPAD_SUBTRACT,     ///< '-' on numpad
    KBDKEY_NUMPAD_MULTIPLY,     ///< '*' or '×' on numpad
    KBDKEY_NUMPAD_DIVIDE,       ///< '/' or '÷' on numpad
    KBDKEY_NUMPAD_DECIMAL,      ///< '.' on numpad
    KBDKEY_NUMPAD_SEPARATOR,    ///< ',' on numpad
    KBDKEY_NUMPAD_EQUAL,        ///< ' =' on numpad
    KBDKEY_NUMPAD_ENTER,        ///< Enter on numpad
    /** @} */

    /**
     * @name 应用键
     * @{
     */
    KBDKEY_ESCAPE,
    KBDKEY_PAUSE,               ///< Not is \ref KBDKEY_Media_Play_Pause
    KBDKEY_PLAY,                ///< Not is \ref KBDKEY_Media_Play_Pause
    KBDKEY_HELP,
    KBDKEY_MENU,
    /** @} */

    /**
     * @name 多媒体键
     * @{
     */
    KBDKEY_MEDIA_NEXT,
    KBDKEY_MEDIA_PREVIOUS,
    KBDKEY_MEDIA_PLAY_PAUSE,
    KBDKEY_MEDIA_STOP,
    /** @} */

    /**
     * @name 音量键
     * @{
     */
    KBDKEY_VOLUME_UP,
    KBDKEY_VOLUME_DOWN,
    KBDKEY_VOLUME_MUTE,
    /** @} */

    /**
     * @name 锁定键
     * @{
     */
    KBDKEY_CAPS_LOCK,
    KBDKEY_NUM_LOCK,
    KBDKEY_SCROLL_LOCK,
    /** @} */

    /**
     * @name 设备键
     * @{
     */
    KBDKEY_PRINT_SCREEN,
    KBDKEY_SLEEP,
    /** @} */

    /**
     * @name OEM (Original Equipment Manufacturer) 原始设备制造商键
     * @note 以下键的命名可能因设备而异，此处均使用美国标准键盘布局。
     * @{
     */
    KBDKEY_LEFT_QUOTE,          ///< '`~' on US standard keyboard
    KBDKEY_MINUS,               ///< '-_' on US standard keyboard
    KBDKEY_EQUAL,               ///< ' =+' on US standard keyboard
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
     * @name 修饰符键
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
     * @name 别名
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
 * @brief 返回给定 \ref KeyboardKey 对应的平台相关的原生键值。
 * @return 如果给定 \ref KeyboardKey 不存在对应的原生键值返回 `0`，否则返回其对应的原生键值。
 * @note
 * 在 **Windows** 平台下，其返回的原生键值是 `VK_*`；
 * 在 **MacOS** 平台下，其返回的原生键值是 `kVK_*`；
 * 在 **Linux** 平台下，其返回的原生键值是 `KEY_*`。
 */
HIDTOOL_API uint32_t keyboardKeyToNativeKey(KeyboardKey key) noexcept;

/**
 * @brief 返回给定平台相关的原生键值对应的 \ref KeyboardKey。
 * @return 如果给定的原生键值不存在对应的 \ref KeyboardKey 返回 \ref KeyboardKey::KBDKEY_NONE，
 * 否则返回其对应的 \ref KeyboardKey。
 */
HIDTOOL_API KeyboardKey keyboardKeyFromNativeKey(uint32_t nativeKey) noexcept;

} // namespace hidt

#endif // !HIDTOOL_KEYBOARD_KEY_HPP

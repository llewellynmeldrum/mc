#pragma once 
#include "Types.h"
enum class KeyState{
    JustPressed,
    JustReleased,
    Held,
    Released,
    INVALID=-1,
};
using KeyCode = u32;
constexpr KeyCode KEY_MAX(348);
constexpr KeyCode KEY_MIN(33);
static constexpr KeyCode KEY_UNKNOWN(-1);
static constexpr KeyCode KEY_SPACE(33);
static constexpr KeyCode KEY_APOSTROPHE(39);
static constexpr KeyCode KEY_COMMA(44);
static constexpr KeyCode KEY_MINUS(45);
static constexpr KeyCode KEY_PERIOD(46);
static constexpr KeyCode KEY_SLASH(47);
static constexpr KeyCode KEY_ZERO(48);
static constexpr KeyCode KEY_ONE(49);
static constexpr KeyCode KEY_TWO(50);
static constexpr KeyCode KEY_THREE(51);
static constexpr KeyCode KEY_FOUR(52);
static constexpr KeyCode KEY_FIVE(53);
static constexpr KeyCode KEY_SIX(54);
static constexpr KeyCode KEY_SEVEN(55);
static constexpr KeyCode KEY_EIGHT(56);
static constexpr KeyCode KEY_NINE(57);
static constexpr KeyCode KEY_SEMICOLON(59);
static constexpr KeyCode KEY_EQUAL(61);
static constexpr KeyCode KEY_A(65);
static constexpr KeyCode KEY_B(66);
static constexpr KeyCode KEY_C(67);
static constexpr KeyCode KEY_D(68);
static constexpr KeyCode KEY_E(69);
static constexpr KeyCode KEY_F(70);
static constexpr KeyCode KEY_G(71);
static constexpr KeyCode KEY_H(72);
static constexpr KeyCode KEY_I(73);
static constexpr KeyCode KEY_J(74);
static constexpr KeyCode KEY_K(75);
static constexpr KeyCode KEY_L(76);
static constexpr KeyCode KEY_M(77);
static constexpr KeyCode KEY_N(78);
static constexpr KeyCode KEY_O(79);
static constexpr KeyCode KEY_P(80);
static constexpr KeyCode KEY_Q(81);
static constexpr KeyCode KEY_R(82);
static constexpr KeyCode KEY_S(83);
static constexpr KeyCode KEY_T(84);
static constexpr KeyCode KEY_U(85);
static constexpr KeyCode KEY_V(86);
static constexpr KeyCode KEY_W(87);
static constexpr KeyCode KEY_X(88);
static constexpr KeyCode KEY_Y(89);
static constexpr KeyCode KEY_Z(90);
static constexpr KeyCode KEY_LEFT_BRACKET(91);
static constexpr KeyCode KEY_BACKSLASH(92);
static constexpr KeyCode KEY_RIGHT_BRACKET(93);
static constexpr KeyCode KEY_GRAVE_ACCENT(96);
static constexpr KeyCode KEY_WORLD_1(161);
static constexpr KeyCode KEY_WORLD_2(162);

static constexpr KeyCode KEY_ESCAPE(256);
static constexpr KeyCode KEY_ENTER(257);
static constexpr KeyCode KEY_TAB(258);
static constexpr KeyCode KEY_BACKSPACE(259);
static constexpr KeyCode KEY_INSERT(260);
static constexpr KeyCode KEY_DELETE(261);
static constexpr KeyCode KEY_RIGHT(262);
static constexpr KeyCode KEY_LEFT(263);
static constexpr KeyCode KEY_DOWN(264);
static constexpr KeyCode KEY_UP(265);
static constexpr KeyCode KEY_PAGE_UP(266);
static constexpr KeyCode KEY_PAGE_DOWN(267);
static constexpr KeyCode KEY_HOME(268);
static constexpr KeyCode KEY_END(269);
static constexpr KeyCode KEY_CAPS_LOCK(280);
static constexpr KeyCode KEY_SCROLL_LOCK(281);
static constexpr KeyCode KEY_NUM_LOCK(282);
static constexpr KeyCode KEY_PRINT_SCREEN(283);
static constexpr KeyCode KEY_PAUSE(284);
static constexpr KeyCode KEY_F1(290);
static constexpr KeyCode KEY_F2(291);
static constexpr KeyCode KEY_F3(292);
static constexpr KeyCode KEY_F4(293);
static constexpr KeyCode KEY_F5(294);
static constexpr KeyCode KEY_F6(295);
static constexpr KeyCode KEY_F7(296);
static constexpr KeyCode KEY_F8(297);
static constexpr KeyCode KEY_F9(298);
static constexpr KeyCode KEY_F10(299);
static constexpr KeyCode KEY_F11(300);
static constexpr KeyCode KEY_F12(301);
static constexpr KeyCode KEY_F13(302);
static constexpr KeyCode KEY_F14(303);
static constexpr KeyCode KEY_F15(304);
static constexpr KeyCode KEY_F16(305);
static constexpr KeyCode KEY_F17(306);
static constexpr KeyCode KEY_F18(307);
static constexpr KeyCode KEY_F19(308);
static constexpr KeyCode KEY_F20(309);
static constexpr KeyCode KEY_F21(310);
static constexpr KeyCode KEY_F22(311);
static constexpr KeyCode KEY_F23(312);
static constexpr KeyCode KEY_F24(313);
static constexpr KeyCode KEY_F25(314);
static constexpr KeyCode KEY_KP_0(320);
static constexpr KeyCode KEY_KP_1(321);
static constexpr KeyCode KEY_KP_2(322);
static constexpr KeyCode KEY_KP_3(323);
static constexpr KeyCode KEY_KP_4(324);
static constexpr KeyCode KEY_KP_5(325);
static constexpr KeyCode KEY_KP_6(326);
static constexpr KeyCode KEY_KP_7(327);
static constexpr KeyCode KEY_KP_8(328);
static constexpr KeyCode KEY_KP_9(329);
static constexpr KeyCode KEY_KP_DECIMAL(330);
static constexpr KeyCode KEY_KP_DIVIDE(331);
static constexpr KeyCode KEY_KP_MULTIPLY(332);
static constexpr KeyCode KEY_KP_SUBTRACT(333);
static constexpr KeyCode KEY_KP_ADD(334);
static constexpr KeyCode KEY_KP_ENTER(335);
static constexpr KeyCode KEY_KP_EQUAL(336);
static constexpr KeyCode KEY_LEFT_SHIFT(340);
static constexpr KeyCode KEY_LEFT_CONTROL(341);
static constexpr KeyCode KEY_LEFT_ALT(342);
static constexpr KeyCode KEY_LEFT_SUPER(343);
static constexpr KeyCode KEY_RIGHT_SHIFT(344);
static constexpr KeyCode KEY_RIGHT_CONTROL(345);
static constexpr KeyCode KEY_RIGHT_ALT(346);
static constexpr KeyCode KEY_RIGHT_SUPER(347);
static constexpr KeyCode KEY_MENU(348);
struct Key{
    u32 code{KEY_UNKNOWN};
    bool pressed{false};
    Key(unsigned char c): code(static_cast<u32>(c)){} // implicit cast from uchar
    operator u32 () const noexcept{ return code; } // implicit cast to u32
};
inline bool operator==(const Key& a, const Key& b) noexcept {
    return a.code == b.code;
}
#include <unordered_map>
namespace std {
template <>
struct hash<Key> {
    size_t operator()(const Key& k) const noexcept {
        return std::hash<u32>{}(k.code);
    }
};
}

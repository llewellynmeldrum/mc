#pragma once
#include "Types.h"
#include "cppslop.hpp"
#include <unordered_map>

enum class KeyState {
    Released,
    JustPressed,
    JustReleased,
    Held,
    INVALID = -1,
};
using KeyCode = i32;
constexpr KeyCode        KEY_MAX(348);
constexpr KeyCode        KEY_MIN(31);
constexpr KeyCode        KEY_COUNT(KEY_MAX-KEY_MIN);
static constexpr KeyCode KEY_UNKNOWN(-1);
static constexpr KeyCode KEY_SPACE(32);
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
static constexpr KeyCode KEY_MENU(348);//////////////////////////////////////////////


struct Key {
    i32  code{ KEY_UNKNOWN };
    bool pressed{ false };
    Key(i32 c) : code(static_cast<i32>(c)) {}  // implicit ctor cast FROM uchar
    operator i32() const noexcept { return code; }       // implicit operator cast TO i32
};

inline bool operator==(const Key& a, const Key& b) noexcept {
    return a.code == b.code;
}

STD_HASH_SPECIALIZATION(Key, k, 
    return std::hash<i32>{}(k.code);
)

template<typename Tv>
constexpr std::unordered_map<KeyCode, Tv> makeKeyMap(Tv __default=Tv{}){
    return std::unordered_map<KeyCode,Tv>{
        {KEY_UNKNOWN, std::forward<Tv>(__default)},
        {KEY_SPACE, std::forward<Tv>(__default)},
        {KEY_APOSTROPHE, std::forward<Tv>(__default)},
        {KEY_COMMA, std::forward<Tv>(__default)},
        {KEY_MINUS, std::forward<Tv>(__default)},
        {KEY_PERIOD, std::forward<Tv>(__default)},
        {KEY_SLASH, std::forward<Tv>(__default)},
        {KEY_ZERO, std::forward<Tv>(__default)},
        {KEY_ONE, std::forward<Tv>(__default)},
        {KEY_TWO, std::forward<Tv>(__default)},
        {KEY_THREE, std::forward<Tv>(__default)},
        {KEY_FOUR, std::forward<Tv>(__default)},
        {KEY_FIVE, std::forward<Tv>(__default)},
        {KEY_SIX, std::forward<Tv>(__default)},
        {KEY_SEVEN, std::forward<Tv>(__default)},
        {KEY_EIGHT, std::forward<Tv>(__default)},
        {KEY_NINE, std::forward<Tv>(__default)},
        {KEY_SEMICOLON, std::forward<Tv>(__default)},
        {KEY_EQUAL, std::forward<Tv>(__default)},
        {KEY_A, std::forward<Tv>(__default)},
        {KEY_B, std::forward<Tv>(__default)},
        {KEY_C, std::forward<Tv>(__default)},
        {KEY_D, std::forward<Tv>(__default)},
        {KEY_E, std::forward<Tv>(__default)},
        {KEY_F, std::forward<Tv>(__default)},
        {KEY_G, std::forward<Tv>(__default)},
        {KEY_H, std::forward<Tv>(__default)},
        {KEY_I, std::forward<Tv>(__default)},
        {KEY_J, std::forward<Tv>(__default)},
        {KEY_K, std::forward<Tv>(__default)},
        {KEY_L, std::forward<Tv>(__default)},
        {KEY_M, std::forward<Tv>(__default)},
        {KEY_N, std::forward<Tv>(__default)},
        {KEY_O, std::forward<Tv>(__default)},
        {KEY_P, std::forward<Tv>(__default)},
        {KEY_Q, std::forward<Tv>(__default)},
        {KEY_R, std::forward<Tv>(__default)},
        {KEY_S, std::forward<Tv>(__default)},
        {KEY_T, std::forward<Tv>(__default)},
        {KEY_U, std::forward<Tv>(__default)},
        {KEY_V, std::forward<Tv>(__default)},
        {KEY_W, std::forward<Tv>(__default)},
        {KEY_X, std::forward<Tv>(__default)},
        {KEY_Y, std::forward<Tv>(__default)},
        {KEY_Z, std::forward<Tv>(__default)},
        {KEY_LEFT_BRACKET, std::forward<Tv>(__default)},
        {KEY_BACKSLASH, std::forward<Tv>(__default)},
        {KEY_RIGHT_BRACKET, std::forward<Tv>(__default)},
        {KEY_GRAVE_ACCENT, std::forward<Tv>(__default)},
        {KEY_WORLD_1, std::forward<Tv>(__default)},
        {KEY_WORLD_2, std::forward<Tv>(__default)},
        {KEY_ESCAPE, std::forward<Tv>(__default)},
        {KEY_ENTER, std::forward<Tv>(__default)},
        {KEY_TAB, std::forward<Tv>(__default)},
        {KEY_BACKSPACE, std::forward<Tv>(__default)},
        {KEY_INSERT, std::forward<Tv>(__default)},
        {KEY_DELETE, std::forward<Tv>(__default)},
        {KEY_RIGHT, std::forward<Tv>(__default)},
        {KEY_LEFT, std::forward<Tv>(__default)},
        {KEY_DOWN, std::forward<Tv>(__default)},
        {KEY_UP, std::forward<Tv>(__default)},
        {KEY_PAGE_UP, std::forward<Tv>(__default)},
        {KEY_PAGE_DOWN, std::forward<Tv>(__default)},
        {KEY_HOME, std::forward<Tv>(__default)},
        {KEY_END, std::forward<Tv>(__default)},
        {KEY_CAPS_LOCK, std::forward<Tv>(__default)},
        {KEY_SCROLL_LOCK, std::forward<Tv>(__default)},
        {KEY_NUM_LOCK, std::forward<Tv>(__default)},
        {KEY_PRINT_SCREEN, std::forward<Tv>(__default)},
        {KEY_PAUSE, std::forward<Tv>(__default)},
        {KEY_F1, std::forward<Tv>(__default)},
        {KEY_F2, std::forward<Tv>(__default)},
        {KEY_F3, std::forward<Tv>(__default)},
        {KEY_F4, std::forward<Tv>(__default)},
        {KEY_F5, std::forward<Tv>(__default)},
        {KEY_F6, std::forward<Tv>(__default)},
        {KEY_F7, std::forward<Tv>(__default)},
        {KEY_F8, std::forward<Tv>(__default)},
        {KEY_F9, std::forward<Tv>(__default)},
        {KEY_F10, std::forward<Tv>(__default)},
        {KEY_F11, std::forward<Tv>(__default)},
        {KEY_F12, std::forward<Tv>(__default)},
        {KEY_F13, std::forward<Tv>(__default)},
        {KEY_F14, std::forward<Tv>(__default)},
        {KEY_F15, std::forward<Tv>(__default)},
        {KEY_F16, std::forward<Tv>(__default)},
        {KEY_F17, std::forward<Tv>(__default)},
        {KEY_F18, std::forward<Tv>(__default)},
        {KEY_F19, std::forward<Tv>(__default)},
        {KEY_F20, std::forward<Tv>(__default)},
        {KEY_F21, std::forward<Tv>(__default)},
        {KEY_F22, std::forward<Tv>(__default)},
        {KEY_F23, std::forward<Tv>(__default)},
        {KEY_F24, std::forward<Tv>(__default)},
        {KEY_F25, std::forward<Tv>(__default)},
        {KEY_KP_0, std::forward<Tv>(__default)},
        {KEY_KP_1, std::forward<Tv>(__default)},
        {KEY_KP_2, std::forward<Tv>(__default)},
        {KEY_KP_3, std::forward<Tv>(__default)},
        {KEY_KP_4, std::forward<Tv>(__default)},
        {KEY_KP_5, std::forward<Tv>(__default)},
        {KEY_KP_6, std::forward<Tv>(__default)},
        {KEY_KP_7, std::forward<Tv>(__default)},
        {KEY_KP_8, std::forward<Tv>(__default)},
        {KEY_KP_9, std::forward<Tv>(__default)},
        {KEY_KP_DECIMAL, std::forward<Tv>(__default)},
        {KEY_KP_DIVIDE, std::forward<Tv>(__default)},
        {KEY_KP_MULTIPLY, std::forward<Tv>(__default)},
        {KEY_KP_SUBTRACT, std::forward<Tv>(__default)},
        {KEY_KP_ADD, std::forward<Tv>(__default)},
        {KEY_KP_ENTER, std::forward<Tv>(__default)},
        {KEY_KP_EQUAL, std::forward<Tv>(__default)},
        {KEY_LEFT_SHIFT, std::forward<Tv>(__default)},
        {KEY_LEFT_CONTROL, std::forward<Tv>(__default)},
        {KEY_LEFT_ALT, std::forward<Tv>(__default)},
        {KEY_LEFT_SUPER, std::forward<Tv>(__default)},
        {KEY_RIGHT_SHIFT, std::forward<Tv>(__default)},
        {KEY_RIGHT_CONTROL, std::forward<Tv>(__default)},
        {KEY_RIGHT_ALT, std::forward<Tv>(__default)},
        {KEY_RIGHT_SUPER, std::forward<Tv>(__default)},
        {KEY_MENU, std::forward<Tv>(__default)},
    };
}

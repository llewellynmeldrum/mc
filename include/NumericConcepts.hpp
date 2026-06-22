#pragma once 
template <typename T>
concept FloatingPoint = std::floating_point<std::remove_cvref_t<T>>;

template <typename T>
concept Integral = std::integral<std::remove_cvref_t<T>>;


template <typename T>
concept Numeric = FloatingPoint<T> || Integral<T>;


template <typename T>
concept IVec2Like = requires(T v){
    {v.x}->Integral;
    {v.y}->Integral;
};

template <typename T>
concept IVec3Like = IVec2Like<T> && requires(T v){
    {v.z}->Integral;
};

template <typename T>
concept IVec4Like =IVec3Like<T> && requires(T v){
    {v.w}->Integral;
};



template <typename T>
concept FVec2Like = requires(T v){
    {v.x}->FloatingPoint;
    {v.y}->FloatingPoint;
};

template <typename T>
concept FVec3Like = FVec2Like<T> && requires(T v){
    {v.z}->FloatingPoint;
};

template <typename T>
concept FVec4Like = FVec3Like<T> && requires(T v){
    {v.w}->FloatingPoint;
};


template <typename T>
concept AnyVec2Like = requires(T v){
    {v.x}->Numeric;
    {v.y}->Numeric;
};

template <typename T>
concept AnyVec3Like = AnyVec2Like<T> && requires(T v){
    {v.z}->Numeric;
};

template <typename T>
concept AnyVec4Like = AnyVec3Like<T> &&  requires(T v){
    {v.w}->Numeric;
};

// exclude higher dimensions 
template <typename T>
concept FVec2 = FVec2Like<T> && !FVec3Like<T> && !FVec4Like<T>;

template <typename T>
concept FVec3 = FVec3Like<T> && !FVec4Like<T>;

template <typename T>
concept FVec4 = FVec4Like<T>;

template <typename T>
concept IVec2 = IVec2Like<T> && !IVec3Like<T> && !IVec4Like<T>;

template <typename T>
concept IVec3 = IVec3Like<T> && !IVec4Like<T>;

template <typename T>
concept IVec4 = IVec4Like<T>;

template <typename T>
concept AnyVec2 = AnyVec2Like<T> && !AnyVec3Like<T> && !AnyVec4Like<T>;

template <typename T>
concept AnyVec3 = AnyVec3Like<T> && !AnyVec4Like<T>;

template <typename T>
concept AnyVec4 = AnyVec4Like<T>;




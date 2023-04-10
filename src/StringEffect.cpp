#include "StringEffect.hpp"

StringEffect::StringEffect(String str, uint32 font_size, Point pos, double duration, double fade_duration)
    :str(str), font_size(font_size), pos(pos), duration(duration), fade_duration(fade_duration){}

bool StringEffect::update(double time) {
    double alpha{1.0};
    
    if(time < fade_duration){
        alpha = std::lerp(0.0, 1.0, time / fade_duration);
    }
    else if(time > duration - fade_duration){
        alpha = std::lerp(0.0, 1.0, (duration - time) / fade_duration);
    }
    
    FontAsset(U"Kaisotai")(str).drawAt(font_size, pos, ColorF{MyColor::White, alpha});
    
    return time < duration;
}

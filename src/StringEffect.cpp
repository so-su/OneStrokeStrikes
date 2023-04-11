#include "StringEffect.hpp"

StringEffect::StringEffect(const String& str, uint32 font_size, Point pos, double duration, double fadein_duration, double fadeout_duration)
    :str(str), font_size(font_size), pos(pos), duration(duration), fadein_duration(fadein_duration), fadeout_duration(fadeout_duration){}

bool StringEffect::update(double time) {
    double alpha{1.0};
    
    if(time < fadein_duration){
        alpha = std::lerp(0.0, 1.0, time / fadein_duration);
    }
    else if(time > duration - fadeout_duration){
        alpha = std::lerp(0.0, 1.0, (duration - time) / fadeout_duration);
    }
    
    FontAsset(U"Kaisotai")(str).drawAt(font_size, pos, ColorF{Palette::White, alpha});
    
    return time < duration;
}

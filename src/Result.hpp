#pragma once
#include "Common.hpp"

// リザルトシーン
class Result : public App::Scene {
   public:
    Result(const InitData& init);

    void update() override;

    void draw() const override;

   private:
    static constexpr Rect frame{100,150,1200,600};
};

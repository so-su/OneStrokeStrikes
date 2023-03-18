# ソースコードを結合して標準出力に出力するスクリプト
# 現状のSiv3D for Webは複数ファイルのコンパイルをサポートしてないため

cd ./src/
cat Common.hpp Parameter.hpp Button.hpp SimpleButton.hpp ProgressBar.hpp SquareEffect.hpp RingEffect.hpp AttackShape.hpp AttackRoulette.hpp Polyomino.hpp Enemy.hpp AlphaEnemy.hpp Player.hpp Title.hpp Game.hpp Result.hpp Ranking.hpp \
Button.cpp SimpleButton.cpp ProgressBar.cpp SquareEffect.cpp RingEffect.cpp AttackShape.cpp AttackRoulette.cpp Polyomino.cpp Enemy.cpp AlphaEnemy.cpp Player.cpp Title.cpp Game.cpp Result.cpp Ranking.cpp Main.cpp \
| grep -v '#' | sed '1s/^/#include <Siv3D.hpp>\n/'

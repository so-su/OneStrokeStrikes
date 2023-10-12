# ソースコードを結合して標準出力に出力するスクリプト
# 現状のSiv3D for Webは複数ファイルのコンパイルをサポートしてないため

cd ./src/
cat Common.hpp Parameter.hpp DriftingPolyominoes.hpp Button.hpp SimpleButton.hpp ProgressBar.hpp SquareEffect.hpp RingEffect.hpp ExplosionEffect.hpp StringEffect.hpp AttackShape.hpp AttackRoulette.hpp Polyomino.hpp Enemy.hpp AlphaEnemy.hpp Player.hpp Title.hpp Game.hpp Result.hpp Ranking.hpp \
DriftingPolyominoes.cpp Button.cpp SimpleButton.cpp ProgressBar.cpp SquareEffect.cpp RingEffect.cpp ExplosionEffect.cpp StringEffect.cpp AttackShape.cpp AttackRoulette.cpp Polyomino.cpp Enemy.cpp AlphaEnemy.cpp Player.cpp Title.cpp Game.cpp Result.cpp Ranking.cpp Main.cpp \
| grep -v '#' | sed '1s/^/#include <Siv3D.hpp>\n/'

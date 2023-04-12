# すとすと

[Siv3D](https://siv3d.github.io/ja-jp/)を使って制作しているパズルゲームです。  
https://so-su.github.io/game/ で遊べます。

 ___
[1. 制作状況](#anchor1)  
[2. こんなゲームです](#anchor2)  
[3. あそびかた](#anchor3)  
[4. 技術的コメント](#anchor4)  
[5. 今後実装、改善すること](#anchor5)  
___

<a id="anchor1"></a>

## 1. 制作状況
ゲームの遊びのロジック部分はほとんど完成しています。いまは、ゲーム体験を向上させるべく、見た目、手触り、インタラクション性などをブラッシュアップしています。

___

<a id="anchor2"></a>

## 2. こんなゲームです
### プレイ動画
※ 定期的に更新していますが、最新のものと異なる部分があるかもしれません。

[![alt設定](http://img.youtube.com/vi/_RKoGQ85wCo/0.jpg)](https://www.youtube.com/watch?v=_RKoGQ85wCo)

### ゲームの目的
- このゲームの目的は、上にある黄色い敵「オオミノ」のブロックをすべて破壊して倒すことです。
- そのために、下にあるカラフルな敵「コミノ」たちを**一筆書き**して倒し、ポイントをためます。
- 同じ色を連続させて一筆書きすると、そのぶんポイントがボーナスされます。
- ポイントがたまるたびに、オオミノのブロックを破壊することができます。

### 敵のギミック
- オオミノ、コミノの外周をゲージが周回しており、これが一周するたびにその敵が行動してきます。
    - オオミノ：場にいるコミノのゲージの周回速度を上昇させます。
    - コミノ：プレイヤーにダメージを与えてきます。
- オオミノのブロックを破壊していくと、はじめはひとつだったオオミノが上下左右に繋がらなくなることで、**いくつものオオミノに分裂してそれぞれが独立に動き始めます**。

冒頭のプレイ動画のように、オオミノをたくさん分裂させてしまうと、コミノのゲージがどんどん加速されて、ダメージが休む間もなく飛んできます！

### ここがおもしろい！
- 敵の特性が図形的な性質で決まる。
    - 大きいコミノ：外周が長いので攻撃の間隔が長い（＝攻撃力が低い）。もらえるポイントもおいしい。でも倒すのは難しい。
    - 小さいコミノ：外周が短いので攻撃の間隔が短い（＝攻撃力が高い）。もらえるポイントもまずい。でも倒すのは易しい。
    - 形によって一筆書きが妙に難しいことも。たまにいるちょっと強い敵みたいで、よいスパイスになる。
- 最初は易しく、だんだん難しくなっていくというゲームの流れを、図形が分裂していく様子によって自然な形で実現した。
- 戦況が図形的に表現されていて、わかりやすい。
    - オオミノがたくさん分裂していたら、穏やかじゃないことはわかる。
    - 後ろから見ている人も、ちょっと説明すれば理解して面白がってくれるかも。

<a id="anchor3"></a>

___

## 3. あそびかた
ゲームのおおまかな説明は、[2. こんなゲームです](#anchor2) をご覧ください。

### ゲームの難易度
「みならい」「じゅくれん」「しょくにん」の3つがあります。

### ゲームの基本
- 一筆書きの操作：まず始点をクリックして、その後にマウスカーソルを動かすことによってすべてのブロックをなぞります（ドラッグする必要はありません）。
- 下にある3色の棒状のゲージは、プレイヤーのステータスです。
- コミノを倒したときに、消したブロックの色によって、各色のゲージがたまります。また、同じ色を連続させて消すと、その長さに応じてボーナスが入ります。
- 緑のゲージが体力です。これが空になるとゲームオーバーです。
- 赤のゲージは満タンになると光ります。この状態でゲージをクリックすると、オオミノに攻撃してブロックを破壊することができます。
    - 攻撃するときは、ルーレットで選ばれた方法でオオミノのブロックを破壊します。
    - 数字が選ばれた場合、その数だけランダムでブロックが破壊されます。
    - 図形が選ばれた場合、攻撃したい位置に合わせてクリックすると、その図形と重なるブロックを破壊できます。すべてのブロックを無駄にすることなくオオミノのブロックを破壊したとき、「ぴったりコンボ」が発生し、続けてもう一度攻撃できます。
    - 攻撃が終わると、次の攻撃時のルーレットが予告されます。
- 青のゲージは満タンになると光ります。この状態でゲージをクリックすると、場にいるコミノをすべて倒すことができます。

### もっと知りたい人向け
- 一筆書きの最中に[Space]キーを押すと、始点と終点が入れ替わります。実は重要テクニックです。
- 場にいるコミノをすべて倒しきると、「オールクリア」ボーナスとして各色のゲージが大きくたまります。
- コミノを倒したとき、一筆書きの始点と終点にした色は、次の攻撃時のルーレットで、対応する色の領域がわずかに大きくなります。
- 右下にある小さなルーレットをクリックすると、ゲームを一時停止して、次の攻撃時のルーレットが確認できます。

<a id="anchor4"></a>

___

## 4. 技術的コメント
このゲームでは、図形（[ポリオミノ](https://ja.wikipedia.org/wiki/ポリオミノ)といいます）の外周をゲージが周回しています。これを実現するために、ポリオミノの外周を計算するアルゴリズムを考えました。
- アルゴリズムの詳細は、[Qiitaに投稿した記事](https://qiita.com/koikuchisauce/items/c3e2e577a144774505b5)をご覧ください。
- `src/AlphaEnemy.cpp`の`AlphaEnemy::new_shape_initialize()` において、アルゴリズムが実装されています。
- このアルゴリズムを思いつくのには、競技プログラミングに取り組むことで鍛えられた能力が活かされました。それは、アルゴリズムの知識というよりは、対象をよく観察して粘り強く考える力だと思います。
- 一方で、実装のうえで「深さ優先探索」によりポリオミノをひとつずつ抽出しながら処理をする部分などは、競技プログラミングに取り組むことで染みついた、基礎的なアルゴリズム力が役に立ったと思います。

<a id="anchor5"></a>

___

## 5. 今後実装、改善すること（の一部）
- 各アクション（ダメージを受ける、ブロックを破壊、敵が出現、など）にエフェクトをつけて、起きている事象に納得感を出す。
- 音楽や効果音をつける。
- ゲージの周回速度が速いときなど、プレイヤーがピンチのときに焦らせるような演出をつける。
- オオミノの生成アルゴリズムを改良して、指定した大きさのオオミノを生成できるようにする。
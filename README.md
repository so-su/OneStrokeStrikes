# すとすと

## これはなに？

[Siv3D](https://siv3d.github.io/ja-jp/)を使って制作しているパズルゲームです。ゲームの「遊び」の部分はほとんど固まったのですが、まだまだこれから作り込んでいく段階にあります。そのため、不親切な点や、不満に感じる点が多くあると思います。あらかじめご容赦ください。

## あそびかた
### ゲームの基本
- このゲームをクリアするには、上の黄色い敵（以下「オオミノ」と呼びます）のブロックをすべて破壊しなければなりません。
- 下の3つのカラフルな敵はオオミノの子分たち（以下、「コミノ」と呼びます）です。コミノは一筆書きをすることで倒せます。一筆書きをするときは、まず始点をクリックして、その後にマウスカーソルを動かすことによってブロックをなぞっていきます（ドラッグする必要はありません）。
- コミノの周りを周回するゲージが一周するたび、プレイヤーはダメージを受けます。
- オオミノの周りを周回するゲージが一周するたび、場にいるコミノのゲージの周回する速さが上昇します。
- 下にある3つの棒状のゲージは、プレイヤーのステータスです。
- コミノを倒したときに、消したブロックの色によって、各色のゲージがたまります。また、同じ色を連続させて消すと、その長さに応じてボーナスが入ります。
- 緑のゲージが体力です。これが空になるとゲームオーバーです。
- 赤のゲージが満タンの状態で[D]キーを押すと、オオミノのブロックを破壊することができます。
- 攻撃するときには、ルーレットで選ばれた方法でオオミノのブロックを破壊します。
    - 数字が選ばれた場合、その数だけランダムでブロックが破壊されます。
    - 図形が選ばれた場合、攻撃したい位置に合わせてクリックすると、その図形と重なるブロックを破壊できます。
- オオミノのブロックを破壊していき、ブロックがタテ、ヨコに繋がらなくなると、分裂したオオミノが、それぞれ独立して行動を始めます。
- 青のゲージが満タンの状態で[F]キーを押すと、場にいるコミノをすべて倒すことができます。

### もっと知りたい人向け
- 一筆書きの最中に[Space]キーを押すと、始点と終点が入れ替わります。これを使いこなせるかどうかで、このゲームのうまさが決まると言っても過言ではありません。
- 右下にあるルーレットをクリックすると、ゲームを一時停止して、次の攻撃時のルーレットが確認できます。
- コミノを倒したとき、一筆書きの始点と終点にした色は、次の攻撃時のルーレットで、対応する色の領域がわずかに大きくなります。
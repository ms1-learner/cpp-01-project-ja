# Drive adventure game
## セットアップと実行の手順
### clone
clone [this repository](https://github.com/takayuki-asoma-lec/cpp-01-project-ja) and checkout [drive_game branch](https://github.com/takayuki-asoma-lec/cpp-01-project-ja/tree/drive_game). 
* HTTPS 
``` 
git clone https://github.com/takayuki-asoma-lec/cpp-01-project-ja.git
cd ./cpp-01-project-ja
git switch drive_game
```
### build
```
cd ./src
g++ -std=c++20 -Wall -pedantic-errors -o driving_game main.cpp myutils.cpp mymap.cpp mycar.cpp
```
### run
- Mac
```
./drivig_game
```
## プロジェクトにおける重要な設計やその設計理由
* 要件を満たしながらも、実装が複雑になりすぎないよう、今回は車の内部状態（速さ、向き等）を離散的な値のみ取る設定。
* 身動きが取れなくなることがないようにUターンコマンドを追加。また、ユーザーが選択可能なコマンドのみを表示する仕様。
## このツールまたはサービスの使い方の説明（ユーザ向けの説明）
* 「表示されたコマンド一覧から実行したいコマンドを選択して入力+Enter => 自車位置が更新」の繰返し
* 終了したい場合Ctrl+C
* ガソリンがなくなると終了。ガソリンスタンドを通過すると燃料が補給される。

- Console出力イメージ
![Console出力イメージ図](/src/images/console.png)
# d-Spline データあてはめ（データフィッティング）

[English](README.md) | [日本語](README-ja.md)

入力されたデータを最適ななめらかさで補完されたデータを出力してグラフ化するライブラリおよびツールである。

READMEは順次更新する予定である

- [準備](#準備)
- [ファイル構成](#ファイル構成)
- [データのあてはめの詳細](#データのあてはめの詳細)
- [ツールとしての使用方法](#ツールとしての使用方法)
- [ライブラリとしての使用方法](#ライブラリとしての使用方法)
- [最新の変更](#最新の変更)

## 準備

### ファイル生成

ルートディレクトリのMakefileを使用して`make`コマンドを実行するだけで必要なファイルが生成される．

``` bash
make
```

### 環境変数への追加

自動生成される `setup_env.sh` を使うと，必要な環境変数設定が行われる．

``` bash
source setup_env.sh
```

## ファイル構成

- dspline
  - bin  
  生成される実行ファイル
  - include  
  main.cに入力するインクルードファイル
  - lib  
  生成されるライブラリファイル
  - src  
  コンパイルに必要なファイル群
  - tool  
  ツール用のmain.c (基本的にいじらない)
  - work  
  オブジェクトファイル群
  - Makefile  
  ツールのビルド用Makefile

## データのあてはめの詳細  

実験データには一般に誤差が含まれている。その誤差を分離し、データ本来の構造を取り出すためにデータのあてはめを行う。

## ツールとしての使用方法

ツールとしての使用方法は[ドキュメント](docs/tool-ja.md)を参照．

## ライブラリとしての使用方法

ライブラリとしての使用方法は[ドキュメント](docs/library-ja.md)を参照．

## 最新の変更

過去の更新については[変更履歴](CHANGELOG-ja.md)を参照．

### Version 0.1.2+docs2 (2025-03-10)

- その他の更新
  - README を更新
- 既知の問題
  - 二相問題用のモデル適用時，人間にとって違和感のある結果となる場合があり，詳細調査中 (2025-03-09)

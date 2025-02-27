# d-Spline データあてはめ（データフィッティング）

入力されたデータを最適ななめらかさで補完されたデータを出力してグラフ化するライブラリおよびツールである。

READMEは順次更新する予定である

- [データのあてはめの詳細](#データのあてはめの詳細)
- [ツールとしての使用方法](docs/tool.md)
- [ライブラリとしての使用方法](docs/library.md)

## ファイル構成

- data_file  
データあてはめを行う際にデータファイルを格納しておく場所
- dspline
  - bin  
  実行ファイル
  - include  
  main.cに入力するインクルードファイル
  - lib  
  静的ライブラリ
  - src  
  コンパイルに必要なファイル群
  - tool  
  ツール用のmain.c(基本的にいじらない)
  - work  
  オブジェクトファイル群
  - Makefile  
  ツールで使用する際のMakefile

## データのあてはめの詳細  

実験データには一般に誤差が含まれている。その誤差を分離し、データ本来の構造を取り出すためにデータのあてはめを行う。

## 最新の変更

過去の更新については[変更履歴](CHANGELOG.md)を参照。

### Version 0.1.0+changelog (2025-02-27)

- ドキュメント更新
  - 変更履歴を追加

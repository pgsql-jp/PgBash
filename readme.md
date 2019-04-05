# PgBash

境田雅明様からPgBash( http://db.psn.ne.jp/pgbash/ )を引き継ぎ、github( https://github.com/pgsql-jp/PgBash )にリポジトリを作成しました。 (2017-12-21 kuwamura)


[[元の索引](index-j.md)] [[ダウンロード](download-j.md)] [[インストール](install-j.md)] [[使用例](example-j.md)] [[FAQ](faq-j.md)]

## PgBash

PgBashは、bashシェルの中に PostgreSQL用のデータベースアクセス機能を組み込んだ**シェル**です。 [シェルスクリプト](./examples/example01.md)を作成してバッチ処理で SQLを実行したり、 端末から[対話的に SQLを直接実行](./examples/example02.md)することができます。 また、簡易的な[Webアプリケーション](./examples/example03.md)を作成することができます。  

* * *

(PgBash最新バージョン)  
[pgbash-8r3.2](./download/pgbash-8r3.2.tar.gz) ... bash-3.2.48ベース  
[pgbash-8r4.1](./download/pgbash-8r4.1.tar.gz) ... bash-4.1.10ベース  
(環境設定ファイル)  
[pgbashrc-9.0](./download/pgbashrc-9.0) ... PostgreSQL-9.0用

* * *

## PgBashの概要

### データベース・インターフェイス
PgBashは、bashシェルの中に PostgreSQL用の直接SQL/埋込SQLインターフェイスを組み込んでいます。 ここで、「直接SQL」とは、SQLを入力すると即座に実行してその結果を標準出力に出力する機能を表します。また、「埋込SQL」とは、検索結果をシェル変数に代入し、シェルスクリプト言語で処理する機能を表します。PgBashでは、SELECT_INTO文 もしくは　FTECH_INTO文を「埋込SQL」として使用することができます。  

### 対話型でSQLを実行
PgBashを、ログインシェルもしくはサブシェル（親のシェルから起動された子のシェル）として起動し、コマンドラインにSQL文を入力して即座に実行することができます。また、SQL文とパイプライン／リダイレクション、バックグランドジョブ命令などを組み合わて実行することもできます。  

### シェルスクリプトを作成可能
PgBashは、bashシェルが持っているシェルスクリプト機能を使用して、シェルスクリプトを作成することができます。bashシェルのIF/WHILEなどの制御文、SQL文や埋込SQL文を使用してシェルスクリプトを作成します。

### Webアプリケーションを作成可能
PgBashは、Webアプリケーション(CGI プログラム)として使用するために、GET/POSTメソッドのデータをシェル変数に自動的にセットする機能を備えています。  

### 実行環境
PgBashは、Unix環境もしくは WindowsのCygwin環境で実行可能です。PgBashは、psql とほぼ同等の機能があり実行環境も psqlと同様になります。  

## 対話型環境設定ファイル
PgBashの対話型環境では、bashシェルと同様に .bash_profile(ログインシェル用)、.bashrc(サブシェル用)環境設定ファイルなどが必要になります。  

また、データベース環境設定ファイルとして **pgbashrc** が必要です。pgbashrcは、対話環境で操作する上で便利なコマンドや、データベース情報を表示するための各種コマンドが設定されています。

pgbashrc は、まず ~/.pgbashrc を読み、存在していなければ /etc/pgbashrc を読みます。PgBashを起動中は、source コマンドによって再読み込みすることができます。  

## PgBashの特徴

### 対話型環境における機能

1. PgBashは、psql と同等のデータベースアクセス機能が有ります。
2. PgBashは、pgbashrc を使用してユーザ独自の対話型環境を簡単に設定できます。
3. PgBashは、bashの標準機能である alias、function、操作履歴の利用などの**柔軟な対話型の操作環境**を提供することができます。
4. PgBashは、SQLと**パイプライン、リダイレクション、バックグランドジョブ**とを組み合わせて実行することができます。

### PgBash独自のSQL拡張機能

1. PgBashは、CONNECT, DISCONNECT, SET CONNECTIONを用いて、**複数のデータベースにアクセス**することができます。
2. PgBashは、「SELECT INTO ：ホスト変数」文や「FETCH INTO ：ホスト変数」文によって**検索結果をシェル変数に代入する機能**があります。
3. PgBashは、SQL実行後のエラーコード、SELECTの結果のタプル数、列数、列名などをシェル変数に保管します。

### Webアプリケーションとしての機能

1. PgBashは、SQL出力をHTMLに切り替え、GET/POSTメソッドによるデータの読込、HTTP_COOKIE の値の読込などを自動的に行う機能があります。
2. PgBashは、SELECTの結果を自動的にHTMLのTABLE形式で出力できます。その際に、TABLE タグ、ヘッダー部の TRタグ、THタグ、ボディ部の TDタグを変更することができます。

* * *

## 使用方法<a id="USAGE" name="USAGE">&nbsp;</a>

以下の説明では、PgBashは"/usr/local/bin"ディレクトリにあるものとします。

1. [PgBashの起動と終了](./usage/usage01-j.md)  

    + [対話型環境](./usage/usage01-j.md#TTY)
    + [シェルスクリプト](./usage/usage01-j.md#SHELL)
    + [CGI（Webアプリケーション）](./usage/usage01-j.md#CGI)

2. [SQLの指定方法](./usage/usage02-j.md)  

    + [SQLの記述方法](./usage/usage02-j.md#SQL)
    + [SQLの実行方法](./usage/usage02-j.md#EXEC_SQL)
    + [SQLの指定方法の制限](./usage/usage02-j.md#LIMIT)
    + [SQL文中のシングル/ダブルクォート](./usage/usage02-j.md#QUOTATION)
    + [SQL文中のコメント](./usage/usage02-j.md#COMMENT)

3. [データベースの接続と切断](./usage/usage03-j.md)  

    + [データベースの接続](./usage/usage03-j.md#CONNECT)
    + [データベースの切断](./usage/usage03-j.md#DISCONNECT)
    + [現在の接続の切替](./usage/usage03-j.md#SETCONNECT)
    + [データベースの接続状態の表示](./usage/usage03-j.md#STATUS)

4. [検索結果の出力](./usage/usage04-j.md)  

    + [検索結果を即座に表示](./usage/usage04-j.md#DIRECT)
    + [検索結果の値をシェル変数に代入](./usage/usage04-j.md#SHELL)
    + [検索結果を１ページごと表示](./usage/usage04-j.md#PIPE)
    + [検索結果をファイルに書き出す](./usage/usage04-j.md#REDIRECT)

5. [SQLの実行オプション](./usage/usage05-j.md)  

    + [オプションの指定方法](./usage/usage05-j.md#OPTION)
    + [set文による定常的なオプション](./usage/usage05-j.md#SETOPTION)
    + [exec_sqlコマンドによる一時的なオプション](./usage/usage05-j.md#TEMPOPTION)

6. [CGI（Webアプリケーション）の使用方法](./usage/usage06-j.md)  

    + [ウェブサーバソフトの設定](./usage/usage06-j.md#CONFL)
    + [シェルスクリプトの記述方法](./usage/usage06-j.md#SHELL)
    + [シェルスクリプトの一般的な作成方法](./usage/usage06-j.md#CGI)

7. [PgBashが使用するシェル変数](./usage/usage07-j.md)  

    + [SQLの実行状態を表すシェル変数](./usage/usage07-j.md#STATUS)
    + [オプション変数](./usage/usage07-j.md#VAR)
    + [エラーコード](./usage/usage07-j.md#ERRCODE)
    + [その他のシェル変数](./usage/usage07-j.md#ETC)

8. [ヘルプおよびデータベース情報の表示について](./usage/usage08-j.md)  

    + [ヘルプメニューの表示方法](./usage/usage08-j.md#HELPMENU)
    + [SQL予約語一覧のヘルプ](./usage/usage08-j.md#RESERVED)
    + [SQL文法のヘルプ](./usage/usage08-j.md#SQLSYNTAX)
    + [対話型環境におけるデータベース情報の表示方法](./usage/usage08-j.md#I_DBINFO)

9. [ラージオブジェクトの取扱](./usage/usage10-j.md)  

    + [_lo_import](./usage/usage10-j.md#LO_IMPORT)
    + [_lo_export](./usage/usage10-j.md#LO_EXPORT)
    + [_lo_unlink](./usage/usage10-j.md#LO_UNLINK)
    + [_lo_list](./usage/usage10-j.md#LO_LIST)
    + [pgbash_descriptionテーブル](./usage/usage10-j.md#DESCRIPTION)

10. [使用制限](./usage/usage11-j.md)  

    + [bashコマンド](./usage/usage11-j.md#BASH)
    + [シェル変数の制限](./usage/usage11-j.md#SHELLVAR)
    + [PostgreSQLのバージョンと互換性](./usage/usage11-j.md#VERSION)

* * *

## 開発履歴

* [2018年〜](./history.md)
* [〜2011年](./history-old.md)

## その他の情報

+ 技術評論社殿のご厚意により、[Software Design 2000年6月号のPgBashの記事](pgbash_sd.pdf)をPDFファイルで公開できることになりました。**但し、印刷配布およびCD-ROMなどのメディアへの転載は禁止です。**

* 境田雅明氏のセミナーでPgBashを紹介した際の資料[フリーソフトウェアを使用したWeb-DB連携システム構築法](pgbash_peak.pdf)です。  

* ふかふかさんの「ふかふかの部屋」サイト、PostgreSQLをSSL付きにした場合の PgBashの修正方法が書かれています。  
[http://www1.plala.or.jp/fukafuka/install/pgbash.html](http://www1.plala.or.jp/fukafuka/install/pgbash.html)  

* PostgreSQL国際開発チーム（本家）サイト:
[http://www.postgresql.org](https://www.postgresql.org)  

* GNU Bash （本家）サイト:
[https://www.gnu.org/software/bash/](https://www.gnu.org/software/bash/)  

* 日本PostgreSQLユーザ会のサイト:
[http://www.postgresql.jp/](https://www.postgresql.jp/)  

* * *

![JPUG GitHub](https://github.com/pgsql-jp/) Last Modified at 2019/04/05

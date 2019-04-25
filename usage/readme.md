[[Home](../index-j.md)] [[ダウンロード](../download-j.md)] [[インストール](../install-j.md)] [[使用方法](./readme.md)] [[使用例](../example-j.md)] [[FAQ](../faq-j.md)]

# PgBash の使用方法

PgBashは、SQLをシェルコマンドとして実行することができます。以下の説明では、PgBashは"/usr/local/bin"ディレクトリにあるものとします。

1. [PgBashの起動と終了](./usage01-j.md)  

    + [対話型環境](./usage01-j.md#TTY)
    + [シェルスクリプト](./usage01-j.md#SHELL)
    + [CGI（Webアプリケーション）](./usage01-j.md#CGI)

2. [SQLの指定方法](./usage02-j.md)  

    + [SQLの記述方法](./usage02-j.md#SQL)
    + [SQLの実行方法](./usage02-j.md#EXEC_SQL)
    + [SQLの指定方法の制限](./usage02-j.md#LIMIT)
    + [SQL文中のシングル/ダブルクォート](./usage02-j.md#QUOTATION)
    + [SQL文中のコメント](./usage02-j.md#COMMENT)

3. [データベースの接続と切断](./usage03-j.md)  

    + [データベースの接続](./usage03-j.md#CONNECT)
    + [データベースの切断](./usage03-j.md#DISCONNECT)
    + [現在の接続の切替](./usage03-j.md#SETCONNECT)
    + [データベースの接続状態の表示](./usage03-j.md#STATUS)

4. [検索結果の出力](./usage04-j.md)  

    + [検索結果を即座に表示](./usage04-j.md#DIRECT)
    + [検索結果の値をシェル変数に代入](./usage04-j.md#SHELL)
    + [検索結果を１ページごと表示](./usage04-j.md#PIPE)
    + [検索結果をファイルに書き出す](./usage04-j.md#REDIRECT)

5. [SQLの実行オプション](./usage05-j.md)  

    + [オプションの指定方法](./usage05-j.md#OPTION)
    + [set文による定常的なオプション](./usage05-j.md#SETOPTION)
    + [exec_sqlコマンドによる一時的なオプション](./usage05-j.md#TEMPOPTION)

6. [CGI（Webアプリケーション）の使用方法](./usage06-j.md)  

    + [ウェブサーバソフトの設定](./usage06-j.md#CONFL)
    + [シェルスクリプトの記述方法](./usage06-j.md#SHELL)
    + [シェルスクリプトの一般的な作成方法](./usage06-j.md#CGI)

7. [PgBashが使用するシェル変数](./usage07-j.md)  

    + [SQLの実行状態を表すシェル変数](./usage07-j.md#STATUS)
    + [オプション変数](./usage07-j.md#VAR)
    + [エラーコード](./usage07-j.md#ERRCODE)
    + [その他のシェル変数](./usage07-j.md#ETC)

8. [ヘルプおよびデータベース情報の表示について](./usage08-j.md)  

    + [ヘルプメニューの表示方法](./usage08-j.md#HELPMENU)
    + [SQL予約語一覧のヘルプ](./usage08-j.md#RESERVED)
    + [SQL文法のヘルプ](./usage08-j.md#SQLSYNTAX)
    + [対話型環境におけるデータベース情報の表示方法](./usage08-j.md#I_DBINFO)

9. [ラージオブジェクトの取扱](./usage10-j.md)  

    + [_lo_import](./usage10-j.md#LO_IMPORT)
    + [_lo_export](./usage10-j.md#LO_EXPORT)
    + [_lo_unlink](./usage10-j.md#LO_UNLINK)
    + [_lo_list](./usage10-j.md#LO_LIST)
    + [pgbash_descriptionテーブル](./usage10-j.md#DESCRIPTION)

10. [使用制限](./usage11-j.md)  

    + [bashコマンド](./usage11-j.md#BASH)
    + [シェル変数の制限](./usage11-j.md#SHELLVAR)
    + [PostgreSQLのバージョンと互換性](./usage11-j.md#VERSION)

* * *

[[Home](../index-j.md)] [[ダウンロード](../download-j.md)] [[インストール](../install-j.md)] [[使用方法](./readme.md)] [[使用例](../example-j.md)] [[FAQ](../faq-j.md)]

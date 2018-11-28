\[[Home](index-j.html)\] \[[ダウンロード](download-j.html)\] \[[インストール](install-j.html)\] \[[使用方法](usage-j.html)\] \[[使用例](example-j.html)\] \[[FAQ](faq-j.html)\]

  
**pgbash の使用方法**  
  

　pgbashは、SQL; をシェルコマンドとして実行することができます。以下の説明では、pgbashは"/usr/local/bin"ディレクトリにあるものとします。

  

> 1.  [pgbashの起動と終了](usage/usage01-j.html)  
>       
>     
>     *   [対話型環境](usage/usage01-j.html#TTY)
>     *   [シェルスクリプト](usage/usage01-j.html#SHELL)
>     *   [CGI（Webアプリケーション）](usage/usage01-j.html#CGI)
>     
>       
>     
> 2.  [SQLの指定方法](usage/usage02-j.html)  
>       
>     
>     *   [SQLの記述方法](usage/usage02-j.html#SQL)
>     *   [SQLの実行方法](usage/usage02-j.html#EXEC_SQL)
>     *   [SQLの指定方法の制限](usage/usage02-j.html#LIMIT)
>     *   [SQL文中のシングル/ダブルクォート](usage/usage02-j.html#QUOTATION)
>     *   [SQL文中のコメント](usage/usage02-j.html#COMMENT)
>     
>       
>     
> 3.  [データベースの接続と切断](usage/usage03-j.html)  
>       
>     
>     *   [データベースの接続](usage/usage03-j.html#CONNECT)
>     *   [データベースの切断](usage/usage03-j.html#DISCONNECT)
>     *   [現在の接続の切替](usage/usage03-j.html#SETCONNECT)
>     *   [データベースの接続状態の表示](usage/usage03-j.html#STATUS)
>     
>       
>     
> 4.  [検索結果の出力](usage/usage04-j.html)  
>       
>     
>     *   [検索結果を即座に表示](usage/usage04-j.html#DIRECT)
>     *   [検索結果の値をシェル変数に代入](usage/usage04-j.html#SHELL)
>     *   [検索結果を１ページごと表示](usage/usage04-j.html#PIPE)
>     *   [検索結果をファイルに書き出す](usage/usage04-j.html#REDIRECT)
>     
>       
>     
> 5.  [SQLの実行オプション](usage/usage05-j.html)  
>       
>     
>     *   [オプションの指定方法](usage/usage05-j.html#OPTION)
>     *   [set文による定常的なオプション](usage/usage05-j.html#SETOPTION)
>     *   [exec\_sqlコマンドによる一時的なオプション](usage/usage05-j.html#TEMPOPTION)
>     
>       
>     
> 6.  [CGI（Webアプリケーション）の使用方法](usage/usage06-j.html)  
>       
>     
>     *   [ウェブサーバソフトの設定](usage/usage06-j.html#CONFL)
>     *   [シェルスクリプトの記述方法](usage/usage06-j.html#SHELL)
>     *   [シェルスクリプトの一般的な作成方法](usage/usage06-j.html#CGI)
>     
>       
>     
> 7.  [pgbashが使用するシェル変数](usage/usage07-j.html)  
>       
>     
>     *   [SQLの実行状態を表すシェル変数](usage/usage07-j.html#STATUS)
>     *   [オプション変数](usage/usage07-j.html#VAR)
>     *   [エラーコード](usage/usage07-j.html#ERRCODE)
>     *   [その他のシェル変数](usage/usage07-j.html#ETC)
>     
>       
>     
> 8.  [ヘルプおよびデータベース情報の表示について](usage/usage08-j.html)  
>       
>     
>     *   [ヘルプメニューの表示方法](usage/usage08-j.html#HELPMENU)
>     *   [SQL予約語一覧のヘルプ](usage/usage08-j.html#RESERVED)
>     *   [SQL文法のヘルプ](usage/usage08-j.html#SQLSYNTAX)
>     *   [対話型環境におけるデータベース情報の表示方法](usage/usage08-j.html#I_DBINFO)
>     *   [シェルスクリプトにおけるデータベース情報の表示方法](usage/usage08-j.html#C_DBINFO)
>     
>       
>     
> 9.  [ラージオブジェクトの取扱](usage/usage10-j.html)  
>       
>     
>     *   [\_lo\_import](usage/usage10-j.html#LO_IMPORT)
>     *   [\_lo\_export](usage/usage10-j.html#LO_EXPORT)
>     *   [\_lo\_unlink](usage/usage10-j.html#LO_UNLINK)
>     *   [\_lo\_list](usage/usage10-j.html#LO_LIST)
>     *   [pgbash\_descriptionテーブル](usage/usage10-j.html#DESCRIPTION)
>     
>       
>     
> 10.  [使用制限](usage/usage11-j.html)  
>       
>     
>     *   [bashコマンド](usage/usage11-j.html#BASH)
>     *   [シェル変数の制限](usage/usage11-j.html#SHELLVAR)
>     *   [PostgreSQLのバージョンと互換性](usage/usage11-j.html#VERSION)
>     
>       
>     

* * *

\[[Home](index-j.html)\] \[[ダウンロード](download-j.html)\] \[[インストール](install-j.html)\] \[[使用方法](usage-j.html)\] \[[使用例](example-j.html)\] \[[FAQ](faq-j.html)\]
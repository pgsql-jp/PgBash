\[[Home](index-j.md)\] \[[ダウンロード](download-j.md)\] \[[インストール](install-j.md)\] \[[使用方法](usage-j.md)\] \[[使用例](example-j.md)\] \[[FAQ](faq-j.md)\]

  

**p g b a s h**

  

pgbashは、bashシェルの中に PostgreSQL用のデータベースアクセス機能を組み込んだ**シェル**です。 [シェルスクリプト](./examples/example01.md)を作成してバッチ処理で SQLを実行したり、 端末から[対話的に SQLを直接実行](./examples/example02.md)することができます。 また、簡易的な[Webアプリケーション](./examples/example03.md)を作成することができます。  

  
  

* * *

(pgbash最新バージョン)  
[pgbash-8r3.2](./download/pgbash-8r3.2.tar.gz) ... bash-3.2.48ベース  
[pgbash-8r4.1](./download/pgbash-8r4.1.tar.gz) ... bash-4.1.10ベース  
(環境設定ファイル)
[pgbashrc-9.0](./download/pgbashrc-9.0) ... PostgreSQL-9.0用

* * *

  

**◎ pgbashの概要**  

*   **データベース・インターフェイス**  
    pgbashは、bashシェルの中に PostgreSQL用の直接SQL/埋込SQLインターフェイスを組み込んでいます。 ここで、「直接SQL」とは、SQLを入力すると即座に実行してその結果を標準出力に出力する機能を表します。また、「埋込SQL」とは、検索結果をシェル変数に代入し、シェルスクリプト言語で処理する機能を表します。Pgbashでは、SELECT\_INTO文 もしくは　FTECH\_INTO文を「埋込SQL」として使用することができます。  
      
    
*   **対話型でSQLを実行**  
    Pgbashを、ログインシェルもしくはサブシェル（親のシェルから起動された子のシェル）として起動し、コマンドラインにSQL文を入力して即座に実行することができます。また、SQL文とパイプライン／リダイレクション、バックグランドジョブ命令などを組み合わて実行することもできます。  
      
    
*   **シェルスクリプトを作成可能**  
    Pgbashは、bashシェルが持っているシェルスクリプト機能を使用して、シェルスクリプトを作成することができます。bashシェルのIF/WHILEなどの制御文、SQL文や埋込SQL文を使用してシェルスクリプトを作成します。  
      
    
*   **Webアプリケーションを作成可能**  
    Pgbashは、Webアプリケーション(CGI プログラム)として使用するために、GET/POSTメソッドのデータをシェル変数に自動的にセットする機能を備えています。  
      
    
*   **実行環境**  
    Pgbashは、Unix環境もしくは WindowsのCygwin環境で実行可能です。Pgbashは、psql とほぼ同等の機能があり実行環境も psqlと同様になります。  
      
    
*   **対話型環境設定ファイル**  
    Pgbashの対話型環境では、bashシェルと同様に .bash\_profile(ログインシェル用)、.bashrc(サブシェル用)環境設定ファイルなどが必要になります。  
      
    また、データベース環境設定ファイルとして **pgbashrc** が必要です。pgbashrcは、対話環境で操作する上で便利なコマンドや、データベース情報を表示するための各種コマンドが設定されています。  
      
    pgbashrc は、まず ~/.pgbashrc を読み、存在していなければ /etc/pgbashrc を読みます。Pgbashを起動中は、source コマンドによって再読み込みすることができます。  
      
    

  
**◎ pgbashの特徴**  

*   対話型環境における機能
    
    1.  pgbashは、psql と同等のデータベースアクセス機能が有ります。
    2.  pgbashは、pgbashrc を使用してユーザ独自の対話型環境を簡単に設定できます。
    3.  pgbashは、bashの標準機能である alias、function、操作履歴の利用などの**柔軟な対話型の操作環境**を提供することができます。
    4.  pgbashは、SQLと**パイプライン、リダイレクション、バックグランドジョブ**とを組み合わせて実行することができます。
    
      
    
*   pgbash独自のSQL拡張機能
    
    1.  pgbashは、CONNECT, DISCONNECT, SET CONNECTIONを用いて、**複数のデータベースにアクセス**することができます。
    2.  pgbashは、「SELECT INTO ：ホスト変数」文や「FETCH INTO ：ホスト変数」文によって**検索結果をシェル変数に代入する機能**があります。
    3.  pgbashは、SQL実行後のエラーコード、SELECTの結果のタプル数、列数、列名などをシェル変数に保管します。
    
      
    
*   Webアプリケーションとしての機能
    1.  pgbashは、SQL出力をHTMLに切り替え、GET/POSTメソッドによるデータの読込、HTTP\_COOKIE の値の読込などを自動的に行う機能があります。
    2.  pgbashは、SELECTの結果を自動的にHTMLのTABLE形式で出力できます。その際に、TABLE タグ、ヘッダー部の TRタグ、THタグ、ボディ部の TDタグを変更することができます。

* * *

  
**◎ 開発履歴**  

*   1999.07.初 : pgsql-jp メーリングリスト(ML)にてインターフェイスの検討。
*   1999.07.中 : 久保健洋さんが pqbashを作成して pgsql-jp MLで公開。
*   1999.07.末 : 直接SQL/埋込SQLのpgbashを pgsql-jp MLで検討。
*   1999.08.31 : pgbash-1.0-beta を公開。
*   1999.09.30 : pgbash-1.1.0を公開(CGI機能の追加、出力形式の統一）
*   1999.10.30 : pgbash-1.2.1を公開(操作環境の整備とCOPY文の機能追加）
*   1999.11.22 : pgbash-1.2.3を公開(SQLOIDシェル変数の追加)  
    
*   1999.12.25 : pgbash-2.0を公開 (bash-2.03本体を修正してSQL実行機能を追加)
*   2000.01.07 : pgbash-2.0.1を公開(MB関数リンク、システムメッセージ出力制御追加)
*   2000.05.16 : pgbash-2.1(bash-2.03) を公開(PostgreSQL-6.5/7.0に対応)
*   2001.05.01 : pgbash-2.2(bash-2.03/2.04/2.05)を公開(PostgreSQL-6.5/7.0/7.1に対応)
*   2001.12.15 : pgbash-2.4(PostgreSQL-7.0/7.1/7.2に対応)を公開
*   2002.02.10 : pgbash-2.4a(bash-2.05aに対応)を公開
*   2002.05.10 : pgbash-2.4a.1を公開
*   2002.07.22 : pgbash-2.4a.2を公開
*   2003.02.10 : pgbash-7.3を公開
*   2003.10.14 : pgbash-7.3a(pgbash-7.3 bug fix) by Tomo Nomura.
*   2011.08.15 : pgbash-8r2.05 を公開
*   2011.08.20 : pgbash-8r3.2 を公開*   2011.08.25 : pgbash-8r4.1 を公開

  
**◎ 主な更新内容**(2011年8月)  

*   PostrgeSQL 7.4,8.0, 8.1, 8.2, 8.3, 8.4, 9.0 に対応  
    
*   bash 2.05a, bash-3.2.48, bash-4.1.10 に対応  
    
*   pgbash独自のCOPY機能を削除  
    
*   Webアプリケーション作成時の EXEC\_SQL\_PREPARE機能を削除  
    

  
**◎ 関連ページ**  

*   PostgreSQL本家のページです。  
    [http://www.postgresql.org](http://www.postgresql.org)  
    
*   日本PostgreSQLユーザ会のページです。  
    [http://www.postgresql.jp/](http://www.postgresql.jp/)  
    
*   石井さんのPostgreSQLに関するページです。  
    [http://www2b.biglobe.ne.jp/~caco/pgpage/](http://www2b.biglobe.ne.jp/~caco/pgpage/)  
    
*   ふかふかさんの「ふかふかの部屋」に、PostgreSQLをSSL付きにした場合の Pgbashの修正方法が書かれています。  
    [http://www1.plala.or.jp/fukafuka/install/pgbash.html](http://www1.plala.or.jp/fukafuka/install/pgbash.html)  
    
*   技術評論社殿のご厚意により、[Software Design 2000年6月号のpgbashの記事](pgbash_sd.pdf)をPDFファイルで公開できることになりました。但し、印刷配布およびCD-ROMなどのメディアへの転載は禁止です。  
    *   あるセミナーでpgbashを紹介したときの[プレゼンテーション資料](pgbash_peak.pdf)です。  
      
    

  

* * *

![](/cgi-bin/Count.cgi?df=pgbash-jp|dd=E|ft=2) Last Modified at 2011/08/25

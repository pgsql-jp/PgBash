\[[Home](index-j.md)\] \[[ダウンロード](download-j.md)\] \[[インストール](install-j.md)\] \[[使用方法](usage-j.md)\] \[[使用例](example-j.md)\] \[[FAQ](faq-j.md)\]

  
**Ｆ Ａ Ｑ**  

  

  

**インストールに関するFAQ**  

*   [pgbashは、bashのどのバージョンを使用しますか？](#Q1030)
*   [pgbashは、PostgreSQLのどのバージョンを使用しますか？](#Q1031)
*   [独自の対話型操作環境を作成したのですがどうすれば良いですか？](#Q1041)
*   [シェルスクリプトは存在しているが"No such file or directory"で実行できません。](#Q1042)

  
**bashの操作に関するFAQ**  

*   [シェルスクリプトで alias相当の指定ができますか？](#Q1040)
*   [pgbashの標準エラー出力を標準出力に切り替えることができますか？](#Q1060)

  
**データベースの接続に関するFAQ**  

*   [CONNECT文を使用しなくても自動的にデータベースに接続してくれますか？](#Q2001)
*   [CONNECT文で接続名を指定しなかった場合はどうなりますか？](#Q2002)
*   [セッションが切れた場合 backendは終了しますか？](#Q2009)
*   [SQL使用中にbackendが死んだ場合はどうなりますか？](#Q2010)

  
**SQLの操作に関するFAQ**  

*   [SQL文はそのまま backend に送信されますか？](#Q3010)
*   [列の値がNULLであること判断できますか？](#Q3060)
*   [データベース情報を表示するコマンドはどのような　QUERY をバックエンドに送信しているのですか？](#Q3070)

  
**検索結果の出力に関するFAQ**  

*   [pgbash の検索結果と psql の検索結果は同じですか？](#Q4009)
*   [Select Into 文で検索結果が複数タプルの場合はどうなりますか？](#Q4040)
*   [Fetch Into 文で　検索数とか'ALL'を指定したらどうなりますか？](#Q4050)
*   [スペースとNULLとゼロビット(空)とを区別して表示できますか？](#Q4090)
*   [SELECTの結果をCSV形式でファイルに出力することができますか？](#Q4100)
*   [検索した時の列名を取得する方法はありますか？](#Q4200)

  
**Webアプリケーションに関するFAQ**  

*   [pgbashはCGIプログラムとして利用することができますか？](#Q5100)
*   [HTTP クッキーを処理する方法が提供されていますか？](#Q5110)

  

* * *

  

**Ｑ．pgbashは、bashのどのバージョンを使用しますか？**  

pgbash-8r2.05は bash-2.05a, pgbash-8r3.2 は bash-3.2.48, pgbash-8r4.1 は bash-4.1.10 のソースファイルを使用します。  

**Ｑ．pgbashは、PostgreSQLのどのバージョンを使用しますか？**  

pgbash は　PostgreSQL-7.4以降の libpqライブラリを使用します。 そして、PostgreSQL-7.4, PostgreSQL-8.0 から 8.4 および PostgreSQL 9.0 サーバに接続できます。libpqライブラリのPostgreSQLバージョンと、サーバ側のPostgreSQLバージョンは一致しなくてもかまいません。  

**Ｑ．独自の対話型操作環境を作成したのですがどうすれば良いですか？**  

　/etc/pgbashrc をホームディレクトリに .pgbashrc としてコピーし、そのファイルに独自の対話型操作環境を追加してください。  
  

**Ｑ．シェルスクリプトは存在しているが"No such file or directory"で実行できません。**  

シェルスクリプトの改行コードは'**LF**'です。'CR+LF'コードになっていると、このエラーになります。もちろん、パスが間違っても同じです。  

**Ｑ．シェルスクリプトで alias相当の指定ができますか？**  

シェルスクリプト内では alias は使用できません。例えば、次のように function を定義して alias と同様に使用することができます。

 #!/usr/local/bin/pgbash
 function E {
       exec\_sql "$\*"
 }
  E "select \* from test" **Ｑ．pgbashの標準エラー出力を標準出力に切り替えることができますか？**  

はい、できます。**exec 2>&1** とします。CGI用のシェルスクリプトを作成する場合は、プログラムの先頭で必ず指定した方が良いでしょう。

 #!/usr/local/bin/pgbash
 **exec  2>&1** 
 echo "Content-type: text/html"
 echo ""
 set EXEC\_SQL\_OPTION CGI;

* * * **Ｑ．CONNECT文を使用しなくても自動的にデータベースに接続してくれますか？**  

CONNECT文を実行せずに SQL文を実行した場合は、**ログインユーザ名をユーザ名**とし、**ユーザ名と同じ名前のデータベース名**に対して自動的に接続します。  
例えば、ログインユーザ名が admin の場合は、**connect to default;** は **connect to admin user admin;**が実行されたことと同じになります。  
  
　但し、パスワードが設定されているデータベースは、"CONNECT データベース名 user **ユーザ名 \[パスワード\]**"の形式で CONNECT文を実行しなければなりません。CONNECT文の最後にパスワードを記述しなかった場合は、パスワードの入力要求メッセージが表示されますので、それにしたがってパスワードを入力しなければなりません。  

**Ｑ．CONNECT文で接続名を指定しなかった場合はどうなりますか？**  

"CONNECT TO データベース名 AS 接続名"　の接続名を省略した場合は、データベース名と同じ名前になります。次の例では接続名はいずれも"admin"です。

  connect to admin;
  connect to admin@www.psn.co.jp:5432; **Ｑ．セッションが切れた場合 backendは終了しますか？**  

pgbashのシェルスクリプトが終了すると、セッションが切れ、backendは自動的に終了します。対話型でpgbashを使用している場合は、ログアウトするかもしくは、"disconnect 接続名またはALL"で backendは終了します。

**Ｑ．SQL使用中にbackendが死んだ場合はどうなりますか？**  

SQL使用中に、backendプロセスが何らかの理由で死ぬと、backendを再起動しても"SQL\_BAD\_RESPONSE"のエラーが発生してデータ操作ができなくなります。次のいずれかの処理を行わなければなりません。  
  
(対処)  
・ disconnect all; でデータベースの切断を行う。  
・ connect to xxx as db123；のように新しい接続名で接続する。  
・ pgbashをログインシェルとして使用している場合、ログアウトして再度ログインする。  
・ pgbashをサブシェルとして使用している場合は、exit してして再度pgbashを起動する。  
  

* * *

  
**Ｑ．SQL文はそのまま backend に送信されますか？**  

次のSQL文は、pgbashの内部で処理されます。  
  
・ CONNECT 文、 DISCONNECT 文、 SET CONNECTION 文  
・ SET EXEC\_SQL\_OPTION文、SET OPTION\_NAME=VALUE; 文  
・ FETCH INTO 文の INTO句　(INTO句を除いた文が backend に送信される)  
・ SELECT INTO 文の INTO句　(INTO句を除いた文が backend に送信される)  
  
上記のSQL文を除いた他の全てのSQL文はそのまま backend に送信されます。  

**Ｑ．列の値がNULLであることを判断できますか？**  

　はい、判断できます。例えば、

  begin;
  declare cur cursor for select \* from test;
  fetch in cur into :aaa **:aaa\_indi**, :bbb :bbb\_indi;
  **if(( aaa\_indi == SQL\_NULL ))**; then
     ...
  fi
  end;

のように fetch into の標識変数(インディケータ)が SQL\_NULLと等しいことで判断できます。ちなみに、そのときのシェル変数($aaa)には '\\0' (１バイト目がビットオフ)が代入されています。echo $aaaとしても何も表示されません。  

**Ｑ．データベース情報を表示するコマンドはどのような　QUERY をバックエンドに送信しているのですか？**  

　これを知るには、**E+** を入力した後にデータベース情報表示コマンドを実行してください。表示結果の先頭に、バックエンドに送信される QUERY が表示されます。元に戻すには　**E-** を入力します。  

   pgbash> E+
   pgbash> ?l
   \[ List of databases \]
   ########## QUERY ##########
   SELECT  pg\_database.datname as "Database", pg\_user.usename as "Owner",
     pg\_encoding\_to\_char(pg\_database.encoding) as "Encoding"
    FROM pg\_database, pg\_user
    WHERE pg\_database.datdba = pg\_user.usesysid
    UNION
    SELECT pg\_database.datname as "Database", NULL as "Owner",
     pg\_encoding\_to\_char(pg\_database.encoding) as "Encoding"
    FROM pg\_database
    WHERE pg\_database.datdba NOT IN (SELECT usesysid FROM pg\_user)
    ORDER BY "Database"
   ###########################
 
   Database |Owner   |Encoding
   ---------+--------+---------
   admin    |postgres|EUC\_JP
   postgres |postgres|SQL\_ASCII
   template0|postgres|SQL\_ASCII
   template1|postgres|SQL\_ASCII **Ｑ．pgbash の検索結果と psql の検索結果は同じですか？**  

はい、基本的には同じです。但し、pgbashは次の機能を拡張しています。  
  
・外枠罫線の出力有無のコントロール  
・NULLや ZERO(文字型のAll bit off)に適当な文字列を表示する機能  
・表形式の出力において、CHAR型であってもデータの後ろのスペースを削除して罫線を揃えます。  

**Ｑ．Select Into 文で検索結果が複数タプルの場合はどうなりますか？**  

検索結果が複数タプルの場合は、into句の処理(シェル変数に検索結果を代入）はされません。検索結果は、標準出力(画面)に出力されてしまいます。  

**Ｑ．Fetch Into 文で　検索数とか'ALL'を指定したらどうなりますか？**  

　fetch ALL in cur into :aaa, :bbb, :ccc; のようにした場合、検索結果が複数タプルの場合は、into句の処理はされません。標準出力(画面)に出力されてしまいます。ALLを指定しても検索結果が１タプルの場合は、into句は処理され、シェル変数 aaa,bbb, ccc に値がセットされます。一般的には、into句を指定する場合は検索数とか'ALL'は指定しません。

**Ｑ．スペースとNULLとゼロビット(空)とを区別して表示できますか？**  

　はい、可能です。例えば、NULLを"-NULL-"、空を"-0-"にした場合は次のようにします。

set OPTION\_NULLSTRING="-NULL-";
set OPTION\_ZEROSTRING="-0-";
select \* from test;

code  |name      |address
------+----------+--------
222   |test2     |addr2
333   |          |-0-
444   |-NULL-    |-NULL-
(3 rows) **Ｑ．SELECTの結果をCSV形式でファイルに出力することができますか？**  

はい、可能です。一時的にCSV形式にするには、  
  
exec\_sql -S ',' -TBA "select ... from ... where ..."  
  
全ての出力を CSV形式にする場合は、次の方が便利です。  
  
set OPTION\_SEPARATOR=',';  
set OPTION\_HEADER=OFF;  
set OPTION\_BOTTOM=OFF;  
set OPTION\_ALIGNMENT=OFF;  
のようにオプションを指定しておいて、SELECTを実行します。  

**Ｑ．検索した時の列名を取得する方法はありますか？**  

はい、あります。$SQLNFIELD に列数、${SQLFIELDNAME\[i\]}に列名がセットされています。例えば、次のような方法で列名を表示することができます。

  select \* from test;
  declares -i x; let x=0; while(( x < **SQLNFIELD** ))
  do
      echo "NAME=**${SQLFIELDNAME\[x\]}**"
      let x=x+1
  done

* * * **Ｑ．pgbashはCGIプログラムとして利用することができますか？**  

　はい、できます。すべての出力を HTML出力に切り替え、GET/POSTメソッドにデータを読み込みシェル変数にセットする機能があります。pgbashをCGIプログラムとするには次の文が必要です。但し、exec 2>&1 は必須ではありません。

 #!/usr/local/bin/pgbash
 **exec 2>&1    ................ pgbashの標準エラー出力を標準出力に切り替える**
 **echo "Content-type: text/html"**
 **echo ""**
 **set EXEC\_SQL\_OPTION CGI; .... HTML出力、GET/POSTのデータ処理** **Ｑ．HTTP クッキーを処理する方法が提供されていますか？**  

はい、提供されています。set EXEX\_SQL\_OPTION CGI; を実行すると $HTTP\_NCOOKIEにクッキーの個数、${HTTP\_COOKIEKEY\[i\]}にキー名、${HTTP\_COOKIEVAL\[i\]}にクッキーの値がセットされます。（クッキーの文字列は $HTTP\_COOKIE にセットされています）

 #!/usr/local/bin/pgbash
 exec 2>&1
 **echo "Content-type: text/html"**
 **echo "Set-Cookie: key11=111"**
 **echo "Set-Cookie: kei22=222"**
 **echo ""**
 **set EXEC\_SQL\_OPTION CGI;**
 # 
 declares -i x; let x=0; while(( x < **HTTP\_NCOOKIE** ))
 do
        echo "**${HTTP\_COOKIEKEY\[x\]** = **${HTTP\_COOKIEVAL\[x\]}**"
	let x=x+1
 done 

* * *

\[[Home](index-j.md)\] \[[ダウンロード](download-j.md)\] \[[インストール](install-j.md)\] \[[使用方法](usage-j.md)\] \[[使用例](example-j.md)\] \[[FAQ](faq-j.md)\]
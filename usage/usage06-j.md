\[[Prev](usage05-j.md)\] \[[Menu](../usage-j.md)\] \[[Next](usage07-j.md)\]

* * *

**CGI（Webアプリケーション）の使用方法**  

* * *

  

*   **ウェブサーバソフトの設定**  
      
    Pgbashを CGIとして実行する場合、LD\_LIBRARY\_PATH環境変数に PostrgeSQLのライブラリパス名を指定する必要があります。apacheウェブサーバソフトの場合は、"httpd.conf"の先頭付近に次のように指定します。  
      
    **SetEnv LD\_LIBRARY\_PATH /usr/local/pgsql/lib**  
      
    
*   **シェルスクリプトの記述方法**  
      
    次のコマンドをシェルスクリプトの先頭に指定します。  
      
    
    #!/usr/local/bin/pgbash  
    exec 2>&1 ............ (1)  
    echo "Content-type: text/html" ........................ (2)  
    echo ""  
    **set EXEC\_SQL\_OPTION CGI;** ......(3)  
    
      
    **(1)** exec 2>&1  
    exec 2>1 は、標準エラー出力を標準出力に出力するための指定です。必須ではありませんが、bashが出力するエラーメッセージをホームページに表示することができます。  
      
    **(2)** Content-type  
    CGIとして使用するために、echo "Content-type: text/html"と echo ""は必須です。  
      
    **(3)** set EXEC\_SQL\_OPTION CGI;  
    set EXEC\_SQL\_OPTION CGI; は、ホームページで入力されたデータをシェル変数に登録し、出力モードをHTMLにします。  
      
    HTMLのFORM文では、GET/POSTメソッドを使用できます。また、クッキーがセットされている場合、クッキーの値は、次のシェル変数に登録されます。  
    
      **$HTTP\_NCOOKIE       ** : クッキーの個数
      **${HTTP\_COOKIEKEY\[i\]}** : クッキーのキー名
      **${HTTP\_COOKIEVAL\[i\]}** : クッキーの値   
     (但し、i は 0 から HTTP\_NCOOKIE-1 までです。)
     
    
      
    
*   **シェルスクリプトの一般的な作成方法**  
      
    Webサーバ上に、例えば次のようなHTMLファイル(ホームページ)があるものとします。ここでは、passwd, MYNAME, ADDRESS の３つのデータを入力するものとします。  
      
    
    <HTML>  
    <FORM METHOD=POST ACTION="test.sh">  
    <INPUT TYPE=password NAME=**passwd** >  
    <INPUT TYPE=text NAME="**MYNAME**" VALUE="sakaida">  
    <INPUT TYPE=text NAME="**ADDRESS**" VALUE="Osaka">  
    <INPUT TYPE=submit VALUE=submit>  
    </HTML>  
    
      
    この場合、ホームページから入力された３つのデータは、CGIシェルスクリプトの set EXEC\_SQL\_OPTION CGI; によって、それぞれ $passwd, $MYNAME, $ADDRESS シェル変数に格納されます。そのシェル変数を利用したCGIスクリプトの例は次のようになります。  
      
    
    #!/usr/local/bin/pgbash  
    exec 2>&1  
    echo "Content-type: text/html"  
    echo ""  
    **set EXEC\_SQL\_OPTION CGI;**  
    #  
    connect to dbname as db1 user nopbody **$passwd**;  
    insert into test values( '**$MYNAME**','**$ADDRESS**');  
    select \* from test order by uname limit 100;  
    
      
    pgbashは、マルチバイト文字のシェル変数名（例えば、$氏名）を使用することはできません。従いまして、HTMLのINPUTタグのNAMEにおいて、全角文字を指定することはできないことになります。  
      
    
    * * *
    
    \[[Prev](./usage05-j.md)\] \[[Menu](../usage-j.md)\] \[[Next](./usage07-j.md)\]

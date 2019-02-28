\[[Prev](usage03-j.md)\] \[[Menu](../usage-j.md)\] \[[Next](usage05-j.md)\]

* * *

**検索結果の出力**  

* * *

  

*   **検索結果を表を即座に表示**  
      
    (1) 対話的な操作  
      
    端末から操作している場合、select文を実行すると検索結果の表は即座に端末に表示されます。例えば、端末から、connect文やselect文を直接入力した場合を次ぎに示します。  
      
    pgbash> connect to postgres\_db user xxx NULL;  
    pgbash> select \* from friends;  
    
     user\_id | name     | zip      | pref\_id | address      
    ---------+----------+----------+---------+------------
          22 | 野xx洋子 | 611-2222 |      77 | 神戸市西区   
          23 | 木xx忠志 | 622-3333 |      75 | 神戸市東区   
          24 | 鈴xx信彦 | 633-4444 |      77 | 神戸市南区   
    ....
    (12 rows)
    
      
    (2) シェルスクリプトを実行  
      
    シェルスクリプトを実行した場合も対話的な操作と同様で、select文を実行すると検索結果の表は即座に端末に表示されます。例えば、"go.bat"シュルスクリプトファイルを作成して実行した場合を次に示します。  
      
    \["go.bat"ファイル作成\]  
    
    #!/usr/local/bin/pgbash
    connect to postgres\_db user xxx NULL;
    select \* from from friends;
    
      
    pgbash> chmod +x go.bat ...........パーミッションの設定  
    pgbash> go.bat ...................go.batの実行  
      
      
    
*   **検索結果の値をシェル変数に代入**  
      
    「SELECT INTO ：ホスト変数」文もしくは「FETCH INTO ：ホスト変数」文を使用して、検索結果の値をシェル変数に読み込むことができます。この場合、検索結果の表は端末には表示されません。  
    
    SELECT  列名並び 
     **\[INTO :host\_var1 \[\[INDICATOR\] :ind\_var1\], ...
                :host\_varN \[\[INDICATOR\] :ind\_varN\]\]** 
          ..
    
    FETCH \[オプション\] \[IN\] カーソル名
     **\[INTO :host\_var1 \[\[INDICATOR\] :ind\_var1\], ...
                :host\_varN \[\[INDICATOR\] :ind\_varN\]\];** 
    
    host\_var...... ホスト変数(データ読み込み用シェル変数)
    ind\_var....... インディケータ変数(NULLのとき"1"、以外は"0")
    
      
    (例)  
    
    select col1, col2 into :col1, :col2 from test where code='111';
    select version() into :version;
    select count(\*) into  :count  from test;
    
    begin; 
    declare cur cursor for select \* from test;
    declares -i x ; let x=0;  while ((x < $count))  
    do
       FETCH IN CUR INTO :code INDICATOR :ind\_c,
                         :name:ind\_n, :address :ind\_a;
        if (( SQLCODE == SQL\_OK )); then
            if ((ind\_n != SQL\_NULL || ind\_a != SQL\_NULL)); then
                echo "$code, $name, $address"
                let x=x+1
            fi
        else
            if(( SQLCODE == SQL\_NOT\_FOUND )); then
                    let x=11
            else
                    echo "$SQLERRMC"
                    let x=x+1
            fi
        fi
    done
    end;
    
      
    INTO句を使用する場合は、検索結果が１行でなければなりません。検索結果が複数行になった場合は、シェル変数には代入されずに標準出力に出力されます。  
      
    ここで、SQLCODEは SQLエラーコードを、$SQLERRMCは SQLエラーメッセージを表すシェル変数です。また、SQL\_NULL, SQL\_NOT\_FOUNDは pgbashが初期設定している値です。(エラーコードを参照)  
      
    FETCH INTO に使用するホスト変数(シェル変数）は、**英小文字**を使用した方が無難です。なぜなら、pgbashは先頭が SQL, OPTION, POSTGRESQL などの大文字のシェル変数を使用しているからです。  
      
    \# (( ))演算式のなかでは、整数型シェル変数の先頭'$'は省略することができます。  
      
      
    
*   **検索結果を１ページごと表示**  
      
    Pgbashは、SELECTの検索結果を連続的に表示します。psqlのように１ページごとに表示を停止する表示方法ではありません。そのため、１ページごと表示したい場合は、SQL; のうしろにパイプ('|')と more (もしくは less) が付ける必要があります。(詳細は、Unixのパイプを参照）  
      
    例)  
    pgbash> select \* from test; | more (もしくは less)  
      
    
*   **検索結果をファイルに書き出す**  
      
    Pgbashで、検索結果をファイルに書き出すことは実に簡単です。SQL; のうしろに &> ファイル名　とします。  
      
    例)  
    pgbash> select \* from test; &> /tmp/sel.dat ... "/tmp/sel.dat"ファイルに書き出す  
      
    ファイルが存在していない場合は、> だけでもかまいません。&> は、もしファイルが存在していたらそのファイルを削除して書き直します。(詳細は、Unixのリダイレクションを参照）  
    

* * *

\[[Prev](./usage03-j.md)\] \[[Menu](../usage-j.md)\] \[[Next](./usage05-j.md)\]

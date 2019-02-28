\[[Prev](usage06-j.md)\] \[[Menu](../usage-j.md)\] \[[Next](usage08-j.md)\]

* * *

**Pgbash が使用するシェル変数**  

* * *

  

Pgbashは、先頭の文字列が SQL, OPTION, POSTGRESQL のシェル変数を使用します。いずれも大文字です。ユーザが、独自のシェル変数を使用する場合、英小文字を使用してください。そうすれば、Pgbashが自動的に作成するシェル変数と重なることはありません。  
  
*   **SQLの実行状態を表すシェル変数**  
      
    　Pgbashは、SQL実行後の処理結果を次のシェル変数にセットします。  
      
    
      **$SQLOID **    :(整数型) 最新のinsertのOIDの値
    　**$SQLCODE**    :(整数型) SQLエラーコード (9.エラーコード参照)
                     0    --- 正常終了          
                     100  --- EOF(Eond Of File)  
                     負値 --- SQLエラー         
      **$SQLERRMC**   :(文字型) SQLエラーメッセージ(最大70字)
    　**$SQLERRML**   :(整数型) SQL エラーメッセージの長さ(<70)
      **$SQLERRD2**   :(整数型) 検索結果の行数(PQntuples()の値)
      **$SQLERRD3**   :(整数型) 検索結果の列数(PQnfields()の値)
      **$SQLNTUPLE**  :(整数型) SQLERRD2と同じ
      **$SQLNFIELD**  :(整数型) SQLERRD3と同じ
    
      **${SQLFIELDNAME\[i\]}** : 列名並び(i は 0 から SQLNFIELD-1 まで)
    
      
      
    
*   **オプション用のシェル変数**  
      
    'set option\_name\[=value\]; 'を使用してセットされた値は、オプション用のシェル変数に保管されます。  
      
    
      **$OPTION\_ECHO** : turn on/off ECHO query  
      **$OPTION\_QUIET** : turn on/off QUIET mode  
      **$OPTION\_HEADER** : turn on/off TOP\_header  
      **$OPTION\_BOTTOM** : turn on/off BOTTOM\_print
      **$OPTION\_ALIGNMENT** : turn on/off ALIGNMENT   
      **$OPTION\_FRAME** : turn on/off OUTER\_FRAME 
      **$OPTION\_EXPANDED** : turn on/off EXPANDED    
      **$OPTION\_HTML** : turn on/off HTML mode   
      **$OPTION\_SEPARATOR** : set SEPARATOR character 
      **$OPTION\_NULLSTRING** : set NULL string         
      **$OPTION\_ZEROSTRING** : set NULL string         
      **$OPTION\_CAPTION** : set NULL string         
      **$OPTION\_TABLETAG** : set NULL string         
    
      
      
    
*   **エラーコード用のシェル変数**  
      
    Pgbashは、SQLエラーコードを次のシェル変数にセットしています。  
    対話型環境では、'**??sqlcode**'とするとエラーコードを表示することができます。  
      
    pgbash> ??sqlcode
    
           Value Name        |               Comment                     | Value
    -------------------------+-------------------------------------------+--------
     $SQL\_OK                 | normal end.                               |0
     $SQL\_NOT\_FOUND          | EOF(End Of File).                         |100
     $SQL\_OUT\_OF\_MEMORY      | out of memory.                            |-12
     $SQL\_SYSTEM\_ERROR       | system error.                             |-200
     $SQL\_TOO\_MANY\_ARGUMENTS | too many arguments in fetch\_stmt.         |-201
     $SQL\_TOO\_FEW\_ARGUMENTS  | too few  arguments in fetch\_stmt.         |-202
     $SQL\_CONNECT\_ERROR      | database connection error.                |-203
     $SQL\_INT\_FORMAT         | int format error.                         |-204
     $SQL\_UINT\_FORMAT        | uint format error.                        |-205
     $SQL\_FLOAT\_FORMAT       | float format error.                       |-206
     $SQL\_NUMERIC\_FORMAT     | numeric format error.                     |-207
     $SQL\_INTERVAL\_FORMAT    | interval format error.                    |-208
     $SQL\_DATE\_FORMAT        | date format error.                        |-209
     $SQL\_TIMESTAMP\_FORMAT   | timestamp format error.                   |-210
     $SQL\_CONVERT\_BOOL       | convert bool error.                       |-211
     $SQL\_EMPTY              | empty.                                    |-212
     $SQL\_MISSING\_INDICATOR  | missing\_indicator.                        |-213
     $SQL\_NO\_ARRAY           | no array.                                 |-214
     $SQL\_DATA\_NOT\_ARRAY     | data not array.                           |-215
     $SQL\_ARRAY\_INSERT       | array insert error.                       |-216
     $SQL\_NO\_CONN            | no connection.                            |-220
     $SQL\_NOT\_CONN           |can not connection.                        |-221
     $SQL\_INVALID\_STMT       | invalid statements.                       |-230
     $SQL\_READONLY\_SHELLVAR  | can not set read-only shell variable.     |-231
     $SQL\_DB\_NOT\_OPEN        | database not open.                        |-232
     $SQL\_CNAME\_NOT\_FOUND    | connect-name not found.                   |-233
     $SQL\_CNAME\_ALREADY\_USE  | connect-name already exist.               |-234
     $SQL\_INVALID\_COMMAND    | invalid command.                          |-235
     $SQL\_INVALID\_DATA       | invalid data.                             |-236
     $SQL\_UNKNOWN\_DESCRIPTOR | unknown descriptor.                       |-240
     $SQL\_INVALID\_DESC\_INDEX | invalid descriptor index.                 |-241
     $SQL\_UNKNOWN\_DESC\_ITEM  | unknown descriptor item.                  |-242
     $SQL\_VAR\_NOT\_NUMERIC    | variable not numeric.                     |-243
     $SQL\_VAR\_NOT\_CHAR       | variable not charater.                    |-244
     $SQL\_BAD\_RESPONSE       | bad response(backend maybe died).         |-400
     $SQL\_EMPTY\_QUERY        | empty query (backend lost query).         |-401
     $SQL\_CONNECTION\_BAD     | connection bad(disconnect backend)        |-403
     $SQL\_FATAL\_ERROR        | query fatal error   (SQL error on backend)|-403
     $SQL\_NONFATAL\_ERROR     | query nonfatal error(SQL error on backend)|-404
     $SQL\_NULL               | indicator is NULL.                        |-1
    
      
    　エラーコードは、(( ))演算式を利用してシェルスクリプトの中で次のように使用します。
    
    (例) insert into test values(111,'aaa','bbb');
         if(( SQLCODE < SQL\_OK )); then
            echo $SQLERRMC
         fi
    
    また、SQL実行後に'**?sta**'とすると、エラーコードなどを表示することができます。
    
    (例) pgbash> ?sta
      # Status after executing SQL
      SQLCODE   = 0         (SQL error code)
      SQLNTUPLE = 4         (number of tuples)
      SQLNFIELD = 3         (number of fields)
      SQLERRML  = 0         (length of SQLERRMC)
    
      
    
*   **その他のシェル変数**  
      
    (1) **$SQLNFILED、${SQLFILEDNAME\[i\]}**シェル変数  
    select 結果の列数と列名を保管しています。  
      
    (2) **$SQLOID**シェル変数  
    直前のinsertの OIDの値を保管しています。  
      
    (3) **$POSTGRESQL\_VERSION** シェル変数  
    POSTGRESQL\_VERSIONは、接続したサーバのPostgreSQLバージョン番号を保管しています。  
      
    (4) クッキー用のシェル変数  
    Webアプリケーションにおいて、次のクッキーの値を参照することができます。  
    
     **$HTTP\_NCOOKIE** : クッキーの個数
     **${HTTP\_COOKIEKEY\[i\]}** : クッキーのキー名
     **${HTTP\_COOKIEVAL\[i\]}** : クッキーの値   
     (但し、i は 0 から HTTP\_NCOOKIE-1 までです。)
    

* * *

\[[Prev](./usage06-j.md)\] \[[Menu](../usage-j.md)\] \[[Next](./usage08-j.md)\]

[[Prev](./usage06-j.md)] [[Menu](./readme.md)] [[Next](./usage08-j.md)]

* * *

# Pgbash が使用するシェル変数

PgBashは、先頭の文字列が SQL, OPTION, POSTGRESQL のシェル変数を使用します。いずれも大文字です。ユーザが、独自のシェル変数を使用する場合、英小文字を使用してください。そうすれば、PgBashが自動的に作成するシェル変数と重なることはありません。  
  
## SQLの実行状態を表すシェル変数<a id="STATUS" name="STATUS">&nbsp;</a>

PgBashは、SQL実行後の処理結果を次のシェル変数にセットします。  

<dl>
      <dt>$SQLOID:</dt>
      <dd>(整数型) 最新のinsertのOIDの値</dd>
      <dt>$SQLCODE:</dt>
      <dd>(整数型) SQLエラーコード (9.エラーコード参照)<br />0    --- 正常終了<br />100  --- EOF(Eond Of File)<br />負値 --- SQLエラー</dd>
      <dt>$SQLERRMC:</dt>
      <dd>(文字型) SQLエラーメッセージ(最大70字)</dd>
      <dt>$SQLERRML:</dt>
      <dd>(整数型) SQL エラーメッセージの長さ(&lt;70)</dd>
      <dt>$SQLERRD2:</dt>
      <dd>(整数型) 検索結果の行数(PQntuples()の値)</dd>
      <dt>$SQLERRD3:</dt>
      <dd>(整数型) 検索結果の列数(PQnfields()の値)</dd>
      <dt>$SQLNTUPLE:</dt>
      <dd>(整数型) SQLERRD2と同じ</dd>
      <dt>$SQLNFIELD:</dt>
      <dd>(整数型) SQLERRD3と同じ</dd>
      <dt>${SQLFIELDNAME[i]}:</dt>
      <dd>列名並び(i は 0 から SQLNFIELD-1 まで)</dd>
</dl>

## オプション用のシェル変数<a id="VAR" name="VAR">&nbsp;</a>

`'set option_name[=value];'` を使用してセットされた値は、オプション用のシェル変数に保管されます。  

<dl>
      <dt>$OPTION_ECHO:</dt>
      <dd>turn on/off ECHO query</dd>
      <dt>$OPTION_QUIET:</dt>
      <dd>turn on/off QUIET mode</dd>
      <dt>$OPTION_HEADER:</dt>
      <dd>turn onoff TOP_header</dd>
      <dt>$OPTION_BOTTOM:</dt>
      <dd>turn on/off BOTTOM_print</dd>
      <dt>$OPTION_ALIGNMENT:</dt>
      <dd>turn on/off ALIGNMENT</dd>
      <dt>$OPTION_FRAME:</dt>
      <dd>turn on/off OUTER_FRAME</dd>
      <dt>$OPTION_EXPANDED:</dt>
      <dd>turn on/off EXPANDED</dd>
      <dt>$OPTION_HTML:</dt>
      <dd>turn on/off HTML mode</dd>
      <dt>$OPTION_SEPARATOR:</dt>
      <dd>set SEPARATOR character</dd>
      <dt>$OPTION_NULLSTRING:</dt>
      <dd>set NULL string</dd>
      <dt>$OPTION_ZEROSTRING:</dt>
      <dd>set NULL string</dd>
      <dt>$OPTION_CAPTION:</dt>
      <dd>set NULL string</dd>
      <dt>$OPTION_TABLETAG:</dt>
      <dd>set NULL string</dd>
</dl>

## エラーコード用のシェル変数<a id="ERRCODE" name="ERRCODE">&nbsp;</a>

PgBashは、SQLエラーコードを次のシェル変数にセットしています。  
対話型環境では、'**??sqlcode**'とするとエラーコードを表示することができます。  

    pgbash> ??sqlcode
    
           Value Name        |               Comment                     | Value
    -------------------------+-------------------------------------------+--------
     $SQL_OK                 | normal end.                               |0
     $SQL_NOT_FOUND          | EOF(End Of File).                         |100
     $SQL_OUT_OF_MEMORY      | out of memory.                            |-12
     $SQL_SYSTEM_ERROR       | system error.                             |-200
     $SQL_TOO_MANY_ARGUMENTS | too many arguments in fetch_stmt.         |-201
     $SQL_TOO_FEW_ARGUMENTS  | too few  arguments in fetch_stmt.         |-202
     $SQL_CONNECT_ERROR      | database connection error.                |-203
     $SQL_INT_FORMAT         | int format error.                         |-204
     $SQL_UINT_FORMAT        | uint format error.                        |-205
     $SQL_FLOAT_FORMAT       | float format error.                       |-206
     $SQL_NUMERIC_FORMAT     | numeric format error.                     |-207
     $SQL_INTERVAL_FORMAT    | interval format error.                    |-208
     $SQL_DATE_FORMAT        | date format error.                        |-209
     $SQL_TIMESTAMP_FORMAT   | timestamp format error.                   |-210
     $SQL_CONVERT_BOOL       | convert bool error.                       |-211
     $SQL_EMPTY              | empty.                                    |-212
     $SQL_MISSING_INDICATOR  | missing_indicator.                        |-213
     $SQL_NO_ARRAY           | no array.                                 |-214
     $SQL_DATA_NOT_ARRAY     | data not array.                           |-215
     $SQL_ARRAY_INSERT       | array insert error.                       |-216
     $SQL_NO_CONN            | no connection.                            |-220
     $SQL_NOT_CONN           |can not connection.                        |-221
     $SQL_INVALID_STMT       | invalid statements.                       |-230
     $SQL_READONLY_SHELLVAR  | can not set read-only shell variable.     |-231
     $SQL_DB_NOT_OPEN        | database not open.                        |-232
     $SQL_CNAME_NOT_FOUND    | connect-name not found.                   |-233
     $SQL_CNAME_ALREADY_USE  | connect-name already exist.               |-234
     $SQL_INVALID_COMMAND    | invalid command.                          |-235
     $SQL_INVALID_DATA       | invalid data.                             |-236
     $SQL_UNKNOWN_DESCRIPTOR | unknown descriptor.                       |-240
     $SQL_INVALID_DESC_INDEX | invalid descriptor index.                 |-241
     $SQL_UNKNOWN_DESC_ITEM  | unknown descriptor item.                  |-242
     $SQL_VAR_NOT_NUMERIC    | variable not numeric.                     |-243
     $SQL_VAR_NOT_CHAR       | variable not charater.                    |-244
     $SQL_BAD_RESPONSE       | bad response(backend maybe died).         |-400
     $SQL_EMPTY_QUERY        | empty query (backend lost query).         |-401
     $SQL_CONNECTION_BAD     | connection bad(disconnect backend)        |-403
     $SQL_FATAL_ERROR        | query fatal error   (SQL error on backend)|-403
     $SQL_NONFATAL_ERROR     | query nonfatal error(SQL error on backend)|-404
     $SQL_NULL               | indicator is NULL.                        |-1

エラーコードは、(( ))演算式を利用してシェルスクリプトの中で次のように使用します。

(例) 
      insert into test values(111,'aaa','bbb');
      if(( SQLCODE < SQL_OK )); then
            echo $SQLERRMC
      fi

また、SQL実行後に'**?sta**'とすると、エラーコードなどを表示することができます。

(例) 
      pgbash> ?sta
      # Status after executing SQL
      SQLCODE   = 0         (SQL error code)
      SQLNTUPLE = 4         (number of tuples)
      SQLNFIELD = 3         (number of fields)
      SQLERRML  = 0         (length of SQLERRMC)

## その他のシェル変数<a id="ETC" name="ETC">&nbsp;</a>

### (1) $SQLNFILED、${SQLFILEDNAME[i]} シェル変数  
select 結果の列数と列名を保管しています。  

### (2) $SQLOID シェル変数  
直前のinsertの OIDの値を保管しています。  

### (3) $POSTGRESQL_VERSION シェル変数  
POSTGRESQL_VERSIONは、接続したサーバのPostgreSQLバージョン番号を保管しています。  

### (4) クッキー用のシェル変数  
Webアプリケーションにおいて、次のクッキーの値を参照することができます。  

<dl>
    <dt>$HTTP_NCOOKIE:</dt>
    <dd>クッキーの個数</dd>
    <dt>${HTTP_COOKIEKEY[i]}:</dt>
    <dd>クッキーのキー名</dd>
    <dt>${HTTP_COOKIEVAL[i]}:</dt>
    <dd>クッキーの値<br />(但し、i は 0 から HTTP_NCOOKIE-1 までです。)</dd>
</dl>

* * *

[[Prev](./usage06-j.md)] [[Menu](./readme.md)] [[Next](./usage08-j.md)]

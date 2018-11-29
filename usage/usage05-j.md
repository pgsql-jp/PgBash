\[[Prev](usage04-j.md)\]

\[[Menu](../usage-j.md)\]

\[[Next](usage06-j.md)\]

* * *

**SQLの実行オプション**  

* * *

  

*   **オプションの指定方法**  
      
    オプションには、set文を使用した**定常的なオプション**と、exec\_sqlコマンドを使用した**一時的なオプション**の２種類があります。定常的なオプションは、それが変更されるまで、実行される全てのSQLに対して有効です。一時的なオプションは、exec\_sqlコマンドで指定されたSQLのみ有効になります。  
      
    
*   **set文による定常的なオプション**  
      
    定常的なオプションは次のように指定します。但し大文字小文字の区別はありません。  
      
    **set EXEC\_SQL\_OPTION DEFAULT;  
    set EXEC\_SQL\_OPTION CGI;  
    set option\_name\[=value\];  
    **  
    (1) DEFAULT  
    'set EXEC\_SQL\_OPTION DEFAULT;'は、全てのオプションをデフォルトに戻します。  
      
    (2) CGI  
    'set EXEC\_SQL\_OPTION CGI;'は、シェルスクリプトをCGIモードにします。  
      
    (3) その他のオプション  
    その他のオプションでは、「ON（または TRUE)」、「OFF（または FALSE)」もしくは「オプションの値」をセットします。  
    尚、ON/OFFの省略した場合は ON がセットされたことになります。例えば、set OPTION\_ECHO; は set OPTION\_ECHO=ON; と同じです。  
      
    例)  
    set OPTION\_ECHO; または set OPTION\_ECHO=ON; または set OPTION\_ECHO=TRUE;  
    set OPTION\_QUIET=OFF; または set OPTION\_QUIET=FALSE;  
    set OPTION\_SEPARATOR=',';  
    set OPTION\_NULLSTRING="\*N\*";  
      
    
    option\_name
    
    内容
    
    DEFAULT  
    value
    
    OPTION\_ECHO
    
    バックエンドに送信されるクエリを表示
    
    OFF
    
    OPTION\_QUIET
    
    エラーメッセージを表示しない
    
    OFF  
    (表示する)
    
    OPTION\_HEADER
    
    表のヘッダー部を表示  
    
    例) set option\_header=on;
        select \* from test;
    
        code|name   |addr
        ----+-------+-----
         101|tanaka |tokyo
         111|yoshida|fukui
        (2 rows)
    
    例) set option\_header=off;
        select \* from test;
    
         101|tanaka |tokyo
         111|yoshida|fukui
        (2 rows)
    
    ON
    
    OPTION\_BOTTOM
    
    表の最下段(行数)を表示  
    
    例) set option\_bottom=off;
        select \* from test;
    
        code|name   |addr
        ----+-------+-----
         101|tanaka |tokyo
         111|yoshida|fukui
    
    ON
    
    OPTION\_ALIGNMENT
    
    表の列の並びを揃える  
      
    CSV形式で出力する場合は OFF にする。  
    OPTION\_SEPARATORを参照。
    
    ON
    
    OPTION\_FRAME
    
    表の外側の罫線を表示  
    
    例) set option\_frame=on;
        select \* from test;
    
        +------+---------+-------+
        | code | name    | addr  |
        +------+---------+-------+
        |  101 | tanaka  | tokyo |
        |  111 | yoshida | fukui |
        +------+---------+-------+
        (2 rows)
    
    OFF
    
    OPTION\_EXPANDED
    
    検索結果の出力を拡張(expanded)表示形式に切り替える  
    
    例) set option\_expanded=on;
        select \* from test;
    
        ----- RECORD 0 -----
        code｜101   
        name｜tanaka
        addr｜tokyo
        ----- RECORD 1 -----
        code｜111
        name｜yoshida
        addr｜fukui
    
    OFF
    
    OPTION\_SEPARATOR
    
    表の区切りの罫線文字を指定  
    
    例) set option\_expanded=on;
        set option\_separator='**：**'; 
        select \* from test;
    
        ----- RECORD 0 -----
        code**:**101
        name**:**tanaka
        addr**:**tokyo
        ----- RECORD 1 -----
        code**:**111
        name**:**yoshida
        addr**:**fukui
      
    例) set option\_header=off;
        set option\_bottom=off;
        set option\_expanded=off;
        set option\_alignment=off;
        set option\_separator=',';
        select \* from test;
    
        \[CSV形式の出力\]
        101,tanaka,tokyo
        111,yoshida,fukui
    
      |
    
    OPTION\_NULLSTRING  
    OPTION\_ZEROSTRING
    
    検索結果の値が NULL もしくは all bit off の文字を指定  
    
    例) set option\_nullstring='\\N';
        set option\_zerostring='\\0';
        select \* from test;
    
        code|name   |addr
        ----+-------+-----
         101|tanaka |tokyo
         110|\\N     |\\0
         210|sakaida|osaka
    
    例) set option\_nullstring='';
        set option\_zerostring=''
        select \* from test;
    
        code|name   |addr
        ----+-------+-----
         101|tanaka |tokyo
         110|       |  
         210|sakaida|osaka
    
      
    
    OPTION\_CAPTION
    
    表のタイトル文字列を指定
    
      
    
    OPTION\_HTML
    
    HTML出力を行う  
      
    set EXEC\_SQL\_OPTION CGI; を実行すると自動的に HTML出力モード(set OPTION\_HTML=ON;)になる
    
    OFF
    
    OPTION\_TABLETAG
    
    HTMLのTABLEタグのオプションを指定
    
    例) set OPTION\_TABLETAG='
        <table bgcolor=#eeeeee%>';
    
      
    
    OPTION\_HEADERTR
    
    TABLEヘッダーのTRタグを指定（主にヘッダーの１行全列の背景色を変更する場合に使用)  
    
    例) set OPTION\_HEADERTR='
        <TR BGCOLOR=#92CDCD>';
    
      
    
    OPTION\_HEADERTH
    
    TABLEヘッダーのTHタグを、各列の数だけカンマ区切りで指定（列ごとの長さ色を変える場合などに使用）  
    
    例) set OPTION\_HEADERTH='
        <TH bgcolor=#0088FF>,
        <TH nowrap>,
        <TH>';
    
    OPTION\_HEADERTHで指定したタグ数が、検索結果の列数に満たないときは、残りの列は、<TH>で満たされる。
    
      
    
    OPTION\_BODYTAG
    
    TABLEボディのTDタグを、各列の数だけカンマ区切りで指定(列ごとの長さ色を変える場合に使用）  
    
    例) set OPTION\_BODYTAG='
        <TD bgcolor=#0088FF>,
        <TD nowrap>,
        <TD>';
    
    OPTION\_BODYTAGで指定したタグ数が、検索結果の列数に満たないときは、残りの列は、<TD>で満たされる。
    
      
    
    OPTION\_INPUTTAG
    
    更新可能なフィールド形式に変更して検索結果を表示  
    (下記の例2を参照）
    
    OFF
    
    OPTION\_INPUTSIZE
    
    OPTION\_INPUTTAGがONの場合、各列の長さをカンマ区切りで指定  
    
    例) set option\_inputsize='-1,32,48,0';
    
    長さを0にすると自動決定し、長さを -1 にすると入力不可になる。  
    OPTION\_INPUTSIZEで指定した長さの値の数が、検索結果の列数に満たないときは、残りは全て自動決定になる。
    
      
    
      
    （例1)  
    set OPTION\_CAPTION='<P align=left><FONT SIZE=5><B>会員名簿</B></FONT></P>';  
    set OPTION\_TABLETAG='<table bgcolor=#D1EEEE border=4 cellspacing=0 cellpadding=2>';  
    set OPTION\_HEADERTR='<tr bgcolor=#96CDCD>';  
    set OPTION\_HEADERTH='<TH bgcolor=#0088FF nowrap>,<TH nowrap>,<TH nowrap>';  
    set OPTION\_BODYTAG='<TD bgcolor=#0088FF nowrap>,<TD nowrap>,<TD nowrap>';  
    select \* from member where userid>0 order by userid;  
    
    **会員名簿**
    
    userid
    
    氏名
    
    email
    
    電話
    
    1220
    
    青xx木正志
    
    XXXYYY62@nify.ne.jp
    
    0777-66-xxxx
    
    1249
    
    安yy孝則
    
    XXXYYYc@email.com
    
    090-xxxx-yyyy
    
    (**2** rows)  
      
    (例2)  
    set OPTION\_CAPTION='<P align=left><FONT SIZE=5><B>会員名簿</B></FONT></P>';  
    set OPTION\_TABLETAG='<table border=1 bgcolor=#D1EEEE cellspacing=1 cellpadding=2>';  
    set OPTION\_HEADER=OFF;  
    set OPTION\_BOTTOM=OFF;  
    set OPTION\_EXPANDED=ON;  
    set OPTION\_INPUTTAG=ON;  
    set OPTION\_INPUTSIZE='-1, 32, 48, 0';  
    select \* from member where userid=1258;  
    
    **会員名簿**
    
    **userid**
    
    1258
    
    **氏名**
    
    **email**
    
    **電話**
    
      
    

  
  
*   **exec\_sqlコマンドによる一時的なオプション**  
      
    
    一時的にオプションを指定してSQLを実行するには、exec\_sqlコマンドを使用します。  
      
    **exec\_sql オプション "SQL"**  
      
    
    オプション
    
    内容
    
    set文のオプションとの比較
    
    \-e
    
    バックエンドに送信されるクエリを表示
    
    set OPTION\_ECHO=ON
    
    \-q
    
    エラーメッセージを表示しない
    
    set OPTION\_QUIET=ON
    
    \-T
    
    表のヘッダー部を表示しない  
    
    set OPTION\_HEADER=OFF
    
    \-B
    
    表の最下段(行数)を表示しない  
    
    set OPTION\_BOTTOM=OFF
    
    \-A
    
    表の罫線を揃えない
    
    set OPTION\_ALIGNMENT=OFF
    
    \-L
    
    表の外側の罫線を表示
    
    set OPTION\_FRAME=ON
    
    \-X
    
    拡張表示形式に切り替える
    
    set OPTION\_EXPANDED=ON
    
    \-S sep
    
    表の区切りの罫線文字を指定
    
    set OPTION\_SEPARATOR='sep'
    
    \-N null
    
    検索結果の値がNULLのときの文字を指定
    
    set OPTION\_NULLSTRING='null'
    
    \-Z zero
    
    検索結果の値が all bit off の文字を指定
    
    set OPTION\_ZEROSTRING='zero'
    
    \-C cap
    
    表のタイトル文字列を指定
    
    set OPTION\_CAPTION='cap'
    
    \-H
    
    HTML出力を行う
    
    set OPTION\_HTML=ON
    
    (例-3) 検索結果をCSV形式でファイルに書き出す
    
    exec\_sql **\-S ',' -TBA** "select \* from test where code >= 111"
    
    111,222,333  
    44,5555,66666  
    5555,88,9999  
    
    * * *
    
    \[[Prev](./usage04-j.md)\]
    
    \[[Menu](../usage-j.md)\]
    
    \[[Next](./usage06-j.md)\]

\[[Prev](usage02-j.md)\]

\[[Menu](../usage-j.md)\]

\[[Next](usage04-j.md)\]

* * *

**データベースの接続と切断**  

* * *

  

*   **データベースの接続**  
      
    データベースに接続するには、CONNECT文を使用します。CONNECTを実行せずに　SELECTなどのSQLを実行した場合は、自動的に"**CONNECT TO DEFAULT**"が発行されます。  
      
    DEFAULTは、"**Unixログイン名**" をデータベース名およびデータベースユーザ名として使用します。  
      
    **CONNECT TO dbname\[@server\[:port\]\] | DEFAULT \[AS connect\_name\]  
                   \[USER user\_name \[\[{IDENTIFIED BY | USING | /}\] password\]\];  
    **  
    
    dbname..........データベース名
    server..........サーバ名 (例えば、db.xxx.co.jp)
    port............PostgreSQLのポート番号(指定がなければ 5432)
    connect\_name....コネクション名(指定がなければdbnameと同じになる)
    user\_name.......データベースユーザ名（指定がなければUnixユーザ名になる）
    password........パスワード  
    
    使用例) 
    connect to postgres@db1.jp as db1 user postgres xxxxxxxx;
    connect to postgres@db2.jp as db2 user postgres IDENITIFIED BY xxxxxxxx;
    connect to postgres@db3.jp as db3 user postgres USING xxxxxxxx;
    connect to postgres@db4.jp as db4 user postgres/xxxxxxxx;
    
    ・password を指定しない場合は、**password: プロンプト**が表示されパスワード入力待ち状態になります。  
      
    ・パスワードを設定していないユーザは、"CONNECT TO データベース名 USER ユーザ名 NULL"のようにパスワードに"**NULL**"を指定することができます。  
      
    (パスワードが設定されていない場合の例)
    
    connect to postgres\_db user xxx;  
    password: ................. リターンだけを入力する  
      
    connect to postgres\_db user xxx NULL;  
    ... この場合、password: プロンプトは表示されない。  
    
      
    ・Unixのログインユーザ名と同じ名前のユーザ名の場合は、"user"以降を省略することができます。  
      
    ・コネクション名を指定しない場合はデータベースがコネクション名として使用されます。  
    例)  
    
    connect to postgres3 user admin; ..............コネクション名は postgres3 になる  
    
      
    ・同じデータベースに対して違う接続名で接続することができます  
    例)  
    
    connect to postgres\_db as db1;  
    connect to postgres\_db as db2;  
    
      
      
    
*   **データベースの切断**  
      
    disconnect によって、コネクションを切断することができます。disconnect **all**; で全てのコネクションを切断することができます。  
      
    **DISCONNECT connect\_name|CURRENT|DEFAULT|ALL;  
    **
    
    connect\_name.....コネクション名
    CURRENT..........カレントのコネクション
    DEFAULT..........CONNECT TO DEFAULT で接続したコネクション
    ALL..............全てのコネクション
    
    例)
    disconnect db4; ...............................コネクションdb4を切断する
    disconenct all; ...............................全てのコネクションを切断する
    
    対話型環境では Pgbashを終了した時点で、シェルスクリプトではシェルスクリプトが終了した時点で、自動的に全てのコネクションは切断されます。従いまして、disconnect は実行しなくてもかまいません。  
      
      
    
*   **現在(カレント)の接続(コネクション)の切り替え**  
      
    複数のデータベースに接続した場合、set connection 文を使用して「現在(カレント)の接続名」を切り替えることができます。以後、そのデータベース接続に対してSQLが実行されることになります。  
      
    **SET CONNECTION connect\_name|DEFAULT;  
    **
    
    connect\_name.....コネクション名
    DEFAULT..........CONNECT TO DEFAULT で接続したコネクション
    
    例)
    connect to postgres1 as db1;
    connect to postgres2 as db2;
    connect to postgres1 as db3;..............db3 がカレントの接続名になる
    set connection db1;...................... db1をカレントの接続名に変更
    exec\_sql -d db2 "select \* from test2".... 一時的にdb2へ接続
    exec\_sql        "select \* from test1".... カレントのdb1の接続を使用
    exec\_sql -d db3 "select \* from test2".... 一時的にdb3へ接続
    select \* from test; ..................... カレントのdb1の接続を使用
    disconnect all;
    
    また、exec\_sql の -d オプションによって、一時的にデータベース接続を切り替えることができます。  
      
      
    
*   **データベースの接続状態の表示**  
      
    対話型環境では、?dbc 、シェルスクリプトでは "exec\_sql -m" を指定します。  
      
    例えば次のように３つの接続がある場合は次のように表示されます。
    
    connect to default;
    connect to postgres@www2.psn.ne.jp:5432  as db1;
    connect to admin@xxx.psn.ne.jp as db2 user admin;
    
    **?dbc**（もしくは **exec\_sql -mL**)
    
    # List of database connection (C: current database is '\*')
    +---+--------------+-----------+------------------------------+
    | C | connect\_name | user\_name | target\_name                  |
    +---+--------------+-----------+------------------------------+
    |   | \_DEFAULT\_    | postgres  | postgres:5432                |
    |   | db1          | postgres  | postgres@www.psn.ne.jp:5432  |
    | \* | db2          | admin     | admin@xxx.psn.ne.jp:5432     |
    +---+--------------+-----------+------------------------------+
    (3 rows)
    

* * *

\[[Prev](./usage02-j.md)\]

\[[Menu](../usage-j.md)\]

\[[Next](./usage04-j.md)\]

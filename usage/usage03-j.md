[[Prev](./usage02-j.md)] [[Menu](./readme.md)] [[Next](./usage04-j.md)]

* * *

# データベースの接続と切断

## データベースの接続<a id="CONNECT" name="CONNECT">&nbsp;</a>

データベースに接続するには、CONNECT文を使用します。CONNECTを実行せずに　SELECTなどのSQLを実行した場合は、自動的に"**CONNECT TO DEFAULT**"が発行されます。  

DEFAULTは、"**Unixログイン名**" をデータベース名およびデータベースユーザ名として使用します。  

**CONNECT TO dbname\[@server\[:port\]\] | DEFAULT \[AS connect\_name\] \[USER user\_name \[\[{IDENTIFIED BY | USING | /}\] password\]\];**  

<dl>
    <dt>dbname</dt>
    <dd>データベース名</dd>
    <dt>server</dt>
    <dd>サーバ名 (例えば、db.example.jp)</dd>
    <dt>port</dt>
    <dd>PostgreSQLのポート番号(指定がなければ 5432)</dd>
    <dt>connect_name</dt>
    <dd>コネクション名(指定がなければdbnameと同じになる)</dd>
    <dt>user_name</dt>
    <dd>データベースユーザ名（指定がなければUnixユーザ名になる）</dd>
    <dt>password</dt>
    <dd>パスワード</dd>
</dl>

使用例)  

        connect to postgres@db1.jp as db1 user postgres xxxxxxxx;
        connect to postgres@db2.jp as db2 user postgres IDENITIFIED BY xxxxxxxx;
        connect to postgres@db3.jp as db3 user postgres USING xxxxxxxx;
        connect to postgres@db4.jp as db4 user postgres/xxxxxxxx;

+ password を指定しない場合は、**password: プロンプト**が表示されパスワード入力待ち状態になります。  

+ パスワードを設定していないユーザは、"CONNECT TO データベース名 USER ユーザ名 NULL"のようにパスワードに"**NULL**"を指定することができます。  

(パスワードが設定されていない場合の例)

        connect to postgres_db user xxx;  
        password: ................. リターンだけを入力する  

        connect to postgres_db user xxx NULL;  
        ... この場合、password: プロンプトは表示されない。  

+ Unixのログインユーザ名と同じ名前のユーザ名の場合は、"user"以降を省略することができます。  

+ コネクション名を指定しない場合はデータベースがコネクション名として使用されます。  

例)  

        connect to postgres3 user admin; ..............コネクション名は postgres3 になる  

+ 同じデータベースに対して違う接続名で接続することができます  

例)  

        connect to postgres_db as db1;  
        connect to postgres_db as db2;  

## データベースの切断<a id="DISCONNECT" name="DISCONNECT">&nbsp;</a>

disconnect によって、コネクションを切断することができます。  
disconnect **all**; で全てのコネクションを切断することができます。  

**DISCONNECT connect_name|CURRENT|DEFAULT|ALL;**

<dl>
    <dt>connect_name</dt>
    <dd>コネクション名</dd>
    <dt>CURRENT</dt>
    <dd>カレントのコネクション</dd>
    <dt>DEFAULT</dt>
    <dd>CONNECT TO DEFAULT で接続したコネクション</dd>
    <dt>ALL</dt>
    <dd>全てのコネクション</dd>
</dl>

例)

        disconnect db4; ...............................コネクションdb4を切断する
        disconenct all; ...............................全てのコネクションを切断する

対話型環境では PgBashを終了した時点で、シェルスクリプトではシェルスクリプトが終了した時点で、自動的に全てのコネクションは切断されます。従いまして、disconnect は実行しなくてもかまいません。  

## 現在(カレント)の接続(コネクション)の切り替え<a id="SETCONNECT" name="SETCONNECT">&nbsp;</a>

複数のデータベースに接続した場合、set connection 文を使用して「現在(カレント)の接続名」を切り替えることができます。以後、そのデータベース接続に対してSQLが実行されることになります。  

**SET CONNECTION connect_name|DEFAULT;**

<dl>
    <dt>connect_name</dt>
    <dd>コネクション名</dd>
    <dt>DEFAULT</dt>
    <dd>CONNECT TO DEFAULT で接続したコネクション</dd>
</dl>

例)

    connect to postgres1 as db1;
    connect to postgres2 as db2;
    connect to postgres1 as db3;..............db3 がカレントの接続名になる
    set connection db1;...................... db1をカレントの接続名に変更
    exec_sql -d db2 "select \* from test2".... 一時的にdb2へ接続
    exec_sql        "select \* from test1".... カレントのdb1の接続を使用
    exec_sql -d db3 "select \* from test2".... 一時的にdb3へ接続
    select * from test; ..................... カレントのdb1の接続を使用
    disconnect all;

また、exec_sql の -d オプションによって、一時的にデータベース接続を切り替えることができます。  

## データベースの接続状態の表示<a id="STATUS" name="STATUS">&nbsp;</a>

対話型環境では、?dbc 、シェルスクリプトでは "exec_sql -m" を指定します。  

例えば次のように３つの接続がある場合は次のように表示されます。

    connect to default;
    connect to postgres@www2.psn.ne.jp:5432  as db1;
    connect to admin@xxx.psn.ne.jp as db2 user admin;

    ?dbc（もしくは exec_sql -mL)

    # List of database connection (C: current database is '*')
    +---+--------------+-----------+------------------------------+
    | C | connect_name | user_name | target_name                  |
    +---+--------------+-----------+------------------------------+
    |   | _DEFAULT_    | postgres  | postgres:5432                |
    |   | db1          | postgres  | postgres@www.psn.ne.jp:5432  |
    | * | db2          | admin     | admin@xxx.psn.ne.jp:5432     |
    +---+--------------+-----------+------------------------------+
    (3 rows)

* * *

[[Prev](./usage02-j.md)] [[Menu](./readme.md)] [[Next](./usage04-j.md)]

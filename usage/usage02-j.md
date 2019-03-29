\[[Prev](usage01-j.md)\] \[[Menu](../usage-j.md)\] \[[Next](usage03-j.md)\]

* * *

# SQLの指定方法  

* * *

## SQLの記述方法

### 1. 記述方法
**\[timespec\] SQL; \[pipeline\]\[redirection\]\[&\]**

もしくは

**\[timespec\] exec\_sql \[option\] "SQL" \[pipeline\]\[redirection\]\[&\]**  

対話型環境およびシェルスクリプトのいずれも、SQL文をそのまま入力(記述)するだけでSQLを実行することができます。 SQL文は、timespec、パイプ、リダイレクション、バックグランドジョブの指定と伴に実行することができます。また、exec\_sqlは、一時的なオプションを付けて、SQLを実行することができます。

SQL; は pgbash内部で exec\_sql "SQL"に変換されて実行されます。従いまして、 SQL; と exec\_sql "SQL" とは全く同じ意味になります。

### 2. SQLの終端子

SQL文を直接記述する場合は、SQLの終端子（セミコロン）は必須です。しかし、exec\_sql "SQL"と記述した場合、SQLの終端子は有っても無くてもかまいません。

### 3. SQL文の途中の改行

SQL文は**単語の切れ目で自由に改行**することができます。対話型環境では、改行すると > プロンプトが表示されますので続けてSQL文を入力します。セミコロンが現れた時点でSQLの終了とみなします。

(対話型環境の例)

    pgbash> insert into test values(111,  
    > 'aaa','bbb'  
    > );

(シェルスクリプトの例)

    insert into test values(111,
    'aaa','bbb'
    );

### 4. シェル変数の利用
SQL のなかで、シェル変数を利用することができます。シェル変数は、値を参照する場合は頭に $ マークを付けます。  

例)

    _aa='123'
    _bb='name'
    _cc='address'
    insert into test values($_aa, $_bb, $_cc);
    COL1='氏名'
    VAL1='鈴木一郎'
    select $COL1, $COL2 from test where $COL1='$VAL1';
    SQL="select * from test"
    exec_sql "$SQL"

シングルクォート文字データが有る場合、**'** で囲む必要があります。  

例)

    DATA="name'123"
    select * from test where name='$DATA';


## SQLの実行方法

### 1. 対話型環境における実行

対話型環境においてSQLを入力すると、即座に実行されます。SELECTなどの検索命令は、検索結果が即座に画面に表示されます。  

例)

    select * from test; | more .................. パイプの例
    select * from test; &> /tmp/xx.dat .......... リダイレクションの例
    select * from test; & ....................... バックグランドジョブの例
    time select * from test; .................... timespecの例
    select * from test; &> /tmp/xx.dat & ........ リダイレクションとバックグランドジョブ
    exec_sql "select * from test" | more
    exec_sql "select * from test"  &> /tmp/xx.dat 
        
ここで、"| more" は、表示行数が多い場合に、画面のサイズに合わせて表示を一旦停止する指定です。また、timespec は、処理時間を表示する場合に使用します。"&>"は、既にファイルが存在していても強制的に上書でファイルを作成します。  

### 2. シェルスクリプトにおける実行

上記の対話型環境と同様の処理を行なうには、次のようにします。  

例)

    シェルスクリプト"sel.bat"
        
    #!/usr/local/bin/pgbash
    connect to postgres user postgres;
    select * from test;
    disconnect all;  ...... disconnect は無くてもかまいません

    pgbash> sel.bat | more .................. パイプの例
    pgbash> sel.bat &> /tmp/xx.dat .......... リダイレクションの例
    pgbash> sel.bat & ....................... バックグランドジョブの例
    pgbash> time sel.bat .................... timespecの例
    pgbash> sel.bat &> /tmp/xx.dat & ........ リダイレクションとバックグランドジョブ

### 3. 検索結果の表示の中断

検索結果が画面に表示されている途中に、その表示をとめたい場合は、**Ctrl+C** を押します。但し、Ctrl+C を押しても、画面表示はすぐには止まりません。数十行は表示されてしまいます。  

### 4. メモリオーバー

検索結果は、メモリに一時的に保管されますので、あまりに多くの検索結果を表示すると、メモリオーバ−のエラーになる可能性があります。大量の結果を表示する場合は、limit をつけてください。  

例）  

    pgbash> select * from test limit 500; | more ............. 最大500行表示  
    pgbash> select * from test limit 500 offset 100; | more .. 100行目から最大500行表示  

## SQLの指定方法の制限

### 1. SQLの位置

SQL; は timespec や 空白 を除いて、行の先頭から記述しなければなりません。if文、while文の後ろに記述することはできません。  
  
間違いの例

    if SQL * from test;
    then
    ...
    fi

正しい例

    select * from test;
    if (( SQLCODE == 0 )); then
        ...
    fi

SQLの実行状態を知りたい場合は、SQLCODE シェル変数を利用します。  

### 2. 予約語

Pgbashは、次の予約語がコマンド並びの先頭もしくは timespecの直後にあった場合、それを SQL文だと判断します。  

(PostgreSQL-8.1の予約語の例)  

    ABORT                     CREATE LANGUAGE           DROP VIEW
    ALTER AGGREGATE           CREATE OPERATOR CLASS     END
    ALTER CONVERSION          CREATE OPERATOR           EXECUTE
    ALTER DATABASE            CREATE ROLE               EXPLAIN
    ALTER DOMAIN              CREATE RULE               FETCH
    ALTER FUNCTION            CREATE SCHEMA             GRANT
    ALTER GROUP               CREATE SEQUENCE           INSERT
    ALTER INDEX               CREATE TABLE              LISTEN
    ALTER LANGUAGE            CREATE TABLE AS           LOAD
    ALTER OPERATOR CLASS      CREATE TABLESPACE         LOCK
    ALTER OPERATOR            CREATE TRIGGER            MOVE
    ALTER ROLE                CREATE TYPE               NOTIFY
    ALTER SCHEMA              CREATE USER               PREPARE
    ALTER SEQUENCE            CREATE VIEW               PREPARE TRANSACTION
    ALTER TABLE               DEALLOCATE                REINDEX
    ALTER TABLESPACE          DECLARE                   RELEASE SAVEPOINT
    ALTER TRIGGER             DELETE                    RESET
    ALTER TYPE                DROP AGGREGATE            REVOKE
    ALTER USER                DROP CAST                 ROLLBACK
    ANALYZE                   DROP CONVERSION           ROLLBACK PREPARED
    BEGIN                     DROP DATABASE             ROLLBACK TO SAVEPOINT
    CHECKPOINT                DROP DOMAIN               SAVEPOINT
    CLOSE                     DROP FUNCTION             SELECT
    CLUSTER                   DROP GROUP                SELECT INTO
    COMMENT                   DROP INDEX                SET
    COMMIT                    DROP LANGUAGE             SET CONSTRAINTS
    COMMIT PREPARED           DROP OPERATOR CLASS       SET ROLE
    COPY                      DROP OPERATOR             SET SESSION AUTHORIZATION
    CREATE AGGREGATE          DROP ROLE                 SET TRANSACTION
    CREATE CAST               DROP RULE                 SHOW
    CREATE CONSTRAINT TRIGGER DROP SCHEMA               START TRANSACTION
    CREATE CONVERSION         DROP SEQUENCE             TRUNCATE
    CREATE DATABASE           DROP TABLE                UNLISTEN
    CREATE DOMAIN             DROP TABLESPACE           UPDATE
    CREATE FUNCTION           DROP TRIGGER              VACUUM
    CREATE GROUP              DROP TYPE
    CREATE INDEX              DROP USER

PgbashがSQL文だと判断した場合、**セミコロンが現れるまで入力を要求**します。  

### 3. SQL入力の中断

SQL入力を途中で中断したい場合は、次のようにしてください。  

a. 1行目の途中であれば、**Ctrl+C**  
b. 2行目以降の途中であれば、__Ctrl+D__

もしくは、セミコロンを入力してSQLを実行させシンタックスエラー終了させます。  

### 4. SQL予約語とプログラム名が重なった場合

もしプログラム名と、上記の予約が重なった場合は、プログラムの実行時にパスを指定します。例えば、"ALTER"という名前のプログラムがあった場合は次です。  

    pgbash> ./ALTER ........... カレントの"ALTER"プログラム  
    pgbash> /home/admin/ALTER .. /home/adminの"ALTER"プログラム  

### 5. SQL文中のシングルクォート／ダブルクォート

SQL文では、1個のシングルクォート文字データはシングルクォートを２個並べる必要があります。  

例)

    insert into test values(111,'aaa''aaa');  

シングルクォート１個をシングルクォート文字データとして取り扱うには、**'** で囲まなければなりません。  

例) DATA="aaa'aaa"  

    insert into test values(111,__\'__aaa'aaa__\'__);  
    insert into test values(111,__\'__$DATA__\'__);  

SQL文中のダブルクォートは、pgbash-2.4a.1までは、\\" にしなければなりませんでしたが、pgbash-2.4a.2 より、\\ は不要になりました。  

例）

    select aa as "Name", bb as "Tom's Type" from test;  

## SQL文中のコメント

SQL文中に '#' が現れた場合、'#'から改行までをコメントとみなします。  

例)

    create table member (
    #---------------------------------------------------------------
        userid      int4                     # ユーザコード
                    primary key not null,    # プライマリーキー
    #---------------------------------------------------------------
        kind        varchar(12)              # 会員種別
                    check(kind in ('A','B')),# ('A', or 'B')
    #---------------------------------------------------------------
        name        varchar(32) not null,    # ユーザ名
            
    #---------------------------------------------------------------
        zip         char(8)                  # 郵便番号
                    default '000-0000',
    #---------------------------------------------------------------
        address     varchar(64) not null,    # 住所
          
    #---------------------------------------------------------------
        tel         varchar(16) not null,    # 電話番号
          
    #----------------------------------------------------------------
        email       varchar(64),             # E-mailアドレス
          
    #----------------------------------------------------------------
        up_date     timestamp                # 更新日
                    default 'now'            #(初期値：現在の日時)
    #----------------------------------------------------------------
    ); 

* * *

\[[Prev](./usage01-j.md)\] \[[Menu](../usage-j.md)\] \[[Next](./usage03-j.md)\]

\[[Prev](usage07-j.md)\]

\[[Menu](../usage-j.md)\]

\[[Next](usage10-j.md)\]

* * *

**ヘルプおよびデータベース情報の表示について**  

* * *

  

*   **ヘルプメニューの表示方法**  
      
    対話型の操作環境において、？をタイプすると、次のヘルプメニューが表示されます。  
      
    pgbash> **?**
    
    Informational
      ?              : this help (add '+' for more detail)
      ?h \[SQL\]       : help on syntax of SQL commands
      ?l\[+\]          : list databases
      ?d \[NAME\]      : describe table,view,index, or sequence
      ?d{t|i|v|s}\[+\] : list {tables/indexes/views/sequences}
      ?dS\[+\]         : list system tables
      ?da            : list aggregate functions
      ?db\[+\]         : list tablespaces
      ?dc            : list conversions
      ?dC            : list casts
      ?dd            : show comment for object
      ?dD            : list domains
      ?df\[+\]         : list functions
      ?dn\[+\]         : list schemas
      ?do            : list operators
      ?dp            : list table, view, and sequence access privileges, same as ?z
      ?dT\[+\]         : list data\_types
      ?du            : list roles (users), same as ?dg
    
    Large Objects
      ?lo\_import     : lo\_import <'COMMENT'>
      ?lo\_export     : lo\_export ?lo\_unlink     : lo\_unlink ?lo\_list       : list LARGE OBJECTs, same as ?dl
    
    Connection
      ?con\[nect\] DBNAME\[@SERVER\]\[:PORT\] \[CONNECT\_NAME \[USERID \[PASSWORD\]\]\]
                     : connect to new database
      ?set\[connection\] CONNECT\_NAME
                     : change database connection
      ?dis\[connect\]   {CONNECT\_NAME | ALL}
                     : disconnect databases
      ?encoding \[ENCODING\]
                     : show or set client encoding
      ?password USERNAME
                     : securely change the password for a user
    Status
      ?dbc\[onnection\]: show database connection
      ?opt\[ion\]      : show current values of exec\_sql\_OPTIONs
      ?sta\[tus\]      : show STATUS after executing SQL
      ?ver\[sion\]     : show PostgreSQL and pgbash VERSION
    
    Formatting
      E{+|-}         : turn on/off ECHO\_QUERY sent to the backend
      L{+|-}         : turn on/off OUTER\_FRAME\_print
      T{+|-}         : turn on/off TOP\_header\_print
      B{+|-}         : turn on/off BOTTOM\_count\_print
      A{+|-}         : turn on/off ALIGNMENT of plain text
      X{+|-}         : turn on/off EXPANDED format
      H{+|-}         : turn on/off HTML table format
      C+  \[chr\]      : set CAPTION string          or DEFAULT('')
      S+  \[chr\]      : set SEPARATOR  (e.g. ','  ) or DEFAULT('|')
      N+  \[str\]      : set NULL\_STRING(e.g. '\*N\*') or DEFAULT('')
      Z+  \[str\]      : set ZERO\_STRING(e.g. '\*0\*') or DEFAULT('')
    
    Pgbash help
      ??             : help with pgbash usage
      ??connect      : help with CONNECTION usage
      ??sql          : help with SQL usage
      ??sqlcode      : help with SQLCODE
      ??exec\_sql     : help with 'exec\_sql' usage 
    
    (PostgreSQL-8.1の場合)  
      
      
    
*   **SQL予約語一覧のヘルプ**  
      
    対話型環境において、？ｈ　をタイプすると、次のSQL予約語一覧が表示されます。  
      
    pgbash> **?h**
    
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
    
    (PostgreSQL-8.1の場合)  
      
      
    
*   **SQL文法のヘルプ**  
      
    SQL文法を知りたいたおきは、「？ｈ SQL文の先頭の予約語」 をタイプします。  
      
    pgbash> **?h alter database**
    
    Command:     ALTER DATABASE
    Description: change a database
    Syntax:
    ALTER DATABASE name \[ \[ WITH \] option \[ ... \] \]
    
    where option can be:
    
        CONNECTION LIMIT connlimit
    
    ALTER DATABASE name SET parameter { TO | = } { value | DEFAULT }
    ALTER DATABASE name RESET parameter
    
    ALTER DATABASE name RENAME TO newname
    
    ALTER DATABASE name OWNER TO new\_owner
    
      
    
*   **対話型環境におけるデータベース情報の表示方法**  
      
    データベース情報を知りたい場合は、次のヘルプコマンドをタイプします。  
      
    
      ?l\[+\]          : list databases
      ?d \[NAME\]      : describe table,view,index, or sequence
      ?d{t|i|v|s}\[+\] : list {tables/indexes/views/sequences}
      ?dS\[+\]         : list system tables
      ?da            : list aggregate functions
      ?db\[+\]         : list tablespaces
      ?dc            : list conversions
      ?dC            : list casts
      ?dd            : show comment for object
      ?dD            : list domains
      ?df\[+\]         : list functions
      ?dn\[+\]         : list schemas
      ?do            : list operators
      ?dp            : list table, view, and sequence access privileges, same as ?z
      ?dT\[+\]         : list data\_types
      ?du            : list roles (users), same as ?dg
    
    (表示例）  
      
    pgbash> ?du
    
    \[ List of roles \]
     Role name | Superuser | Create role | Create DB | Connections | Member of
    -----------+-----------+-------------+-----------+-------------+-----------
     pgbash    | no        | no          | yes       | no limit    |
     postgres  | yes       | yes         | yes       | no limit    |
    (2 rows)
    
    pgbash> ?l
    
    \[ List of databases \]
     Name      | Owner    | Encoding
    -----------+----------+----------
     pgbash    | pgbash   | UTF8
     postgres  | postgres | UTF8
     template0 | postgres | UTF8
     template1 | postgres | UTF8
    (4 rows)
    
    pgbash> ?d
    
    \[ List of relations \]
     Schema | Name            | Type     | Owner
    --------+-----------------+----------+--------
     public | member          | table    | pgbash
     public | member\_log      | table    | pgbash
     public | member\_log\_view | view     | pgbash
     public | member\_passwd   | table    | pgbash
     public | member\_seq      | sequence | pgbash
     public | test            | table    | pgbash
     public | test\_inherit    | table    | pgbash
     public | test\_sequence   | sequence | pgbash
     public | test\_table      | table    | pgbash
     public | test\_table2     | table    | pgbash
     public | test\_table\_dept | table    | pgbash
     public | test\_view       | view     | pgbash
    (12 rows)
    
    pgbash> ?d member  
    
    \[ Table "test\_table" \]
     Column   | Type                  | NotNull  | Default
    ----------+-----------------------+----------+--------------------------
     code     | integer               | not null |
     name     | character varying(32) |          | 'aaa'::character varying
     address  | character varying(64) |          |
     dept\_id  | character(4)          |          | '001'::bpchar
     jobid    | character(3)          | not null | '999'::bpchar
     inherits | test\_table2           |          |
    (6 rows)
    
    Indexes
     test\_table\_pkey : CREATE UNIQUE INDEX test\_table\_pkey ON test\_table ...
     ix\_jobid        : CREATE INDEX ix\_jobid ON test\_table USING btree (jobid)
    
    Check constraints
     ch\_code  : CHECK (code >= 1 AND code <= 9999)
     ch\_jobid : CHECK (jobid >= 'aaaa'::bpchar AND jobid <= 'zzzz'::bpchar)
    
    Foreign keys
     test\_table\_dept\_id\_fkey : FOREIGN KEY (dept\_id) REFERENCES test\_table\_...
    
      
    

* * *

\[[Prev](./usage01-j.md)\]

\[[Menu](../usage-j.md)\]

\[[Next](./usage10-j.md)\]

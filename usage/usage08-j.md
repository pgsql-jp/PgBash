\[[Prev](usage07-j.md)\] \[[Menu](../usage-j.md)\] \[[Next](usage10-j.md)\]

* * *

# ヘルプおよびデータベース情報の表示について

## ヘルプメニューの表示方法<a id="HELPMENU" name="HELPMENU">&nbsp;</a>

対話型の操作環境において、？をタイプすると、次のヘルプメニューが表示されます。  

    pgbash> ?

    Informational
      ?              : this help (add '+' for more detail)
      ?h [SQL]       : help on syntax of SQL commands
      ?l[+]          : list databases
      ?d [NAME]      : describe table,view,index, or sequence
      ?d{t|i|v|s}[+] : list {tables/indexes/views/sequences}
      ?dS[+]         : list system tables
      ?da            : list aggregate functions
      ?db[+]         : list tablespaces
      ?dc            : list conversions
      ?dC            : list casts
      ?dd            : show comment for object
      ?dD            : list domains
      ?df[+]         : list functions
      ?dn[+]         : list schemas
      ?do            : list operators
      ?dp            : list table, view, and sequence access privileges, same as ?z
      ?dT[+]         : list data_types
      ?du            : list roles (users), same as ?dg

    Large Objects
      ?lo_import     : lo_import <'COMMENT'>
      ?lo_export     : lo_export ?lo_unlink     : lo_unlink ?lo_list       : list LARGE OBJECTs, same as ?dl

    Connection
      ?con[nect] DBNAME[@SERVER][:PORT] [CONNECT_NAME [USERID [PASSWORD]]]
                     : connect to new database
      ?set[connection] CONNECT_NAME
                     : change database connection
      ?dis[connect]   {CONNECT_NAME | ALL}
                     : disconnect databases
      ?encoding [ENCODING]
                     : show or set client encoding
      ?password USERNAME
                     : securely change the password for a user
    Status
      ?dbc[onnection]: show database connection
      ?opt[ion]      : show current values of exec_sql_OPTIONs
      ?sta[tus]      : show STATUS after executing SQL
      ?ver[sion]     : show PostgreSQL and pgbash VERSION
    
    Formatting
      E{+|-}         : turn on/off ECHO_QUERY sent to the backend
      L{+|-}         : turn on/off OUTER_FRAME_print
      T{+|-}         : turn on/off TOP_header_print
      B{+|-}         : turn on/off BOTTOM_count_print
      A{+|-}         : turn on/off ALIGNMENT of plain text
      X{+|-}         : turn on/off EXPANDED format
      H{+|-}         : turn on/off HTML table format
      C+  [chr]      : set CAPTION string          or DEFAULT('')
      S+  [chr]      : set SEPARATOR  (e.g. ','  ) or DEFAULT('|')
      N+  [str]      : set NULL_STRING(e.g. '*N*') or DEFAULT('')
      Z+  [str]      : set ZERO_STRING(e.g. '*0*') or DEFAULT('')
    
    Pgbash help
      ??             : help with pgbash usage
      ??connect      : help with CONNECTION usage
      ??sql          : help with SQL usage
      ??sqlcode      : help with SQLCODE
      ??exec_sql     : help with 'exec_sql' usage 

(PostgreSQL-8.1の場合)  

## SQL予約語一覧のヘルプ<a id="RESERVED" name="RESERVED">&nbsp;</a>

対話型環境において、？ｈ　をタイプすると、次のSQL予約語一覧が表示されます。  

    pgbash> ?h
    
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

## SQL文法のヘルプ<a id="SQLSYNTAX" name="SQLSYNTAX">&nbsp;</a>

SQL文法を知りたいたおきは、「？ｈ SQL文の先頭の予約語」 をタイプします。  

    pgbash> ?h alter database
    
    Command:     ALTER DATABASE
    Description: change a database
    Syntax:
    ALTER DATABASE name [ [ WITH ] option [ ... ] ]
    
    where option can be:
    
        CONNECTION LIMIT connlimit
    
    ALTER DATABASE name SET parameter { TO | = } { value | DEFAULT }
    ALTER DATABASE name RESET parameter
    
    ALTER DATABASE name RENAME TO newname
    
    ALTER DATABASE name OWNER TO new_owner

## 対話型環境におけるデータベース情報の表示方法<a id="I_DBINFO" name="I_DBINFO">&nbsp;</a>

データベース情報を知りたい場合は、次のヘルプコマンドをタイプします。  

    ?l[+]          : list databases
    ?d [NAME]      : describe table,view,index, or sequence
    ?d{t|i|v|s}[+] : list {tables/indexes/views/sequences}
    ?dS[+]         : list system tables
    ?da            : list aggregate functions
    ?db[+]         : list tablespaces
    ?dc            : list conversions
    ?dC            : list casts
    ?dd            : show comment for object
    ?dD            : list domains
    ?df[+]         : list functions
    ?dn[+]         : list schemas
    ?do            : list operators
    ?dp            : list table, view, and sequence access privileges, same as ?z
    ?dT[+]         : list data_types
    ?du            : list roles (users), same as ?dg

(表示例）  

    pgbash> ?du
    
    [ List of roles ]
     Role name | Superuser | Create role | Create DB | Connections | Member of
    -----------+-----------+-------------+-----------+-------------+-----------
     pgbash    | no        | no          | yes       | no limit    |
     postgres  | yes       | yes         | yes       | no limit    |
    (2 rows)

(表示例）  

    pgbash> ?l
    
    [ List of databases ]
     Name      | Owner    | Encoding
    -----------+----------+----------
     pgbash    | pgbash   | UTF8
     postgres  | postgres | UTF8
     template0 | postgres | UTF8
     template1 | postgres | UTF8
    (4 rows)

(表示例）  

    pgbash> ?d
    
    [ List of relations ]
     Schema | Name            | Type     | Owner
    --------+-----------------+----------+--------
     public | member          | table    | pgbash
     public | member_log      | table    | pgbash
     public | member_log_view | view     | pgbash
     public | member_passwd   | table    | pgbash
     public | member_seq      | sequence | pgbash
     public | test            | table    | pgbash
     public | test_inherit    | table    | pgbash
     public | test_sequence   | sequence | pgbash
     public | test_table      | table    | pgbash
     public | test_table2     | table    | pgbash
     public | test_table_dept | table    | pgbash
     public | test_view       | view     | pgbash
    (12 rows)

(表示例）  

    pgbash> ?d member  
    
    [ Table "test_table" ]
     Column   | Type                  | NotNull  | Default
    ----------+-----------------------+----------+--------------------------
     code     | integer               | not null |
     name     | character varying(32) |          | 'aaa'::character varying
     address  | character varying(64) |          |
     dept_id  | character(4)          |          | '001'::bpchar
     jobid    | character(3)          | not null | '999'::bpchar
     inherits | test_table2           |          |
    (6 rows)
    
    Indexes
     test_table_pkey : CREATE UNIQUE INDEX test_table_pkey ON test_table ...
     ix_jobid        : CREATE INDEX ix_jobid ON test_table USING btree (jobid)
    
    Check constraints
     ch_code  : CHECK (code >= 1 AND code <= 9999)
     ch_jobid : CHECK (jobid >= 'aaaa'::bpchar AND jobid <= 'zzzz'::bpchar)
    
    Foreign keys
     test_table_dept_id_fkey : FOREIGN KEY (dept_id) REFERENCES test_table_...


* * *

\[[Prev](./usage01-j.md)\] \[[Menu](../usage-j.md)\] \[[Next](./usage10-j.md)\]

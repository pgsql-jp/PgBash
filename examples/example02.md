# PgBash example-02

* * *

## Example of executing SQL in the interactive environment.

* * *


prompt> **/usr/local/bin/pgbash**  

    Welcome to Pgbash ( bash-x.x.x ) Patch Ver.x rX.X
    
      Type '?'  for help with pgbash commands.
      Type 'exit' or 'Ctrl+D' to terminate Pgbash.

  
*   **Help menu**  
    pgbash> **?**  
~~~ 
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
      ?lo_export     : lo_export
      ?lo_unlink     : lo_unlink
      ?lo_list       : list LARGE OBJECTs, same as ?dl
    
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
~~~


*   **SQL examples**  
1. )    
    pgbash> **?con pgbash:8408 db84**  

~~~
    # PostgreSQL 8.4.8 on i686-pc-linux-gnu, compiled by GCC gcc (GCC) 4.1.2 
    # CONNECT TO  pgbash:8408  AS  db84  USER  pgbash
    # List of database connection (C: current database is '*')
    +---+--------------+-----------+-----------------------------+-----------------+
    | C | connect_name | user_name | target_name(db@server:port) | client_encoding |
    +---+--------------+-----------+-----------------------------+-----------------+
    | * | db84         | pgbash    | pgbash:8408                 |                 |
    +---+--------------+-----------+-----------------------------+-----------------+
    (1 row)
~~~
2. )    
    pgbash> **insert into member(userid,name,zip,address1,tel,email) values(**  
    &gt; **22, 'xxxxx11','611-2222','osaka','078-999-8888','youko@xxx.co.jp');**  
~~~
    INSERT xxxxxxx  
~~~
3. )    
    pgbash> **insert into member(userid,name,zip,address1,tel,email) values(**  
    &gt; **23, 'aaaaa22','622-3333','osaka','078-888-7777','haru@aaa.co.jp');**
~~~
    INSERT xxxxxxx  
~~~
..................  

4. )
    pgbash> **select userid,name,zip,address1,tel,email from member;**  
~~~
     userid | name     | zip      | address1 | tel          | email           
    --------+----------+----------+----------+--------------+-----------------
         22 | xxxxx11  | 611-2222 | osaka    | 078-999-8888 | youko@xxx.co.jp 
         23 | aaaaa22  | 622-3333 | osaka    | 078-888-7777 | haru@aaa.co.jp  
         24 | bbbbb33  | 633-4444 | kobe-shi | 078-666-5555 | nobu@bbb.co.jp 
         25 | cccccc   | 644-5555 | kobe-shi | 06-1111-2222 | yumi@ccc.co.jp 
         26 | ddddd55  | 311-2222 | kobe-shi | 03-2222-3333 | miti@ddd.co.jp 
    (5 rows)
~~~
5. )     
    pgbash> **L+**  
    pgbash> **!select**  
~~~
    +--------+----------+----------+----------+--------------+-----------------+
    | userid | name     | zip      | address1 | tel          | email           |
    +--------+----------+----------+----------+--------------+-----------------+
    |     22 | xxxxx11  | 611-2222 | osaka    | 078-999-8888 | youko@xxx.co.jp |
    |     23 | aaaaa22  | 622-3333 | osaka    | 078-888-7777 | haru@aaa.co.jp  | 
    |     24 | bbbbb33  | 633-4444 | kobe-shi | 078-666-5555 | nobu@bbb.co.jp  | 
    |     25 | cccccc   | 644-5555 | kobe-shi | 06-1111-2222 | yumi@ccc.co.jp  |
    |     26 | ddddd55  | 311-2222 | kobe-shi | 03-2222-3333 | miti@ddd.co.jp  |
    +--------+----------+----------+----------+--------------+-----------------+
    (5 rows)
~~~
6. )     
    pgbash> **!! | more**  
~~~
    +--------+----------+----------+----------+--------------+-----------------+
    | userid | name     | zip      | address1 | tel          | email           |
    +--------+----------+----------+----------+--------------+-----------------+
    |     22 | xxxxx11  | 611-2222 | osaka    | 078-999-8888 | youko@xxx.co.jp |
    |     23 | aaaaa22  | 622-3333 | osaka    | 078-888-7777 | haru@aaa.co.jp  | 
    |     24 | bbbbb33  | 633-4444 | kobe-shi | 078-666-5555 | nobu@bbb.co.jp  | 
    |     25 | cccccc   | 644-5555 | kobe-shi | 06-1111-2222 | yumi@ccc.co.jp  |
    |     26 | ddddd55  | 311-2222 | kobe-shi | 03-2222-3333 | miti@ddd.co.jp  |
    +--------+----------+----------+----------+--------------+-----------------+
    (5 rows)
~~~
7. )     
    pgbash> **select * from member ; &> /tmp/sel.dat &**  
    pgbash> cat /tmp/sel.dat  
~~~
    +--------+----------+----------+----------+--------------+-----------------+
    | userid | name     | zip      | address1 | tel          | email           |
    +--------+----------+----------+----------+--------------+-----------------+
    |     22 | xxxxx11  | 611-2222 | osaka    | 078-999-8888 | youko@xxx.co.jp |
    |     23 | aaaaa22  | 622-3333 | osaka    | 078-888-7777 | haru@aaa.co.jp  | 
    |     24 | bbbbb33  | 633-4444 | kobe-shi | 078-666-5555 | nobu@bbb.co.jp  | 
    |     25 | cccccc   | 644-5555 | kobe-shi | 06-1111-2222 | yumi@ccc.co.jp  |
    |     26 | ddddd55  | 311-2222 | kobe-shi | 03-2222-3333 | miti@ddd.co.jp  |
    +--------+----------+----------+----------+--------------+-----------------+
    (5 rows)
~~~
8. )      
    pgbash> **L-**  
    pgbash> **select * from member_log_view;**  
~~~
    stat| userid | name     | zip      | address1 | tel            | email
    ----+--------+----------+----------+----------+----------------+-----------------
        |   1265 | kitamina | 555-1111 | Toyonaka |                | tada@xxx.ne.jp
    UP  |   1265 | kitamina | 555-1111 | Toyonaka | 6666-1111,090  | tada@xxx.ne.jp
        |   1252 | hota     | 666-1111 | Nisinomi | 0797-61-1749   | mun@xxxxx.com 
    UP  |   1252 | hota     | 666-1111 | Nisinomi | 0797-61-1749   | mune@xxxxx.com
    ....................
~~~
      
    
*   **SQL syntax**  
      
1. ) List of SQL reserved words  
      
    pgbash> **?h**
~~~
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
~~~

2. ) SQL syntax  
      
    pgbash> **?h crete database**  
````    
    Command:     CREATE DATABASE
    Description: create a new database
    Syntax:
    CREATE DATABASE name
        [ [ WITH ] [ OWNER [=] dbowner ]
               [ TEMPLATE [=] template ]
               [ ENCODING [=] encoding ]
               [ TABLESPACE [=] tablespace ]
               [ CONNECTION LIMIT [=] connlimit ] ]
````    
      
    
*   **How to use Pgbash SQL**  
      
    pgbash> **??sql**
~~~
    # SQL usage:
      Type '[timespec] SQL; [pipeline][redirection][&]'
      ex)
      select * from test limit 300;
      select * from test; > /tmp/test.dat
      select * from test; >> /tmp/test.dat &
      select * from test; | more
      time  select * from test limit 300;
    
      select code, name into :code, :name from test;
      if [ $code = '101' ]; then
          echo "code=$code name=$name"
      fi
    
      begin;
      declare cur cursor for select code,name from test order by code;
      while (( $SQLCODE == $SQL_OK )); do
         fetch in cur into :code, :name;
         if (( $SQLCODE < 0 || $SQLCODE == $SQL_NOT_FOUND)); then
            break
         else
             echo "code=$code  name=$name"
         fi
      done
      end;
~~~
      
    
*   **Error code**  
    pgbash> **??sqlcode**  
~~~
    # SQLCODE
      ex)
      begin;
      declare cur cursor for select code,name from test order by code;
      while (( $SQLCODE == $SQL_OK )); do
         fetch in cur into :code, :name;
         if (( $SQLCODE < 0 || $SQLCODE == $SQL_NOT_FOUND)); then
            break
         else
             echo "code=$code  name=$name"
         fi
      done
      end;
    
    -------------------------+-------------------------------------------+--------
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
~~~
      
    
*   **exec_sq options**  
    pgbash> **??ecec_sql**  
    
    exec_sql: exec_sql [option] ["SQL"]
~~~
        --------------------- show infomations ---------------------
        -h item   show exec_sql OPTION or sql_ERRCODE(item: OP or ER)
        -v        show PostgreSQL and PGBASH VERSION
        -m        show CONNECTION NAMEs
        -s        show STATUS after executing SQL
    
        --------------------- temporary options --------------------
        -d conn   set  CONNECTION NAME
        -e        turn on  ECHO  query sent to the backend
        -q        turn on  QUIET system/sql error messages
        -H        turn on  HTML  format(-H is default in CGI    mode)
        -X        turn on  EXPANDED  format
        -L        turn on  OUTER_FRAME  of PLAIN text
        -A        turn off ALIGNMENT    of PLAIN text
        -T        turn off TOP_header   print
        -B        turn off BOTTOM_count print
        -C cap    set CAPTION of HTML/PLAIN table(e.g. -C 'TITLE')
        -S sep    set SEPARATOR of PLAIN text field (default is '|')
        -N null   set NULL_STRING(e.g. -N '*N*')
        -Z zero   set ZERO_STRINg(e.g. -Z '*0*')
    
        --------------------- execute function  --------------------
        -x func   execute func(e.g. lo_import, lo_export, lo_unlink)
~~~
      
    
*   **List of database connection**  
    pgbash> **?dbc**
~~~
    # List of database connection (C: current database is '*')
    +---+--------------+-----------+-----------------------------+-----------------+
    | C | connect_name | user_name | target_name(db@server:port) | client_encoding |
    +---+--------------+-----------+-----------------------------+-----------------+
    | * | _DEFAULT_    | pgbash    | pgbash:5432                 |                 |
    |   | pgbash       | pgbash    | pgbash@db.psn.ne.jp:8408    |                 |
    +---+--------------+-----------+-----------------------------+-----------------+
    (2 rows) 
~~~
      
    
*   **Value of options**  
    pgbash> **?opt**  
~~~
    # EXEC_SQL_OPTION
      Usage:
      set EXEC_SQL_OPTION DEFAULT ;   --- reset DEFAULT
      set EXEC_SQL_OPTION CGI ;       --- set CGI mode
      set <option_name>[=value] ;
    
      ex)
      SET OPTION_ECHO; or SET OPTION_ECHO=ON; or SET OPTION_ECHO=TRUE;
      SET OPTION_BOTTOM=OFF; or SET OPTION_BOTTOM=FALSE;
      set OPTION_SEPARATOR=',';
      set OPTION_NULLSTRING="\N";
    
    +-------------------+-------------------------+-------+---------------
    |                  |      Comment            |DEFAULT| Current Value
    +-------------------+-------------------------+-------+---------------
    | OPTION_ECHO       | turn on/off ECHO query  | OFF   | OFF
    | OPTION_QUIET      | turn on/off QUIET mode  | OFF   | OFF
    | OPTION_HEADER     | turn on/off TOP_header  | ON    | ON
    | OPTION_BOTTOM     | turn on/off BOTTOM_print| ON    | ON
    | OPTION_ALIGNMENT  | turn on/off ALIGNMENT   | ON    | ON
    | OPTION_FRAME      | turn on/off OUTER_FRAME | OFF   | OFF
    | OPTION_EXPANDED   | turn on/off EXPANDED    | OFF   | OFF
    | OPTION_SEPARATOR  | set SEPARATOR character | ''    | ''
    | OPTION_NULLSTRING | set NULL string         | ""    | ""
    | OPTION_ZEROSTRING | set ZERO string         | ""    | ""
    | OPTION_CAPTION    | set CAPTION             | ""    | ""
    +-------------------+-------------------------+-------+---------------
    
    [ CGI_mode / HTML_output_mode ]
    +-------------------+-------------------------+-------+---------------
    |               |      Comment            |DEFAULT| Current Value
    +-------------------+-------------------------+-------+---------------
    | OPTION_HTML       | turn on/off HTML mode   | OFF   | OFF
    | OPTION_TABLETAG   | set HTML table tag      | ""    | ""
    | OPTION_HEADERTR   | set HTML header <TR>    | ""    | ""
    | OPTION_HEADERTH   | set HTML header <TH>    | ""    | ""
    | OPTION_BODYTAG    | set HTML body <TD> tag  | ""    | ""
    | OPTION_INPUTTAG   | turn on/off INPUT tag   | OFF   | OFF
    | OPTION_INPUTSIZE  | set HTML input tag SIZE | ""    | ""
    +-------------------+-------------------------+-------+--------------- 
~~~
      
    
*   **Status after executing SQL**  
1. ) The case of normal end  
    pgbash> **?sta**  
~~~
    # SQL status 
      SQLCODE   = 0         (SQL error code)
      SQLNTUPLE = 2         (number of tuples)
      SQLNFIELD = 3         (number of fields)
      SQLERRML  = 0         (length of SQLERRMC)
      SQLERRMC  =
~~~
      
2. ) The case of abnormal end  
    pgbash> **select aa,bb from test;**  

    (-403)ERROR: attribute 'aa' not found  
    
    pgbash> **?sta**  
~~~
    # SQL status 
      SQLCODE   = -403      (SQL error code)
      SQLNTUPLE = 0         (number of tuples)
      SQLNFIELD = 0         (number of fields)
      SQLERRML  = 33        (length of SQLERRMC)
      SQLERRMC  = ERROR:  attribute 'aa' not found 
~~~
      
*   **Version number**  
    pgbash> **?ver**
~~~
    # PostgreSQL 8.1.23 on i686-pc-linux-gnu, compiled by GCC gcc (GCC) 4.1.2 20..
    # Pgbash ( bash-3.2.48(5)-release ) Patch Ver.8 r3.2
    # Pgbashrc Version 8.1 
~~~

*   **List of database users**  
    pgbash> **?du**  
~~~
    [ List of roles ]
     Role name | Superuser | Create role | Create DB | Connections | Member of
    -----------+-----------+-------------+-----------+-------------+-----------
     pgbash    | no        | no          | yes       | no limit    |
     postgres  | yes       | yes         | yes       | no limit    |
    (2 rows)
~~~
      
    
*   **List of databases**  
    pgbash> **?l**  
~~~
    [ List of databases ]
     Name      | Owner    | Encoding
    -----------+----------+----------
     pgbash    | pgbash   | UTF8
     postgres  | postgres | UTF8
     template0 | postgres | UTF8
     template1 | postgres | UTF8
    (4 rows)
~~~

*   **List of relations**   
1)  List of relation names.     
    pgbash> **?d**  
~~~
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
~~~
      
2) List of attributes of relation  
    pgbash> **?d test_table**  
~~~
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
     test_table_pkey : CREATE UNIQUE INDEX test_table_pkey ON test_table...
     ix_jobid        : CREATE INDEX ix_jobid ON test_table USING btree (jobid)
    
    Check constraints
     ch_code  : CHECK (code >= 1 AND code <= 9999)
     ch_jobid : CHECK (jobid >= 'aaaa'::bpchar AND jobid <= 'zzzz'::bpchar)
    
    Foreign keys
     test_table_dept_id_fkey : FOREIGN KEY (dept_id) REFERENCES test_table_dept..
~~~
      

*   **List of tables**  
    pgbash> **?dt**  
~~~
    [ List of tables ]
     Schema | Name            | Type  | Owner
    --------+-----------------+-------+--------
     public | member          | table | pgbash
     public | member_log      | table | pgbash
     public | member_passwd   | table | pgbash
     public | test            | table | pgbash
     public | test_inherit    | table | pgbash
     public | test_table      | table | pgbash
     public | test_table2     | table | pgbash
     public | test_table_dept | table | pgbash
    (8 rows) 
~~~

*   **List of indexes**  
    pgbash> **?di**  
~~~
    [ List of indexes ]
     Schema | Name                 | Type  | Owner  | Table
    --------+----------------------+-------+--------+-----------------
     public | ix_jobid             | index | pgbash | test_table
     public | member_email         | index | pgbash | member
     public | member_name          | index | pgbash | member
     public | member_passwd_pkey   | index | pgbash | member_passwd
     public | member_pkey          | index | pgbash | member
     public | member_tel           | index | pgbash | member
     public | test_table2_pkey     | index | pgbash | test_table2
     public | test_table_dept_pkey | index | pgbash | test_table_dept
     public | test_table_pkey      | index | pgbash | test_table
    (9 rows) 
~~~

*   **List of views**  
    pgbash> **?dv**  
~~~
    [ List of views ]
     Schema | Name            | Type | Owner
    --------+-----------------+------+--------
     public | member_log_view | view | pgbash
     public | test_view       | view | pgbash
    (2 rows) 
~~~

*   **List of large objects**  
    pgbash> **?dl (or _lo_list)**  
~~~
    [ List of large objects ]
        OID | Description
    --------+--------------
     118865 | member data1
     118867 | member data2
    (2 rows) 
~~~

*   **List of permission**  
    pgbash> **?dp**  
~~~
    [ List of access privileges ]
     Schema | Name            | Type     | Access privileges
    --------+-----------------+----------+-------------------------
     public | member          | table    | {pgbash=arwdRxt/pgbash}
     public | member_log      | table    |
     public | member_log_view | view     |
     public | member_passwd   | table    | {pgbash=arwdRxt/pgbash}
     public | member_seq      | sequence |
     public | test            | table    |
     public | test_inherit    | table    |
     public | test_sequence   | sequence |
     public | test_table      | table    |
     public | test_table2     | table    |
     public | test_table_dept | table    |
     public | test_view       | view     |
    (12 rows) 
~~~

*   **List of sequnces**  
    pgbash> **?ds**  
~~~
    [ List of sequences ]
     Schema | Name          | Type     | Owner
    --------+---------------+----------+--------
     public | member_seq    | sequence | pgbash
     public | test_sequence | sequence | pgbash
    (2 rows)
~~~
      
* **Ending**      
    pgbash> **disconnect all;**  
    pgbash> **exit**  
    
prompt>


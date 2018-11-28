* * *

**Example of executing SQL in the interactive environment.**  

* * *

  
prompt> **/usr/local/bin/pgbash**  

Welcome to Pgbash ( bash-x.x.x ) Patch Ver.x rX.X

  Type '?'  for help with pgbash commands.
  Type 'exit' or 'Ctrl+D' to terminate Pgbash.

  
*   **Help menu**  
      
    **pgbash> ?**  
    
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
    
      
    
*   **SQL examples**  
      
    pgbash> **?con pgbash:8408 db84**  
    
    \# PostgreSQL 8.4.8 on i686-pc-linux-gnu, compiled by GCC gcc (GCC) 4.1.2 
    # CONNECT TO  pgbash:8408  AS  db84  USER  pgbash
    # List of database connection (C: current database is '\*')
    +---+--------------+-----------+-----------------------------+-----------------+
    | C | connect\_name | user\_name | target\_name(db@server:port) | client\_encoding |
    +---+--------------+-----------+-----------------------------+-----------------+
    | \* | db84         | pgbash    | pgbash:8408                 |                 |
    +---+--------------+-----------+-----------------------------+-----------------+
    (1 row)
    
      
    pgbash> **insert into member(userid,name,zip,address1,tel,email) values(**  
    \> **22, 'xxxxx11','611-2222','osaka','078-999-8888','youko@xxx.co.jp');**  
    INSERT xxxxxxx  
    pgbash> **insert into member(userid,name,zip,address1,tel,email) values(**  
    \> **23, 'aaaaa22','622-3333','osaka','078-888-7777','haru@aaa.co.jp');**  
    INSERT xxxxxxx  
    ..................  
      
    pgbash> **select userid,name,zip,address1,tel,email from member;**  
    
     userid | name     | zip      | address1 | tel          | email           
    --------+----------+----------+----------+--------------+-----------------
         22 | xxxxx11  | 611-2222 | osaka    | 078-999-8888 | youko@xxx.co.jp 
         23 | aaaaa22  | 622-3333 | osaka    | 078-888-7777 | haru@aaa.co.jp  
         24 | bbbbb33  | 633-4444 | kobe-shi | 078-666-5555 | nobu@bbb.co.jp 
         25 | cccccc   | 644-5555 | kobe-shi | 06-1111-2222 | yumi@ccc.co.jp 
         26 | ddddd55  | 311-2222 | kobe-shi | 03-2222-3333 | miti@ddd.co.jp 
    (5 rows)
    
      
    pgbash> **L+**  
    pgbash> **!select**  
    
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
    
      
    pgbash> **!! | more**  
    
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
    
      
    pgbash> **select \* from member ; &> /tmp/sel.dat &**  
    pgbash> cat /tmp/sel.dat  
    
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
    
      
    pgbash> **L-**  
    pgbash> **select \* from member\_log\_view;**  
    
    stat| userid | name     | zip      | address1 | tel            | email
    ----+--------+----------+----------+----------+----------------+-----------------
        |   1265 | kitamina | 555-1111 | Toyonaka |                | tada@xxx.ne.jp
    UP  |   1265 | kitamina | 555-1111 | Toyonaka | 6666-1111,090  | tada@xxx.ne.jp
        |   1252 | hota     | 666-1111 | Nisinomi | 0797-61-1749   | mun@xxxxx.com 
    UP  |   1252 | hota     | 666-1111 | Nisinomi | 0797-61-1749   | mune@xxxxx.com
    ....................
    
      
    
*   **SQL syntax**  
      
    (1) List of SQL reserved words  
      
    **pgbash> ?h**
    
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
    
      
    (2) SQL syntax  
      
    **pgbash> ?h crete database**  
    
    Command:     CREATE DATABASE
    Description: create a new database
    Syntax:
    CREATE DATABASE name
        \[ \[ WITH \] \[ OWNER \[=\] dbowner \]
               \[ TEMPLATE \[=\] template \]
               \[ ENCODING \[=\] encoding \]
               \[ TABLESPACE \[=\] tablespace \]
               \[ CONNECTION LIMIT \[=\] connlimit \] \]
    
      
    
*   **How to use Pgbash SQL**  
      
    **pgbash> ??sql**
    
    \# SQL usage:
      Type '\[timespec\] SQL; \[pipeline\]\[redirection\]\[&\]'
      ex)
      select \* from test limit 300;
      select \* from test; > /tmp/test.dat
      select \* from test; >> /tmp/test.dat &
      select \* from test; | more
      time  select \* from test limit 300;
    
      select code, name into :code, :name from test;
      if \[ $code = '101' \]; then
          echo "code=$code name=$name"
      fi
    
      begin;
      declare cur cursor for select code,name from test order by code;
      while (( $SQLCODE == $SQL\_OK )); do
         fetch in cur into :code, :name;
         if (( $SQLCODE < 0 || $SQLCODE == $SQL\_NOT\_FOUND)); then
            break
         else
             echo "code=$code  name=$name"
         fi
      done
      end;
    
      
    
*   **Error code**  
      
    **pgbash> ??sqlcode**  
    
    \# SQLCODE
      ex)
      begin;
      declare cur cursor for select code,name from test order by code;
      while (( $SQLCODE == $SQL\_OK )); do
         fetch in cur into :code, :name;
         if (( $SQLCODE < 0 || $SQLCODE == $SQL\_NOT\_FOUND)); then
            break
         else
             echo "code=$code  name=$name"
         fi
      done
      end;
    
    -------------------------+-------------------------------------------+--------
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
    
      
    
*   **exec\_sq options**  
      
    **pgbash> ??ecec\_sql**  
    
    exec\_sql: exec\_sql \[option\] \["SQL"\]
    
        --------------------- show infomations ---------------------
        -h item   show exec\_sql OPTION or sql\_ERRCODE(item: OP or ER)
        -v        show PostgreSQL and PGBASH VERSION
        -m        show CONNECTION NAMEs
        -s        show STATUS after executing SQL
    
        --------------------- temporary options --------------------
        -d conn   set  CONNECTION NAME
        -e        turn on  ECHO  query sent to the backend
        -q        turn on  QUIET system/sql error messages
        -H        turn on  HTML  format(-H is default in CGI    mode)
        -X        turn on  EXPANDED  format
        -L        turn on  OUTER\_FRAME  of PLAIN text
        -A        turn off ALIGNMENT    of PLAIN text
        -T        turn off TOP\_header   print
        -B        turn off BOTTOM\_count print
        -C cap    set CAPTION of HTML/PLAIN table(e.g. -C 'TITLE')
        -S sep    set SEPARATOR of PLAIN text field (default is '|')
        -N null   set NULL\_STRING(e.g. -N '\*N\*')
        -Z zero   set ZERO\_STRINg(e.g. -Z '\*0\*')
    
        --------------------- execute function  --------------------
        -x func   execute func(e.g. lo\_import, lo\_export, lo\_unlink)
    
      
    
*   **List of database connection**  
      
    **pgbash> ?dbc**
    
    \# List of database connection (C: current database is '\*')
    +---+--------------+-----------+-----------------------------+-----------------+
    | C | connect\_name | user\_name | target\_name(db@server:port) | client\_encoding |
    +---+--------------+-----------+-----------------------------+-----------------+
    | \* | \_DEFAULT\_    | pgbash    | pgbash:5432                 |                 |
    |   | pgbash       | pgbash    | pgbash@db.psn.ne.jp:8408    |                 |
    +---+--------------+-----------+-----------------------------+-----------------+
    (2 rows) 
    
      
    
*   **Value of options**  
      
    **pgbash> ?opt**  
    
    \# EXEC\_SQL\_OPTION
      Usage:
      set EXEC\_SQL\_OPTION DEFAULT ;   --- reset DEFAULT
      set EXEC\_SQL\_OPTION CGI ;       --- set CGI mode
      set <option\_name>\[=value\] ;
    
      ex)
      SET OPTION\_ECHO; or SET OPTION\_ECHO=ON; or SET OPTION\_ECHO=TRUE;
      SET OPTION\_BOTTOM=OFF; or SET OPTION\_BOTTOM=FALSE;
      set OPTION\_SEPARATOR=',';
      set OPTION\_NULLSTRING="\\N";
    
    +-------------------+-------------------------+-------+---------------
    |   |      Comment            |DEFAULT| Current Value
    +-------------------+-------------------------+-------+---------------
    | OPTION\_ECHO       | turn on/off ECHO query  | OFF   | OFF
    | OPTION\_QUIET      | turn on/off QUIET mode  | OFF   | OFF
    | OPTION\_HEADER     | turn on/off TOP\_header  | ON    | ON
    | OPTION\_BOTTOM     | turn on/off BOTTOM\_print| ON    | ON
    | OPTION\_ALIGNMENT  | turn on/off ALIGNMENT   | ON    | ON
    | OPTION\_FRAME      | turn on/off OUTER\_FRAME | OFF   | OFF
    | OPTION\_EXPANDED   | turn on/off EXPANDED    | OFF   | OFF
    | OPTION\_SEPARATOR  | set SEPARATOR character | ''    | ''
    | OPTION\_NULLSTRING | set NULL string         | ""    | ""
    | OPTION\_ZEROSTRING | set ZERO string         | ""    | ""
    | OPTION\_CAPTION    | set CAPTION             | ""    | ""
    +-------------------+-------------------------+-------+---------------
    
    \[ CGI\_mode / HTML\_output\_mode \]
    +-------------------+-------------------------+-------+---------------
    |   |      Comment            |DEFAULT| Current Value
    +-------------------+-------------------------+-------+---------------
    | OPTION\_HTML       | turn on/off HTML mode   | OFF   | OFF
    | OPTION\_TABLETAG   | set HTML table tag      | ""    | ""
    | OPTION\_HEADERTR   | set HTML header <TR>    | ""    | ""
    | OPTION\_HEADERTH   | set HTML header <TH>    | ""    | ""
    | OPTION\_BODYTAG    | set HTML body <TD> tag  | ""    | ""
    | OPTION\_INPUTTAG   | turn on/off INPUT tag   | OFF   | OFF
    | OPTION\_INPUTSIZE  | set HTML input tag SIZE | ""    | ""
    +-------------------+-------------------------+-------+--------------- 
    
      
    
*   **Status after executing SQL**  
      
    (1) The case of normal end  
    **pgbash> ?sta**  
    
    \# SQL status 
      SQLCODE   = 0         (SQL error code)
      SQLNTUPLE = 2         (number of tuples)
      SQLNFIELD = 3         (number of fields)
      SQLERRML  = 0         (length of SQLERRMC)
      SQLERRMC  =
    
      
    (2) The case of abnormal end  
    **pgbash> select aa,bb from test;**  
    (-403)ERROR: attribute 'aa' not found  
    **pgbash> ?sta**  
    
    \# SQL status 
      SQLCODE   = -403      (SQL error code)
      SQLNTUPLE = 0         (number of tuples)
      SQLNFIELD = 0         (number of fields)
      SQLERRML  = 33        (length of SQLERRMC)
      SQLERRMC  = ERROR:  attribute 'aa' not found 
*   **Version number**  
      
    **pgbash> ?ver**
    
    \# PostgreSQL 8.1.23 on i686-pc-linux-gnu, compiled by GCC gcc (GCC) 4.1.2 20..
    # Pgbash ( bash-3.2.48(5)-release ) Patch Ver.8 r3.2
    # Pgbashrc Version 8.1 
*   **List of database users**  
      
    **pgbash> ?du**  
    
    \[ List of roles \]
     Role name | Superuser | Create role | Create DB | Connections | Member of
    -----------+-----------+-------------+-----------+-------------+-----------
     pgbash    | no        | no          | yes       | no limit    |
     postgres  | yes       | yes         | yes       | no limit    |
    (2 rows)
    
      
    
*   **List of databases**  
      
    **pgbash> ?l**  
    
    \[ List of databases \]
     Name      | Owner    | Encoding
    -----------+----------+----------
     pgbash    | pgbash   | UTF8
     postgres  | postgres | UTF8
     template0 | postgres | UTF8
     template1 | postgres | UTF8
    (4 rows)
    
*   **List of relations**  
      
    (1) List of relation names.  
      
    **pgbash> ?d**  
    
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
    
      
    (2) List of attributes of relation  
      
    **pgbash> ?d test\_table**  
    
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
     test\_table\_pkey : CREATE UNIQUE INDEX test\_table\_pkey ON test\_table...
     ix\_jobid        : CREATE INDEX ix\_jobid ON test\_table USING btree (jobid)
    
    Check constraints
     ch\_code  : CHECK (code >= 1 AND code <= 9999)
     ch\_jobid : CHECK (jobid >= 'aaaa'::bpchar AND jobid <= 'zzzz'::bpchar)
    
    Foreign keys
     test\_table\_dept\_id\_fkey : FOREIGN KEY (dept\_id) REFERENCES test\_table\_dept..
    
      
      
    
*   **List of tables**  
      
    **pgbash> ?dt**  
    
    \[ List of tables \]
     Schema | Name            | Type  | Owner
    --------+-----------------+-------+--------
     public | member          | table | pgbash
     public | member\_log      | table | pgbash
     public | member\_passwd   | table | pgbash
     public | test            | table | pgbash
     public | test\_inherit    | table | pgbash
     public | test\_table      | table | pgbash
     public | test\_table2     | table | pgbash
     public | test\_table\_dept | table | pgbash
    (8 rows) 
*   **List of indexes**  
      
    **pgbash> ?di**  
    
    \[ List of indexes \]
     Schema | Name                 | Type  | Owner  | Table
    --------+----------------------+-------+--------+-----------------
     public | ix\_jobid             | index | pgbash | test\_table
     public | member\_email         | index | pgbash | member
     public | member\_name          | index | pgbash | member
     public | member\_passwd\_pkey   | index | pgbash | member\_passwd
     public | member\_pkey          | index | pgbash | member
     public | member\_tel           | index | pgbash | member
     public | test\_table2\_pkey     | index | pgbash | test\_table2
     public | test\_table\_dept\_pkey | index | pgbash | test\_table\_dept
     public | test\_table\_pkey      | index | pgbash | test\_table
    (9 rows) 
*   **List of views**  
      
    **pgbash> ?dv**  
    
    \[ List of views \]
     Schema | Name            | Type | Owner
    --------+-----------------+------+--------
     public | member\_log\_view | view | pgbash
     public | test\_view       | view | pgbash
    (2 rows) 
*   **List of large objects**  
      
    **pgbash> ?dl (or \_lo\_list)**  
    
    \[ List of large objects \]
        OID | Description
    --------+--------------
     118865 | member data1
     118867 | member data2
    (2 rows) 
*   **List of permission**  
      
    **pgbash> ?dp**  
    
    \[ List of access privileges \]
     Schema | Name            | Type     | Access privileges
    --------+-----------------+----------+-------------------------
     public | member          | table    | {pgbash=arwdRxt/pgbash}
     public | member\_log      | table    |
     public | member\_log\_view | view     |
     public | member\_passwd   | table    | {pgbash=arwdRxt/pgbash}
     public | member\_seq      | sequence |
     public | test            | table    |
     public | test\_inherit    | table    |
     public | test\_sequence   | sequence |
     public | test\_table      | table    |
     public | test\_table2     | table    |
     public | test\_table\_dept | table    |
     public | test\_view       | view     |
    (12 rows) 
*   **List of sequnces**  
      
    **pgbash> ?ds**  
    
    \[ List of sequences \]
     Schema | Name          | Type     | Owner
    --------+---------------+----------+--------
     public | member\_seq    | sequence | pgbash
     public | test\_sequence | sequence | pgbash
    (2 rows)
    
      
      
    pgbash> **disconnect all;**  
    pgbash> **exit**  
    prompt>
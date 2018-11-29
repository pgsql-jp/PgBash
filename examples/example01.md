* * *

**Example of a shell script**  

* * *

  
1\. Edit the "member.ddl" file. (vi member.ddl)  
2\. Change the permission. (chmod u+x member.ddl)  
3\. Execute the shell script ( ./member.ddl)  
  
(Example of executing a shell script)  
./member.ddl postgres@w1.ppp.com postgres NULL  
./member.ddl admin@w2.ppp.com admin xxxxxx  
./member.ddl xxx@yyy.zzz admin NULL  
  
  
"member.ddl" file  

**#!/usr/local/bin/pgbash**
######################################################################
#  Create Schema member.
#
#  ex)  ./member.ddl  postgres@db.ppp.com  postgres  NULL
#
  PGDATABASE='$1'
  PGUSER='$2'
  PGPASSWORD='$3'

  PGUSER2='admin3'
######################################################################

#---------- connect to DATABASE ----------
connect to $PGDATABASE user $PGUSER $PGPASSWORD;
set client\_encoding='SJIS';

#---------------------------------------------------------------------
#  MEMBER table
#---------------------------------------------------------------------
exec\_sql -q "drop table member;"

create table member (
   userid       int4 primary key not null,
   kind     	varchar(12) 
                check(kind in ('\*','net','post')) 
		default '\*',
   staff	varchar(24) default '',
   name         varchar(32) not null,
   zip		char(8)     
                check(zip between '000-0000' and '999-9999') 
		default '000-0000',
   address1	varchar(64) not null,
   address2	varchar(64),
   tel		varchar(16) not null,
   email        varchar(64),
   org		varchar(128),
   new\_date	date        default 'now',
   up\_date	timestamp   default 'now'
);

grant all    on member to $PGUSER;
grant select on member to $PGUSER2;

#---------------------------------------------------------------------
#  MEMBER table indexes
#---------------------------------------------------------------------
create unique index member\_tel on member(tel) ;
create        index member\_name on member(name) ;
create        index member\_email on member(email) ;

#---------------------------------------------------------------------
#  MEMBER PASSWORD table
#---------------------------------------------------------------------
exec\_sql -q "drop table member\_passwd;"

create table member\_passwd (
   userid       int4 primary key not null
                references member(userid)
                on delete cascade,
   passwd       varchar(17) not null
);

grant all    on member\_passwd to $PGUSER;
grant select on member\_passwd to $PGUSER2;

#---------------------------------------------------------------------
#  MEMBER SEQUENCE table
#---------------------------------------------------------------------
exec\_sql -q "drop sequence member\_seq;"

create sequence member\_seq start 1301;

#---------------------------------------------------------------------
#  MEMBER LOG table
#---------------------------------------------------------------------
exec\_sql -q "drop table member\_log;"

create table member\_log (
   userid int4, name varchar(32), 
   old\_zip char(8), old\_address1 varchar(64), old\_address2 varchar(64), 
   old\_tel varchar(16), old\_email varchar(64), 
   old\_org varchar(128), 
   new\_zip char(8), new\_address1 varchar(64), new\_address2 varchar(64), 
   new\_tel varchar(16), new\_email varchar(64), 
   new\_org varchar(128), 
   up\_date TIMESTAMP
);

#
#  Create rule MEMBER\_RULE
#
create rule member\_rule as on UPDATE to member DO 
   insert into member\_log values(
   old.userid, old.name,
   old.zip, old.address1, old.address2, 
   old.tel,     old.email, 
   old.org,
   new.zip, new.address1, new.address2, 
   new.tel,     new.email, 
   new.org, 
   CURRENT\_TIMESTAMP
);

#
#  Create view MEMBER\_LOG\_VIEW 
#
exec\_sql -q "drop view member\_log\_view;"

create view member\_log\_view as 
   select '' as "stat", a.userid, a.name, 
          a.old\_zip as "zip", 
          a.old\_address1 as "address1", 
          a.old\_address2 as "address2",
          a.old\_tel as "tel", 
          a.old\_email as "email",
          a.old\_org as "org", 
          a.up\_date as "up\_date"
    from member\_log a, member\_log b
    union
    select 'update' as "stat", b.userid, b.name,
          b.new\_zip as "zip", 
          b.new\_address1 as "address1", 
          b.new\_address2 as "address2",
          b.new\_tel as "tel", 
          b.new\_email as "email",
          b.new\_org as "org", 
          b.up\_date as "up\_date"
    from member\_log a, member\_log b
    order by "up\_date";

grant select on member\_log\_view to $PGUSER2;

#---------- disconnect DATABASE ----------
disconnect all;

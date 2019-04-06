# PgBash example-01

* * *

## Example of a shell script

* * *

  
1. Edit the "member.ddl" file. (vi member.ddl)  
2. Change the permission. (chmod u+x member.ddl)  
3. Execute the shell script ( ./member.ddl)  
  
(Example of executing a shell script)  
> ./member.ddl postgres@w1.ppp.com postgres NULL  
> ./member.ddl admin@w2.ppp.com admin xxxxxx  
> ./member.ddl xxx@yyy.zzz admin NULL  
  
  
"member.ddl" file  
~~~
#!/usr/local/bin/pgbash
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
set client_encoding='SJIS';

#---------------------------------------------------------------------
#  MEMBER table
#---------------------------------------------------------------------
exec_sql -q "drop table member;"

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
   new_date	date        default 'now',
   up_date	timestamp   default 'now'
);

grant all    on member to $PGUSER;
grant select on member to $PGUSER2;

#---------------------------------------------------------------------
#  MEMBER table indexes
#---------------------------------------------------------------------
create unique index member_tel on member(tel) ;
create        index member_name on member(name) ;
create        index member_email on member(email) ;

#---------------------------------------------------------------------
#  MEMBER PASSWORD table
#---------------------------------------------------------------------
exec_sql -q "drop table member_passwd;"

create table member_passwd (
   userid       int4 primary key not null
                references member(userid)
                on delete cascade,
   passwd       varchar(17) not null
);

grant all    on member_passwd to $PGUSER;
grant select on member_passwd to $PGUSER2;

#---------------------------------------------------------------------
#  MEMBER SEQUENCE table
#---------------------------------------------------------------------
exec_sql -q "drop sequence member_seq;"

create sequence member_seq start 1301;

#---------------------------------------------------------------------
#  MEMBER LOG table
#---------------------------------------------------------------------
exec_sql -q "drop table member_log;"

create table member_log (
   userid int4, name varchar(32), 
   old_zip char(8), old_address1 varchar(64), old_address2 varchar(64), 
   old_tel varchar(16), old_email varchar(64), 
   old_org varchar(128), 
   new_zip char(8), new_address1 varchar(64), new_address2 varchar(64), 
   new_tel varchar(16), new_email varchar(64), 
   new_org varchar(128), 
   up_date TIMESTAMP
);

#
#  Create rule MEMBER_RULE
#
create rule member_rule as on UPDATE to member DO 
   insert into member_log values(
   old.userid, old.name,
   old.zip, old.address1, old.address2, 
   old.tel,     old.email, 
   old.org,
   new.zip, new.address1, new.address2, 
   new.tel,     new.email, 
   new.org, 
   CURRENT_TIMESTAMP
);

#
#  Create view MEMBER_LOG_VIEW 
#
exec_sql -q "drop view member_log_view;"

create view member_log_view as 
   select '' as "stat", a.userid, a.name, 
          a.old_zip as "zip", 
          a.old_address1 as "address1", 
          a.old_address2 as "address2",
          a.old_tel as "tel", 
          a.old_email as "email",
          a.old_org as "org", 
          a.up_date as "up_date"
    from member_log a, member_log b
    union
    select 'update' as "stat", b.userid, b.name,
          b.new_zip as "zip", 
          b.new_address1 as "address1", 
          b.new_address2 as "address2",
          b.new_tel as "tel", 
          b.new_email as "email",
          b.new_org as "org", 
          b.up_date as "up_date"
    from member_log a, member_log b
    order by "up_date";

grant select on member_log_view to $PGUSER2;

#---------- disconnect DATABASE ----------
disconnect all;
~~~


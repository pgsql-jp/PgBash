* * *

**Example of a Web application.**

* * *

**HTML**

<HTML>
<HEAD>
<META HTTP-EQUIV=Content-Language CONTENT=ja>
<META HTTP-EQUIV=Content-Type CONTENT='text/html; charset=UTF-8'>
</HEAD>
<BODY BGCOLOR=#DDDDDD>

<FORM METHOD="POST" ACTION="/cgi-bin/**webapi.cgi**">

Database 
         <input type="text" name="**pgdatabase**" length=32>
Username 
         <input type="text" name="**pguser2**" length=32>
Password 
         <input type="text" name="**pgpassword2**" length=32>

<INPUT TYPE=SUBMIT VALUE="submit">
</FORM>

</BODY>
</HTML>

  
**Web application (CGI) -- "webapi.cgi"**

#!/usr/local/bin/pgbash
##########################################################################
#  "webapi.cgi"
#
#   $pgdatabase   # database name
#   $pguser2      # database user
#   $pgpassword2  # database password
#  
##########################################################################
exec 2>&1
echo "Content-type:  text/html"
echo ""
**set EXEC\_SQL\_OPTION CGI;**

#--------- start HTML ----------
echo "<HTML>"
echo "<HEAD>"
echo "<META HTTP-EQUIV=Content-Language CONTENT=ja>"
echo "<META HTTP-EQUIV=Content-Type CONTENT='text/html; charset=UTF-8'>"
echo "</HEAD>"
echo "<BODY BGCOLOR=#DDDDDD>"

#-------- connect to DATABASE ---------
**connect to $pgdatabase user $pguser2  $pgpassword2;**

if(( SQLCODE != 0 )); then
   exit
fi

#---------- client\_encoding -----------
**set client\_encoding='UTF8';**

#------------ execute SQL -------------
**select \* from member;**

if (( SQLCODE == 0 && SQLNTUPLE == 0 )); then
echo "<H2>Data not found.</H2>"
fi

#------- disconnect DATABASE ----------
**disconnect all;**

echo "</BODY>"
echo "</HTML>"

  
**Output**

userid

name

email

tel

kind

....

1220

XXXXXXX

mmmmmmmm@nifty.ne.jp

0726-76-9999

\*

 

1249

XXXXXX

mmmmmmm@email.ne.jp

090-7103-9999

\*

 

1217

XXXX

mmmmmmmm@nifty.ne.jp

042-571-9999

NET

 

1221

XXXX

mmmmm@mbg.sphere.ne.jp

0726-27-9999

\*

 

1250

XXXX

mmmmmmm@oct.zaq.ne.jp

06-6622-9999

\*

 

1271

XXXXX

mmmmmmmm@nifty.ne.jp

06-6872-9999

\*

 

1222

XXXX

mmmmmmmm@nifty.ne.jp

0798-72-9999

\*

 

1212

XXXX

mmmmmm@sannet.ne.jp

0725-20-9999

\*

 

1203

XXXX

mmmm@computer.com

0742-40-9999

\*

 

1223

XXXX

mmmmmmmm@nifty.ne.jp

0742-45-9999

\*
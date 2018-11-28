\[[Home](index-j.html)\] \[[ダウンロード](download-j.html)\] \[[インストール](install-j.html)\] \[[使用方法](usage-j.html)\] \[[使用例](example-j.html)\] \[[FAQ](faq-j.html)\]

  
**pgbash のダウンロード**  

  

このページから、pgbashのソースファイルをダウンロードすることができます。

  
  

**◎ pgbash**  

pgbashの最新バージョンは 8r3.2, 8r4.1 です。r3.2はbash-3.2.48、r4.1 はbash-4.1.10をベースにしてパッチをあてています。  
  
[pgbash-8r3.2.tar.gz](./download/pgbash-8r3.2.tar.gz)  
[pgbash-8r4.1.tar.gz](./download/pgbash-8r4.1.tar.gz)  
  
Unixシェルとして使用している bashバージョンが 3.xの場合、pgbash-8r3.2がお勧めです。同様に、bashバージョン 4.1を使用している場合は、pgbash-8r4.1がお勧めです。  
  
但し、Unixシェルとしてbash-3.xを使用している場合でも、pgbash-8r4.1を使用することは可能です。  
  
**◎ pgbashrc**  

pgbashrcは、 対話型環境において psqlと同等の機能を実現するため環境設定ファイルです。  
  
pgbashrc ファイルは、pgbash-8rx.x.tar.gz の中に梱包されていますので、通常は以下のファイルをダウンロードする必要はありません。pgbashrcの新バージョンが公開されたときのみダウンロードしてください。  
  
[pgbashrc-9.0](./download/pgbashrc-9.0)  
  
pgbashは、まずホームディレクトリの ~/.pgbashrc を読み、存在してなければ /etc/pgbashrc を読みます。  
  
PostgreSQLサーバのバージョンが PostgreSQL-8.1 の場合、pgbashrc-8.1 を **/etc/pgbashrc もしくは ~/.pgbashrc としてコピー**します。

  
  
**◎ その他のツール**  

・PostgreSQLの開発環境  
**libpq-fe.h** インクルードファイルおよびそれが参照するインクルードファイルと、**libpq.so** (もしくは **libpq.a**) ライブラリが必要です。  
  
**libpq ライブラリのバージョンは、PostgreSQL-7.4 以降であれば、どのバージョンでもかまいません**。PostgreSQLサーバのバージョンとは無関係にリンクすることができます。  
  
例えば、pgbashを PostgreSQL-7.4の開発環境で作成し、作成された pgbash を使用して PostgreSQL-9.0 のデータベースにアクセス可能です。(この場合、pgbashrc-9.0 を使用します）  
  
・ Pgbashのコンパイルは、PostgreSQLのコンパイルと同様に GNU make (gmake), gcc, bison, flex を使用します。  
  

* * *

\[[Home](index-j.html)\] \[[ダウンロード](download-j.html)\] \[[インストール](install-j.html)\] \[[使用方法](usage-j.html)\] \[[使用例](example-j.html)\] \[[FAQ](faq-j.html)\]

>
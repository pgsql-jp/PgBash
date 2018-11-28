\[[Home](index-j.html)\] \[[ダウンロード](download-j.html)\] \[[インストール](install-j.html)\] \[[使用方法](usage-j.html)\] \[[使用例](example-j.html)\] \[[FAQ](faq-j.html)\]

  
**pgbash のインストール**  

  

pgbash のインストール方法について説明します。pgbash のバージョン番号が変更された場合は、以下の説明にあるバージョン番号を新しい番号に置き換えてください。  

  

* * *

  
**◎ PostgreSQL開発環境のインストール**  

pgbashは、libpq-fe.h および PostgreSQL関連インクルードファイル、libpqライブラリを必要とします。そのため、pgbashをインストールする前に PostgreSQLの開発環境をインストールしておく必要があります。  
  
例えば、 yum や Cygwin では **postrgesql-devel** をインストールしておきます。  

  

* * *

  
**◎ pgbashのインストール**  

2.  **pgbash-8rx.x.tar.gz の展開**  
      
    tar xvfz pgbash-8rx.x.tar.gz  
      
    pgbash-8rx.x ディレクトリの中に次のようなディレクトリおよびファイルが作成されます。  
    
    ChangeLog    ........... 変更情報のファイル
    INSTALL      ........... インストール方法の説明ファイル
    README       ........... pgbashの説明ファイル
    rc/          ........... pgbashrcファイル格納ディレクトリ
    src/         ........... pgbashのソース格納ディレクトリ
    
      
    srcディレクトリに移動します。  
    **cd pgbash-8rx.x/src**  
      
      
    
3.  **Configure**  
      
    pgbashは libpq.so ライブラリを必要とします。もし、libpq.a ライブラリのみ存在している場合は、次のように"Makefile.in"ファイルを編集した後に configure してください。  
      
    ("**Makefile.in**"ファイルの編集)  
    変更前：**POSTGRESQL\_LIBS= @pglib\_prefix@/libpq.so**  
    変更後：**POSTGRESQL\_LIBS= @pglib\_prefix@/libpq.a**  
      
      
    **CFLAGS='-O2' ./configure**     <---- デバッグオプション無し  
    もしくは  
    **./configure**     < ---- デバッグオプション有り(CFLAGS='-g -O2')  
      
      
    **/usr/include** に libpq-fe.h 、**/usr/lib** に libpq.so(もしくは libpq.a)が存在していない場合は、--pginc-prefix, --pglib-prefix オプションでディレクトリを指定します。  
    
    (例) 
    CFLAGS='-O2' ./configure --pginc-prefix=/usr/local/include   \\
                             --pglib-prefix=/usr/local/lib
    
      
    
4.  **Makeとインストール** (GNU makeを使用)  
      
    **make**  
    **su**  
    **make install**　　　<--- pgbashのインストール  
    **cd ../rc**  
    **make install**　　　<--- pgbashrcのインストール  
      
    Unixスーパユーザになって **make install**を実行します。make install は、pgbashを /usr/local/bin へ、pgbashrcを /etc/ へコピーします。  
      
    もし、Unixスーパユーザで作業できない場合は、pgbash を適当なディレクトリにコピーしてください。また、pgbashrc を .pgbashrc にしてホームディレクトリへコピーしてください。  
    

  

* * *

  
**◎ 環境の設定**  

Pgbashを対話型で使用する場合、pgbash起動時は bashシェルと同様に環境設定ファイル(~/.bash\_profile, ~/.bashrcなど）を読み込みます。  
  
その後、pgbashは、ホームディレクトリの.pgbashrc(**~/.pgbashrc**)を検索し、無ければ **/etc/pgbashrc** を読み込みます。  
  
*   **PostgreSQLサーバ・バージョンに合わせたpgbashrcを使用**  
      
    pgbashrcは、PostgreSQLサーバのバージョンに合わせて次の pgbashrc を使用する必要があります。  
      
    
    PostgreSQL  
    サーバ・バージョン
    
    使用する pgbashrc
    
    PostgreSQL 7.4
    
    pgbashrc-7.4
    
    PostgreSQL 8.0
    
    pgbashrc-8.0
    
    PostgreSQL 8.1
    
    pgbashrc-8.1
    
    PostgreSQL 8.2
    
    pgbashrc-8.2
    
    PostgreSQL 8.3
    
    pgbashrc-8.3
    
    PostgreSQL 8.4
    
    pgbashrc-8.4
    
    PostgreSQL 9.0
    
    pgbashrc-9.0
    
      
    例えば、サーバにPostgreSQL-8.1 を使用している場合、pgbashrc-8.1 を /etc/pgbashrc(もしくは ~/.pgbashrc) としてコピーします。  
      
    
*   **ユーザ固有の環境設定ファイル(pgbashrc)を作成したい場合**  
      
    ユーザ固有の環境を設定したい場合は、pgbashrc を ~/.pgbashrc としてコピーし、そのファイルを編集します。  
      
    
*   **pgbashをログインシェルとして使用する場合**  
      
    "/etc/passwd"ファイルを編集してログイン時起動プログラムに /usr/local/bin/pgbash を指定し、"/etc/shells"ファイルに /usr/local/bin/pgbash の追加を行なう必要があります。  
      
    通常は、.bash\_profile は .bashrc を読み込むだけの命令を記述し、環境設定は .bashrc に設定します。そうすれば、ログインシェルとサブシェルは同じ環境になります。  
      
    (".bash\_profile"の例)  
    
    if \[ -f ~/.bashrc \]; then
        . ~/.bashrc
    fi
    
      
    
*   **動作確認**  
      
    **/usr/local/bin/pgbash**を実行すると次のように表示されます。  
    prompt> **/usr/local/bin/pgbash**
    
    Welcome to Pgbash ( bash-x.x.x-release ) Patch Ver.8 rX.X
    
      Type '?'  for help with pgbash commands.
      Type 'exit' or 'Ctrl+D' to terminate Pgbash.
    
      
    

  

  

* * *

\[[Home](index-j.html)\] \[[ダウンロード](download-j.html)\] \[[インストール](install-j.html)\] \[[使用方法](usage-j.html)\] \[[使用例](example-j.html)\] \[[FAQ](faq-j.html)\]
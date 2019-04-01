\[[Prev](../usage-j.md)\] \[[Menu](../usage-j.md)\] \[[Next](./usage02-j.md)\]

* * *

# pgbash の起動と終了

## 対話型環境<a id="TTY" name="TTY">&nbsp;</a>

対話型環境において pgbashをサブシェル（親のシェルから起動された子のシェル）として起動するには、ログインした後に /usr/local/bin/pgbash とタイプします。pgbashが起動されると、次の Welcomeメッセージが表示されます。  

例）

    prompt> /usr/local/bin/pgbash
    Welcome to Pgbash ( bash-x.x.x-release ) Ver.8 rx.x
        
    Type '?'  for help with pgbash commands.
    Type 'exit' or 'Ctrl+D' to terminate Pgbash.
        
    pgbash> exit

最後に **exit** (もしくは Ctrl+D)を入力すると、pgbashは終了し元のログインシェルに戻ります。  

対話型環境では、pgbashは**起動時に ~/.bashrcを読みその後 ~/.pgbashrcを読みます**。  
~/.pgbashrcが存在しない場合は、**/etc/pgbashrc** を読み込みます。   
尚、任意の場所の pgbashrc を使用する場合や、使用中に pgbashrc を変更した場合は、次のように source コマンドを使用して環境を更新します。  

例）

    pgbash> source ファイル名 (例: source /etc/pgbashrc)  

## シェルスクリプト<a id="SHELL" name="SHELL">&nbsp;</a>

シェルスクリプトとして Pgbashを起動するには次のようにします。  

1. エディタでシェルスクリプトを作成  
2. 「chmod +x シェルスクリプトファイル名」として実行許可を与える  
3. コマンドラインからシェルスクリプト名を指定して実行  

シェルスクリプトの終了で pgbashが終了することになります。  

シェルスクリプトは、スクリプトの先頭に #!/usr/local/bin/pgbash を記述します。  
尚、シェルスクリプトとして pgbashを起動した場合、**~/.bashrc や  ~/.pgbashrc を読み込みません**。  
~/.pgbashrc の環境を読み込みたい場合は、source コマンドで指定します。  

例）

    #!/usr/local/bin/pgbash
    ...
    #

(**注意**)  

+シェルスクリプトの**改行コードは LF コード**です。  
CR+LF では、シェルスクリプトは実行できません。例えば、Windows のパソコン上で作成したファイルをバイナリモードでアップロードした場合などは、改行コードが CR+LF になってしまいますので注意が必要です。  

+シェルスクリプト内では alias を使用できません。  
pgbashrcに alias を使って記述されたデータベース情報表示機能を使用する場合、source コマンドで pgbashrc を読み込んだ後に、pgbashrcに定義された関数名を指定します。  

## CGI（Webアプリケーション）<a id="CGI" name="CGI">&nbsp;</a>

CGI（Webアプリケーション)もシェルスクリプトです。ただ、Webから実行する点で、通常のシェルスクリプトとは異なります。Pgbashを起動するには次のようにします。  

1. エディタでCGIを作成  
2. cgi-bin ディレクトリ(CGIを実行できるディレクトリ)にコピーする  
3. 「chmod +x CGIファイル名」として実行許可を与える  
4. HTML の FORM文の中から CGIファイル名を指定して実行  

CGIの先頭には、次のような指定を行います。  

例）

    #!/usr/local/bin/pgbash
    exec 2>&1   .............................bashのエラーメッセージを表示
    echo "Content-type: text/html" ..........必須
    echo ""   ...............................必須
    set EXEC_SQL_OPTION CGI; ................必須(CGIモードの切り替え)

exec 2>&1 は、bashのエラーメッセージをホームページに出力するための指示です。  

"Content-type:text/html" と "" の改行は、CGIプログラムにするための必須の出力です。  

「set EXEC_SQL_OPTION CGI」は、ホームページから入力されたデータ(GET/POSTで送信されたデータ)を解釈して、HTMLの INPUT文の NAMEで指定された名前と同じ名前のシェル変数に格納します。  
また、pgbashをCGIプログラムとして動作させるための各種初期設定を行います。  

* * *

\[[Prev](../usage-j.md)\] \[[Menu](../usage-j.md)\] \[[Next](./usage02-j.md)\]

[[Prev](./usage10-j.md)] [[Menu](./readme.md)] [[End](./readme.md)]

* * *

# 使用制限

PgBashの使用上の制限について説明します。（SQL の指定方法の制限につきましては、[SQLの指定方法](usage02-j.html#LIMIT)を参照してください。）  

## bashコマンド<a id="BASH" name="BASH">&nbsp;</a>

bashの declare、set、selectコマンドは、SQL文の先頭語と同じになります。PgBashでは、SQL文を優先的に取り扱いますので、bashのコマンド名を次のように変更してます。  

<dl>
        <dt>bashのdeclareコマンド</dt>
        <dd>declares<dd>
        <dt>bashのsetコマンド</dt>
        <dd>sets</dd>
        <dt>bashのselectコマンド</dt>
        <dd>selects</dd>
</dl>

## シェル変数の制限<a id="SHELLVAR" name="SHELLVAR">&nbsp;</a>

### (1) SQLCODEシェル変数

SQLCODEシェル変数($SQLCODEなど)は、SQLの実行結果コードを保管したシェル変数です。SQLをパイプライン処理( | more など)やバックグランドジョブ(SQL; & )で実行した場合、SQLCODEシェル変数の値を参照することはできません。  

これは、パイプライン処理とバックグランド処理が、別プロセスで起動されるため、もとのプロセスにシェル変数の値を返せないからです。例えば次のような使い方は正しくありません。  

例)

    pgbash> select * from test; | more  .... パイプを使用
    pgbash> echo $SQLCODE (または ?s)   ...上のselect文の実行結果は参照できません。
    pgbash> select * from test; & ........... バックグランドジョブ
    pgbash> echo $SQLCODE (または ?s)   ...上のselect文の実行結果は参照できません。

### (2) 検索結果を受け取るシェル変数

上記で説明した理由と同じ理由で、パイプやバックグランドジョブを使用すると、検索結果を受け取るシェル変数の値を参照することができません。  

例)

    pgbash> select col1,col2 into :aa, :bb from test; | more ... $aa, $bbの値を参照できません
    pgbash> select col1,col2 into :aa, :bb from test; & ... $aa, $bbの値を参照できません

### (3) ユーザが使用するシェル変数

[PgBashは英大文字のシェル変数を使用](usage07-j.html)していますので、ユーザが使用するシェル変数は英小文字の方が無難です。  

## PostgreSQLサーババージョンと互換性<a id="VERSION" name="VERSION">&nbsp;</a>

PgBashは、PostgreSQL-7.4 以降に対応してます。基本的には、PgBashはリンクした libpq のバージョンと、アクセスするデータベースのバージョンが異なっていても問題ありません。  

* * *

[[Prev](./usage10-j.md)] [[Menu](./readme.md)] [[End](./readme.md)]

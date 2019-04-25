[[Prev](./usage08-j.md)] [[Menu](./readme.md)] [[Next](./usage11-j.md)]

* * *

# ラージオブジェクトの取扱

## _lo_import<a id="LO_IMPORT" name="LO_IMPORT">&nbsp;</a>

**_lo_import ファイル名 '注釈'**  

_lo_import は、ファイル名と注釈を指定して、ラージオブジェクトをデータベースに登録します。正常に登録された場合は、"BEGIN","INSERT OID","COMMIT"のメッセージが表示されます。  

例)  

    pgbash> _lo_import member.dat 'member data'  
    BEGIN  
    INSERT 118868 1  
    COMMIT  

## _lo_export<a id="LO_EXPORT" name="LO_EXPORT">&nbsp;</a>

_lo_export は、データベースに登録されたラージオブジェクトをファイルに取り出します。例えば、OIDが 11885 のラージオブジェクトを /tmp/mm ファイルに取り出すには次のようにします。  

例)  

    _lo_export 11885 /tmp/mm  

## _lo_unlink<a id="LO_UNLINK" name="LO_UNLINK">&nbsp;</a>

_lo_unlink は、データベースに登録されたラージオブジェクト情報を削除します。正常に削除された場合は、"BEGIN","DELETE 件数","COMMIT"のメッセージが表示されます。  

例)  

    _lo_unlink 11885  
    BEGIN  
    DELETE 1  
    COMMIT  

## _lo_list<a id="LO_LIST" name="LO_LIST">&nbsp;</a>

_lo_list は、登録されているラージオブジェクトの一覧を表示します。  

例)  

    pgbash> _lo_list  
    
     objoid | description
    --------+--------------
     118865 | member data1
     118867 | member data2
    (2 rows)

## pgbash_description テーブル<a id="DESCRIPTION" name="DESCRIPTION">&nbsp;</a>

PgBashは、ラージオブジェクトのOIDと注釈を、"pgbash_description"テーブルに登録します。"pgbash_description"テーブルは、始めて _lo_import が実行されたときに自動的に作成されます。  
pgbash_descriptionテーブルは、ユーザの権限で作成されるため、Postgresスーパユーザでなくても、ラージオブジェクトの登録が可能です。  

* * *

[[Prev](./usage08-j.md)] [[Menu](./readme.md)] [[Next](./usage11-j.md)]

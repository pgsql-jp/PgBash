\[[Prev](usage08-j.md)\]

\[[Menu](../usage-j.md)\]

\[[Next](usage11-j.md)\]

* * *

**ラージオブジェクトの取扱**  

* * *

  

*   **\_lo\_import**  
      
    **\_lo\_import ファイル名 '注釈'**  
      
    \_lo\_import は、ファイル名と注釈を指定して、ラージオブジェクトをデータベースに登録します。正常に登録された場合は、"BEGIN","INSERT OID","COMMIT"のメッセージが表示されます。  
      
    例)  
    pgbash> \_lo\_import member.dat 'member data'  
    BEGIN  
    INSERT 118868 1  
    COMMIT  
      
      
    
*   **\_lo\_export**  
      
    \_lo\_export は、データベースに登録されたラージオブジェクトをファイルに取り出します。例えば、OIDが 11885 のラージオブジェクトを /tmp/mm ファイルに取り出すには次のようにします。  
      
    例)  
    \_lo\_export 11885 /tmp/mm  
      
      
    
*   **\_lo\_unlink**  
      
    \_lo\_unlink は、データベースに登録されたラージオブジェクト情報を削除します。正常に削除された場合は、"BEGIN","DELETE 件数","COMMIT"のメッセージが表示されます。  
      
    例)  
    \_lo\_unlink 11885  
    BEGIN  
    DELETE 1  
    COMMIT  
      
      
    
*   **\_lo\_list**  
      
    \_lo\_list は、登録されているラージオブジェクトの一覧を表示します。  
      
    pgbash> \_lo\_list  
    
     objoid | description
    --------+--------------
     118865 | member data1
     118867 | member data2
    (2 rows)
    
      
    
*   **pgbash\_description テーブル**  
      
    Pgbashは、ラージオブジェクトのOIDと注釈を、"pgbash\_description"テーブルに登録します。"pgbash\_description"テーブルは、始めて \_lo\_import が実行されたときに自動的に作成されます。  
      
    pgbash\_descriptionテーブルは、ユーザの権限で作成されるため、Postgresスーパユーザでなくても、ラージオブジェクトの登録が可能です。  
    

  

* * *

\[[Prev](./usage08-j.md)\]

\[[Menu](../usage-j.md)\]

\[[Next](./usage11-j.md)\]

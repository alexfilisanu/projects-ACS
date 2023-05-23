-- sa se afiseze numele, gradul salarial si numarul de subordonati pentru seful cu cei
-- mai multi subordonati din firma
-- nu se va tine cont de angajatii din departamentul research

SELECT A.ENAME, B.GRADE, (SELECT COUNT(*)
                            FROM EMP C
                            WHERE C.MGR = A.EMPNO
                            AND C.DEPTNO != (SELECT D.DEPTNO
                                                FROM DEPT D
                                                WHERE D.DNAME = 'RESEARCH')) NR_SUBORDONATI
    FROM EMP A JOIN SALGRADE B ON A.SAL BETWEEN B.LOSAL AND B.HISAL
    WHERE A.EMPNO = (SELECT E.MGR
                        FROM EMP E
                        WHERE E.DEPTNO != (SELECT F.DEPTNO
                                            FROM DEPT F
                                            WHERE F.DNAME = 'RESEARCH')
                        GROUP BY E.MGR
                        HAVING COUNT(*) = (SELECT MAX(COUNT(*))
                                            FROM EMP G
                                            WHERE G.DEPTNO != (SELECT H.DEPTNO
                                                                FROM DEPT H
                                                                WHERE H.DNAME = 'RESEARCH')
                                            GROUP BY G.MGR))
    AND A.DEPTNO != (SELECT I.DEPTNO 
                        FROM DEPT I 
                        WHERE I.DNAME = 'RESEARCH');

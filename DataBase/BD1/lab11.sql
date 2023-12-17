/* Sa se selecteze angajatii din acelasi grad salarial cu Allen care au primele
 doua venituri din acest grad salarial. Veti afisa nume angajat, venit, grad salarial.
 Sa se creeze un view pe aceasta structura */

CREATE OR REPLACE VIEW V_EMP AS
    SELECT E.ENAME, E.SAL, S.GRADE
    FROM EMP E JOIN SALGRADE S ON E.SAL BETWEEN S.LOSAL AND S.HISAL
    WHERE S.GRADE = (SELECT GRADE
                        FROM EMP E JOIN SALGRADE S ON E.SAL BETWEEN S.LOSAL AND S.HISAL
                        WHERE E.ENAME = 'ALLEN')
    AND 2 >= (SELECT COUNT(DISTINCT A.SAL)
				FROM EMP A JOIN SALGRADE B ON A.SAL BETWEEN B.LOSAL AND B.HISAL
                    WHERE B.GRADE = (SELECT GRADE
                                        FROM EMP E JOIN SALGRADE S ON E.SAL BETWEEN S.LOSAL AND S.HISAL
                                        WHERE E.ENAME = 'ALLEN')
				    AND A.SAL > E.SAL);

SELECT * FROM V_EMP;

DROP VIEW V_EMP;

/* sqlplus: SET PARAM VAL

    SET LINES 200
    SET PAGES 200
    SET TIMING ON / OFF
    SET FEEDBACK ON / OFF
    SET SPACE n
*/

/* Sa se selecteze toti angajatii care nu fac parte din departamentul angajatului
 care a venit ultimul in firma. Raportul va avea un titlu numit 'Situatia ang pe depart',
 un footer intitulat 'Sume pe departamente' si va calcula sumele salariilor angajatilor
 pe fiecare departament in parte*/

TTITLE 'Situatia angajatilor pe departamente'
BTITLE 'Sume pe departamente'

COLUMN SAL HEADING 'SALARIU'
BREAK ON DNAME SKIP 1
COLUMN DEPTNO HEADING 'DEPARTAMENT'
COMPUTE SUM OF SAL ON DNAME
    SELECT A.DNAME, B.ENAME, B.SAL
    FROM DEPT A JOIN EMP B ON A.DEPTNO = B.DEPTNO
    WHERE A.DEPTNO NOT IN (SELECT DEPTNO
                            FROM EMP
                            WHERE HIREDATE = (SELECT MAX(HIREDATE)
                                                FROM EMP))
    ORDER BY A.DNAME;

TTITLE OFF
BTITLE OFF
CLEAR BREAKS
CLEAR COLUMNS

-- exista rabele: user_tables, user_constraints
-- SELECT TABLE_NAME FROM USER_TABLES;

/* Sa se selecteze angajatii din departamentul lui Scott care au salariul in gradul salarial 
 din care fac parte cei mai multi angajati */
SELECT E.ENAME, E.SAL, S.GRADE
    FROM EMP E JOIN SALGRADE S ON E.SAL BETWEEN S.LOSAL AND S.HISAL
    WHERE E.DEPTNO = (SELECT DEPTNO
                        FROM EMP
                        WHERE ENAME = 'SCOTT')
    AND S.GRADE = (SELECT S.GRADE
                    FROM EMP E JOIN SALGRADE S ON E.SAL BETWEEN S.LOSAL AND S.HISAL
                    GROUP BY S.GRADE
                    HAVING COUNT(*) = (SELECT MAX(COUNT(*))
                        FROM EMP E JOIN SALGRADE S ON E.SAL BETWEEN S.LOSAL AND S.HISAL
                        GROUP BY S.GRADE));                     

/* Sa se selecteze gradul salarial care cuprinde cei mai multi angajati */
SELECT S.GRADE
    FROM EMP E JOIN SALGRADE S ON E.SAL BETWEEN S.LOSAL AND S.HISAL
    GROUP BY S.GRADE
    HAVING COUNT(*) = (SELECT MAX(COUNT(*))
                        FROM EMP E JOIN SALGRADE S ON E.SAL BETWEEN S.LOSAL AND S.HISAL
                        GROUP BY S.GRADE);   
                     
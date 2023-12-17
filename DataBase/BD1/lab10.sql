/* GRANT SELECT, INSERT, ON ANG TO SCOTT;
REVOKE INSERT ON ANG FROM SCOTT;
CREATE USER GIGA IDENTIFIED BY GIGA_PASSWORD;

ALTER TABLE TAB ADD CONSTRAINT NUME_CONSTANGERE
                    MODIFY COLUMN4
                    DROP CONSTRAINT NUME_CONSTANGERE;

-- se se creeze o tabela care sa contina toti angajatii din emp care
-- nu au gradul salarial egal cu cel al lui Blake, tabela urmand a se numi
-- "DIFERIT_BLAKE"
CREATE TABLE DIFERIT_BLAKE AS
    SELECT *
    FROM EMP E JOIN SALGRADE S ON E.SAL BETWEEN S.LOSAL AND S.HISAL
    WHERE S.GRADE != (SELECT GRADE
                    FROM EMP E JOIN SALGRADE S ON E.SAL BETWEEN S.LOSAL AND S.HISAL
                    WHERE ENAME = 'BLAKE');

-- adaugati o constrangere de tip check(verifica ca o conditie sa fie indeplinita
-- pe acea coloana) care sa verifice ce salariu <= 7000
ALTER TABLE DIFERIT_BLAKE
    ADD CONSTRAINT CHECK_SAL CHECK(SAL <= 7000);

-- adaugati o inregistare cu un angajat care sa aiba salariul 7500
INSERT INTO DIFERIT_BLAKE
    VALUES(8000, 'GIGA', 'MANAGER', 7839, TO_DATE('17-DEC-1980', 'DD-MM-YYYY'), 7500, NULL, 10);

-- drop constrangere
ALTER TABLE DIFERIT_BLAKE
    DROP CONSTRAINT CHECK_SAL;

-- adaugati inregistraraea respectiva
INSERT INTO DIFERIT_BLAKE
    VALUES(8000, 'GIGA', 'MANAGER', 7839, TO_DATE('17-DEC-1980', 'DD-MM-YYYY'), 7500, NULL, 10);

-- adaugati coloana "sal2" pe care o calculati printr un update pe toate inregistrarile
-- ca fiind 'sal + nvl(comm, 0)'
ALTER TABLE DIFERIT_BLAKE
    ADD SAL2 NUMBER(6, 2);

-- stergeti "DIFERIT_BLAKE"
DROP TABLE DIFERIT_BLAKE;

*/

-- Sa se creeze o tabela numita ULTIMII_VENITI care sa contina angajtii
-- veniti ultimii din departamentul lor
-- nu vor instra in calcul cei care s au ingajat intr o luna de primavara
-- structura tabel: nume angajat, denumire departament, luna angajare, data angajare, ani vechime
-- la final drop table
CREATE TABLE ULTIMII_VENITI AS


CREATE TABLE ULTIMII_VENITI AS
    SELECT E.ENAME, D.DNAME, TO_CHAR(E.HIREDATE, 'MONTH') LUNA_ANGAJARE, E.HIREDATE, TRUNC(MONTHS_BETWEEN(SYSDATE, E.HIREDATE) / 12) ANI_VECHIME
    FROM EMP E JOIN DEPT D ON E.DEPTNO = D.DEPTNO
    WHERE TO_CHAR(E.HIREDATE, 'MONTH') NOT IN ('MARCH', 'APRIL', 'MAY')
    AND E.HIREDATE = (SELECT MAX(HIREDATE)
                        FROM EMP A 
                        WHERE A.DEPTNO = E.DEPTNO
                            AND TO_CHAR(E.HIREDATE, 'MONTH') NOT IN ('MARCH', 'APRIL', 'MAY'));

SELECT * FROM ULTIMII_VENITI;

DROP TABLE ULTIMII_VENITI;

--- VIEW ---

-- view cu cei care nu s au angajat in luna decembrie

CREATE OR REPLACE VIEW V_EMP AS
    (SELECT EMPNO, ENAME, HIREDATE DATA_NASTERII
        FROM EMP
        WHERE EXTRACT(MONTH FROM HIREDATE) != 12);

SELECT * FROM V_EMP;

DROP VIEW V_EMP;

-- sa se creeze un view denumit ANG_BLAKE care sa contina toti angajatii
-- din departamentul sefului cu cei mai multi subordonati
-- structura view: nume angajat, departament, an_angajare, venit, nr_ani_vechime

SELECT X.ENAME NUME_ANGAJAT, Y.DNAME DEN_DEPARTAMENT, EXTRACT(YEAR FROM X.HIREDATE) 
        AN_ANGAJARE, (X.SAL + NVL(X.COMM, 0)) VENIT, TRUNC(MONTHS_BETWEEN(SYSDATE, X.HIREDATE) / 12) NR_ANI_VECHIME
    FROM EMP X JOIN DEPT Y ON X.DEPTNO = Y.DEPTNO
    WHERE X.DEPTNO IN (SELECT DEPTNO 
                            FROM EMP 
                            WHERE EMPNO = (
                                SELECT A.EMPNO
                                    FROM EMP A JOIN EMP B ON A.EMPNO = B.MGR
                                    GROUP BY A.EMPNO
                                    HAVING COUNT(*) = (SELECT MAX(COUNT(*))
                                                        FROM EMP C
                                                        GROUP BY C.MGR)));

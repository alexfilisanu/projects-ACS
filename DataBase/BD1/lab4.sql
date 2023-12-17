CREATE TABLE EMP1 AS
	SELECT EMPNO, ENAME, SAL + NVL(COMM, 0) VENIT
		FROM EMP
		WHERE MGR IS NOT NULL;

SELECT * FROM EMP1;

CREATE TABLE DEPTNOCHICAGO AS
	SELECT *
		FROM DEPT
		WHERE LOC != 'CHICAGO';

SELECT * FROM DEPTNOCHICAGO;

SELECT 1, ENAME, 'ANGAJATUL DIN EMP' || ' ARE ID-UL ' || EMPNO 
	FROM EMP;

-- sa se scrie o cerere sql care face o lista cu toti angajatii din departamente cu id uri diferite
-- de 2 valori citite de la tastatura, care au primit comision
-- numele angajatului se va concatena cu id ul departamentului astfel ename 'face parte din' deptno
-- hiredate
-- venit = sal + comm

ACCEPT DEPAR1 NUMBER PROMPT 'Introduceti primul departament:'
ACCEPT DEPAR2 NUMBER PROMPT 'Introduceti al doilea departament:'

SELECT ENAME || ' FACE PARTE DIN ' || DEPTNO, HIREDATE, SAL + NVL(COMM, 0) VENIT
	FROM EMP
	WHERE DEPTNO != &DEPAR1 AND DEPTNO != &DEPAR2 AND NVL(COMM, 0) != 0
	ORDER BY DEPTNO DESC, ENAME, SAL;	

SELECT ENAME, DNAME
	FROM EMP, DEPT
	WHERE EMP.DEPTNO = DEPT.DEPTNO;

-- sa se faca o lista cu toti angajatii din depart 10 sau 30 care au un salariu > citit la tast si nu au comision
-- numele angajatului concat cu id ul 

ACCEPT SALARIU NUMBER PROMPT 'Introduceti salariul minim:'

SELECT DEPTNO, 'ANGAJATUL ' || ENAME || ' ARE ID UL ' || EMPNO, SAL + NVL(COMM, 0) VENIT, COMM
	FROM EMP
	WHERE (DEPTNO = 10 OR DEPTNO = 30) 
		AND SAL > &SALARIU
		AND NVL(COMM, 0) = 0;	

UNDEFINE DEPAR1;
UNDEFINE DEPAR2;
UNDEFINE SALARIU;

DROP TABLE DEPTNOCHICAGO;
DROP TABLE EMP1;
